#include "DatabasePCH.h"
#include <sne/database/ado/AdoDatabase.h>
#include <sne/database/ado/AdoDatabaseUtils.h>
#include <sne/core/utility/Win32Util.h>

#ifdef _WIN32

namespace sne { namespace database {

namespace
{

template <typename T>
inline void setValueWithString(AdoDatabase& database,
    ADODB::_Parameter& parameter, const T& value)
{
    _bstr_t bstr(value.c_str());

    _variant_t vtVal;
    vtVal.vt = VT_BSTR;
    vtVal.bstrVal = bstr.GetBSTR();

    const long length = static_cast<long>(value.length()); 

    try {
        if (parameter.Size < length) {
            parameter.Size = length;
        }
        parameter.Value = vtVal;
    }
    catch(_com_error& e) {
        database.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException(database.getLastError());
    }
}

} // namespace

// = AdoDatabase

AdoDatabase::AdoDatabase() :
    lastResult_(S_OK)
{
    lastError_[0] = '\0';
}


AdoDatabase::~AdoDatabase()
{
    close();
}


bool AdoDatabase::open(const std::string & connectionString,
    long connectionTimeout)
{
    if (isOpened()) {
        close();
    }
    else {
        if (! initAdo()) {
            return false;
        }
        connection_->CursorLocation = ADODB::adUseClient;
    }

    try {
        lastConnectionString_ = connectionString;
        connection_->ConnectionTimeout = connectionTimeout;
        const HRESULT hr = connection_->Open(lastConnectionString_.c_str(), "", "",
            ADODB::adConnectUnspecified);
        return SUCCEEDED(hr);
    }
    catch (_com_error& e) {
        setLastError(e, __FUNCTION__, __LINE__);
    }
    return false;
}


void AdoDatabase::close()
{
    if (isOpened()) {
        connection_->Close();
        connection_.Release();
    }
}


void AdoDatabase::execute(const std::string & sql)
{
    assert(connection_);

    affectedRecordsCount_ = 0;
    if (sql.empty()) {
        throw AdoException("SQL string is empty");
    }

    _variant_t records;
    try {
        connection_->Execute(_bstr_t(sql.c_str()), &records,
            ADODB::adExecuteNoRecords);
        affectedRecordsCount_ = records.iVal;
    }
    catch (_com_error& e) {
        setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException(lastError_);
    }
}


bool AdoDatabase::isOpened()
{
    try {
        return (connection_) &&
            connection_->GetState() != ADODB::adStateClosed;
    }
    catch (_com_error& e) {
        setLastError(e, __FUNCTION__, __LINE__);
    }
    return false;
}


bool AdoDatabase::initAdo()
{
    const HRESULT hr = connection_.CreateInstance(__uuidof(ADODB::Connection));
    return SUCCEEDED(hr);
}


void AdoDatabase::setLastError(_com_error& e, const char* function, int line)
{
    const char* errorMsg = e.ErrorMessage() != nullptr ? e.ErrorMessage() : "n/a";
    const _bstr_t source(e.Source());
    const _bstr_t description(e.Description());

    lastResult_ = e.Error();

    _snprintf_s(lastError_, _countof(lastError_), MAX_ERROR_LEN,
        "AdoDataBase Error: Code = 0x%X, Code meaning = %s"
        ", Source = %s, Description = %s",
        e.Error(), errorMsg, static_cast<LPCSTR>(source),
        static_cast<LPCSTR>(description));

    SNE_LOG_ERROR("AdoDatabase Error(%s) - %s:%d", lastError_, function, line);
}


void AdoDatabase::ping()
{
    _variant_t records;
    try {
        connection_->Execute(L"SELECT @@VERSION", &records,
            ADODB::adExecuteNoRecords);
        affectedRecordsCount_ = records.iVal;
    }
    catch (_com_error& e) {
        setLastError(e, __FUNCTION__, __LINE__);

        close();
        (void)open(lastConnectionString_, 2);
    }
}


///////////////////////////////////////////////////////////////////////////////
// AdorecordSet

AdoRecordSet::AdoRecordSet(AdoDatabase& database) :
    database_(database)
{
}


AdoRecordSet::~AdoRecordSet()
{
    close();
    releaseRecordSet();
}


bool AdoRecordSet::open(const std::string & query)
{
    try {
        if (recordSet_ == 0) {
            const HRESULT hr = recordSet_.CreateInstance(__uuidof(ADODB::Recordset));
            if (FAILED(hr)) {
                SNE_ASSERT(false);
                return false;
            }
        }
        recordSet_->Open(query.c_str(),
            _variant_t(
                static_cast<IDispatch*>(database_.getConnection()), TRUE), 
            ADODB::adOpenForwardOnly,
            ADODB::adLockReadOnly, //adLockOptimistic,
            ADODB::adCmdUnknown);
        return true;
    }
    catch (_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
    }
    return false;
}


void AdoRecordSet::close()
{
    if (isOpened()) {
        try {
            recordSet_->Close();
        }
        catch (_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
        }
    }
}


bool AdoRecordSet::isOpened()
{
    try {
        return recordSet_ &&
            (recordSet_->GetState() != ADODB::adStateClosed);
    }
    catch (_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
    }
    return false;
}


void AdoRecordSet::execute(AdoCommand& cmd)
{
    releaseRecordSet();

    recordSet_ = cmd.execute();
}


bool AdoRecordSet::isEof()
{
    if (recordSet_ == 0) {
        return true;
    }
    
    try {
        return recordSet_->EndOfFile == VARIANT_TRUE;
    }
    catch (_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
    }
    return true;
}


void AdoRecordSet::moveNext()
{
    assert(recordSet_);
    try {
        recordSet_->MoveNext();
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException(__FUNCTION__);
    }
}


void AdoRecordSet::nextRecordset()
{
    if (recordSet_ == 0) {
        throw AdoException(__FUNCTION__ " - Recordset is null");
    }

    try {
        if (recordSet_->GetState() != ADODB::adStateOpen) {
            throw AdoException(__FUNCTION__ " - Recordset is not opened");
        }

        _variant_t vtAffect = 0;
#pragma warning ( push )
#pragma warning ( disable: 4312 )
        recordSet_ = recordSet_->NextRecordset(&vtAffect);
#pragma warning ( pop )
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException(__FUNCTION__);
    }
}


void AdoRecordSet::getFieldValue(const std::string & field, int& value)
{
    assert(recordSet_);
    try {
        getInteger(recordSet_->Fields->GetItem(field.c_str())->Value,
            value);
    }
    catch (_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException();
    }
}


size_t AdoRecordSet::getRecordCount()
{
    try {
        ADODB::ADO_LONGPTR rows = recordSet_->GetRecordCount();
        if (rows == -1) {
            rows = 0;
            if (recordSet_->EndOfFile != VARIANT_TRUE) {
                recordSet_->MoveFirst();
            }

            while (recordSet_->EndOfFile != VARIANT_TRUE) {
                rows++;
                recordSet_->MoveNext();
            }

            if (rows > 0) {
                recordSet_->MoveFirst();
            }
        }
        return rows;
    }
    catch (_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
    }
    return 0;
}


void AdoRecordSet::releaseRecordSet()
{
    if (recordSet_) {
        recordSet_.Release();
    }
}

///////////////////////////////////////////////////////////////////////////////
// AdoCommand

AdoCommand::AdoCommand(AdoDatabase& database, const std::string & cmd) :
    database_(database),
    cmd_(cmd)
{
    try {
        const HRESULT hr = command_.CreateInstance(__uuidof(ADODB::Command));
        if (FAILED(hr)) {
            SNE_LOG_ERROR("AdoCommand::CreateInstance() Failed(%s)",
                _com_error(hr).ErrorMessage());
        }
        if (! cmd_.empty()) {
            command_->CommandText = _bstr_t(cmd_.c_str());
        }
        command_->CommandType = ADODB::adCmdStoredProc;
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException(getCommandName().c_str());
    }
}


void AdoCommand::setCommand(const std::string & cmd)
{
    assert(! cmd.empty());
    cmd_ = cmd;
    command_->CommandText = _bstr_t(cmd_.c_str());
}


ADODB::_RecordsetPtr AdoCommand::execute()
{
    assert(command_);
    try {
        command_->ActiveConnection = database_.getConnection();
        return command_->Execute(NULL, NULL, 0);
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException(getCommandName().c_str());
    }
}


void AdoCommand::executeNoRecords()
{
    assert(command_);
    try {
        command_->ActiveConnection = database_.getConnection();
        (void)command_->Execute(NULL, NULL, ADODB::adExecuteNoRecords);
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException(getCommandName().c_str());
    }
}


bool AdoCommand::addParameter(AdoParameter& param)
{
    try {
        command_->Parameters->Append(param.getParameter());
        return true;
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
// AdoParameter

AdoParameter::AdoParameter(AdoCommand& cmd,
    ADODB::DataTypeEnum type, ADODB::ParameterDirectionEnum direction,
    size_t dataSize) :
    database_(cmd.getDatabase())
{
    const HRESULT hr = parameter_.CreateInstance(__uuidof(ADODB::Parameter));
    SNE_ASSERT(SUCCEEDED(hr)); hr;
    parameter_->Direction = direction;
    parameter_->Type = type;
    parameter_->Size = static_cast<long>(dataSize);

    cmd.addParameter(*this);
}


void AdoParameter::setDataSize(size_t dataSize)
{
    parameter_->Size = static_cast<long>(dataSize);
}


void AdoDateParameter::setValue(time_t value)
{
    const _variant_t vtVal(core::ansiTimeToVariantTime(value));

    try {
        parameter_->Size = sizeof(DATE);
        parameter_->Value = vtVal;
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException();
    }
}


void AdoDateParameter::getValue(time_t& value)
{
    try {
        getInteger(parameter_->Value, value);
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
    }
}


void AdoVarCharParameter::setValue(const std::string & value)
{
    setValueWithString(database_, *parameter_, value);
}


void AdoVarCharParameter::getValue(std::string & value)
{
    value.clear();
    try {
        getString(parameter_->Value, value);
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException();
    }
}


void AdoVarWCharParameter::setValue(const std::wstring & value)
{
    setValueWithString(database_, *parameter_, value);
}


void AdoVarWCharParameter::getValue(std::wstring & value)
{
    value.clear();
    try {
        getString(parameter_->Value, value);
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException();
    }
}


void AdoTextParameter::setValue(const std::string & value)
{
    setValueWithString(database_, *parameter_, value);
}


void AdoTextParameter::getValue(std::string & value)
{
    value.clear();
    try {
        getString(parameter_->Value, value);
    }
    catch(_com_error& e) {
        database_.setLastError(e, __FUNCTION__, __LINE__);
        throw AdoException();
    }
}

}} // namespace sne { namespace database {

#endif // #ifdef _WIN32
