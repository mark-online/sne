#pragma once

#ifdef _WIN32

#include <sne/base/utility/Assert.h>
#include <sne/core/utility/Win32Util.h>
#include <sne/core/memory/MemoryPoolMixin.h>
#include <sne/Common.h>
#include <boost/type_traits/is_enum.hpp>
#include <boost/noncopyable.hpp>
#include <stdexcept>
#include <cmath>

/// int64_t를 지원하는 Windows XP 이상에서만 제대로 동작한다
#ifdef _WIN32
#  undef _WIN32_WINNT
#  define _WIN32_WINNT 0x0501
#endif

/// workaround for handle_t name conflict
#define handle_t RPC_BINDING_HANDLE

#ifdef _MSC_VER // doxygen이 올바르게 파싱하도록 추가
// http://kaistizen.net/EE/index.php/weblog/comments/unhappy_when_importing_x64_ado/
#   ifdef _WIN64
// Windows 10(https://docs.microsoft.com/ko-kr/sql/ado/guide/referencing-the-ado-libraries-in-a-visual-c-application)
#       import "C:\Program Files\Common Files\system\ado\msado15.dll" rename("EOF", "EndOfFile")
// http://support.microsoft.com/kb/2517589 (하위 호환 문제 해결)
//#       import "C:\Program Files\Common Files\System\ado\msado60_Backcompat.tlb" rename("EOF","EndOfFile")
//#       import "C:\Program Files\Common Files\System\ado\msado15.dll" rename("EOF", "EndOfFile")
#   else
#       import "libid:EF53050B-882E-4776-B643-EDA472E8E3F2" rename("EOF", "EndOfFile")
#   endif
#endif

namespace sne { namespace database {

class AdoCommand;
class AdoParameter;

/**
* @class AdoException
* ADO exception
*/
class AdoException : public std::runtime_error
{
public:
    AdoException(const char* message = "AdoException") :
        std::runtime_error(message) {}
};


namespace
{

template <typename To, typename From>
inline To variant_cast_detail(From f, const boost::false_type&)
{
    return static_cast<To>(f);
}


template <typename To, typename From>
inline To variant_cast_detail(From f, const boost::true_type&)
{
    return static_cast<To>(static_cast<int>(f));
}


template <typename To, typename From>
inline To variant_cast(From f)
{
    typedef boost::integral_constant<bool,
        ::boost::is_enum<To>::value> truth_type;
    return variant_cast_detail<To>(f, truth_type());
}


template <typename T>
inline void getInteger(const tagVARIANT& variant, T& value)
{
    switch (variant.vt) {
    case VT_I1:
        value = variant_cast<T>(variant.cVal);
        break;
    case VT_UI1:
        value = variant_cast<T>(variant.bVal);
        break;
    case VT_I2:
        value = variant_cast<T>(variant.iVal);
        break;
    case VT_UI2:
        value = variant_cast<T>(variant.uiVal);
        break;
    case VT_I4:
        value = variant_cast<T>(variant.lVal);
        break;
    case VT_UI4:
        value = variant_cast<T>(variant.ulVal);
        break;
    case VT_INT:
        value = variant_cast<T>(variant.intVal);
        break;
    case VT_UINT:
        value = variant_cast<T>(variant.uintVal);
        break;
    case VT_R4:
        value = variant_cast<T>(variant.fltVal);
        break;
    case VT_R8:
        value = variant_cast<T>(variant.dblVal);
        break;
    case VT_DECIMAL:
        value = variant_cast<T>(variant.decVal.Lo64);
        if (variant.decVal.sign == DECIMAL_NEG) {
            value = variant_cast<T>(value * -1);
        }
        if (variant.decVal.scale > 0) {
            value = variant_cast<T>(value / pow(10.0, variant.decVal.scale));
        }
        break;
    case VT_BOOL:
        value = variant_cast<T>(variant.boolVal == 0 ? 0 : 1);
        break;
    case VT_DATE:
        value = static_cast<T>(core::variantTimeToAnsiTime(variant));
        break;
    case VT_NULL:
        value = variant_cast<T>(0);
        break;
    default:
        throw AdoException("unsupported variant type");
    }
}


inline void getInteger(const tagVARIANT& variant, bool& value)
{
    int intValue;
    getInteger<int>(variant, intValue);
    value = (intValue != 0);
}


inline void getInteger(const tagVARIANT& variant, int64_t& value)
{
    switch (variant.vt) {
    case VT_I8:
    case VT_UI8:
    case VT_DECIMAL:
        value = variant.llVal;
        break;
    case VT_R4:
        value = static_cast<int64_t>(variant.fltVal);
        break;
    case VT_R8:
        value = static_cast<int64_t>(variant.dblVal);
        break;
    case VT_BOOL:
        value = (variant.boolVal == 0 ? 0 : 1);
        break;
    case VT_DATE:
        value = static_cast<int64_t>(core::variantTimeToAnsiTime(variant));
        break;
    default:
        int32_t value32;
        getInteger<int32_t>(variant, value32);
        value = value32;
        break;
    }
}


inline void getInteger(const tagVARIANT& variant, uint64_t& value)
{
    switch (variant.vt) {
    case VT_I8:
    case VT_UI8:
    case VT_DECIMAL:
        value = variant.ullVal;
        break;
    case VT_R4:
        value = static_cast<int64_t>(variant.fltVal);
        break;
    case VT_R8:
        value = static_cast<uint64_t>(variant.dblVal);
        break;
    case VT_BOOL:
        value = (variant.boolVal == 0 ? 0 : 1);
        break;
    default:
        uint32_t value32;
        getInteger<uint32_t>(variant, value32);
        value = value32;
        break;
    }
}


/// float32_t는 지원하지 않는다!!!
inline void getFloat(const tagVARIANT& /*variant*/, float32_t& /*value*/)
{
    SNE_ASSERT(false &&
        "float32_t is not supported, use float64_t instead");
    throw AdoException(
        "float32_t is not supported, use float64_t instead");
}


inline void getFloat(const tagVARIANT& variant, float64_t& value)
{
    switch (variant.vt) {
    case VT_R4:
        value = variant.fltVal;
        break;
    case VT_R8:
        value = variant.dblVal;
        break;
    case VT_DECIMAL:
        value = static_cast<float64_t>(variant.decVal.Lo64);
        value *= (variant.decVal.sign == DECIMAL_NEG) ? -1 : 1;
        if (variant.decVal.scale > 0) {
            value /= pow(10.0, variant.decVal.scale);
        }
        break;
    case VT_NULL:
        value = 0.0;
        break;
    default:
        int64_t value64;
        getInteger<int64_t>(variant, value64);
        value = static_cast<float64_t>(value64);
        break;
    }
}


inline void getString(const tagVARIANT& variant, std::string & value)
{
    if (VT_BSTR == variant.vt) {
        value = static_cast<const char*>(bstr_t(variant.bstrVal));
    }
    else if (VT_NULL == variant.vt) {
        value.clear();
    }
    else {
        throw AdoException("unsupported variant type");
    }
}


inline void getString(const tagVARIANT& variant, std::wstring & value)
{
    if (VT_BSTR == variant.vt) {
        value = static_cast<const wchar_t*>(bstr_t(variant.bstrVal));
    }
    else if (VT_NULL == variant.vt) {
        value.clear();
    }
    else {
        throw AdoException("unsupported variant type");
    }
}

} // namespace


/**
* @class AdoDatabase
*
* ADO(ActiveX Data Objects) wrapper.
*/
class SNE_API AdoDatabase : public boost::noncopyable
{
    enum { MAX_ERROR_LEN = 1024 };
public:
    AdoDatabase();
    ~AdoDatabase();

    /**
     * 데이터베이스에 연결한다.
     * @param connectionString ADO Connection Strings
     *        (http://www.codeproject.com/database/connectionstrings.asp) 참고
     * @param connectionTimeout 연결 시간 제한(초 단위)
     */
    bool open(const std::string & connectionString,
        long connectionTimeout = 4);
    void close();

    void execute(const std::string & sql) /* throw (AdoException) */;

    void setLastError(_com_error& e, const char* function, int line);

public:
    bool isOpened();

    int getAffectedRecordsCount() const {
        return affectedRecordsCount_;
    }

    const char* getLastError() const {
        return lastError_;
    }

    long getLastResult() const {
        return lastResult_;
    }

    ADODB::_ConnectionPtr getConnection() const {
        return connection_;
    }

public:
    void ping();

private:
    bool initAdo();

private:
    long lastResult_;
    char lastError_[MAX_ERROR_LEN];

    std::string  lastConnectionString_;
    ADODB::_ConnectionPtr connection_;

    int affectedRecordsCount_;
};


/**
 * @class AdoRecordSet
 * ADO RecordSet wrapper
 */
class SNE_API AdoRecordSet : public boost::noncopyable
{
public:
    explicit AdoRecordSet(AdoDatabase& db);
    ~AdoRecordSet();

    bool open(const std::string & query);
    void close();

    void execute(AdoCommand& cmd) /* throw (AdoException) */;

    bool isEof();
    void moveNext() /* throw (AdoException) */;

    /// compound query recordSet에서 다음 recordset으로 이동한다
    void nextRecordset() /* throw (AdoException) */;

    bool isOpened();

    size_t getRecordCount();

    template <typename T>
    void getFieldValue(short index, T& value) /* throw (AdoException) */ {
        assert(recordSet_);
        try {
            getInteger(
                recordSet_->Fields->GetItem(_variant_t(index))->Value, value);
        }
        catch (_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    template <typename T>
    void getFieldEnumValue(short index, T& value) /* throw (AdoException) */ {
        uint64_t aValue;
        getFieldValue(index, aValue);
        value = static_cast<T>(aValue);
    }

    void getFieldValue(short index, float64_t& value) /* throw (AdoException) */ {
        assert(recordSet_);
        try {
            getFloat(
                recordSet_->Fields->GetItem(_variant_t(index))->Value, value);
        }
        catch (_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    void getFieldValue(short index, std::string & value) /* throw (AdoException) */ {
        assert(recordSet_);
        try {
            getString(
                recordSet_->Fields->GetItem(_variant_t(index))->Value, value);
        }
        catch (_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    void getFieldValue(short index, std::wstring & value) /* throw (AdoException) */ {
        assert(recordSet_);
        try {
            getString(
                recordSet_->Fields->GetItem(_variant_t(index))->Value, value);
        }
        catch (_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    void getFieldValue(const std::string & field, int& value) /* throw (AdoException) */;

    void getFieldValue(const std::string & field,
        std::string & value) /* throw (AdoException) */ {
        assert(recordSet_);
        value.clear();
        try {
            getString(recordSet_->Fields->GetItem(field.c_str())->Value, value);
        }
        catch(_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    void getFieldValue(const std::string & field,
        std::wstring & value) /* throw (AdoException) */ {
        assert(recordSet_);
        value.clear();
        try {
            getString(recordSet_->Fields->GetItem(field.c_str())->Value, value);
        }
        catch(_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    template <typename T>
    void getColumnValue(const std::string & column, T& value) /* throw (AdoException) */ {
        assert(recordSet_);
        try {
            getInteger(recordSet_->GetCollect(column.c_str()), value);
        }
        catch (_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    void getColumnValue(const std::string & column, float64_t& value) /* throw (AdoException) */ {
        assert(recordSet_);
        try {
            getFloat(recordSet_->GetCollect(column.c_str()), value);
        }
        catch (_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    void getColumnValue(const std::string & column,
        std::string & value) /* throw (AdoException) */ {
        assert(recordSet_);
        try {
            getString(recordSet_->GetCollect(column.c_str()), value);
        }
        catch (_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    void getColumnValue(const std::string & column,
        std::wstring & value) /* throw (AdoException) */ {
        assert(recordSet_);
        try {
            getString(recordSet_->GetCollect(column.c_str()), value);
        }
        catch (_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

private:
    void releaseRecordSet();

private:
    AdoDatabase& database_;
    ADODB::_RecordsetPtr recordSet_;
};


/**
 * @class AdoCommand
 * ADO Command wrapper
 */
class SNE_API AdoCommand : public boost::noncopyable
{
public:
    AdoCommand(AdoDatabase& database, const std::string & cmd);

    void setCommand(const std::string & cmd);

    ADODB::_RecordsetPtr execute() /* throw (AdoException) */;
    void executeNoRecords() /* throw (AdoException) */;

    bool addParameter(AdoParameter& param);

    const std::string & getCommandName() const {
        return cmd_;
    }

public:
    AdoDatabase& getDatabase() const {
        return database_;
    }
private:
    AdoDatabase& database_;
    ADODB::_CommandPtr command_;
    std::string  cmd_;
};


/**
 * @class AdoParameter
 * ADO Parameter wrapper
 */
class SNE_API AdoParameter : public boost::noncopyable
{
public:
    AdoParameter(AdoCommand& cmd,
        ADODB::DataTypeEnum type, ADODB::ParameterDirectionEnum direction,
        size_t dataSize);

    void setDataSize(size_t dataSize);

private:
    friend class AdoCommand;
    ADODB::_ParameterPtr getParameter() const {
        return parameter_;
    }

protected:
    AdoDatabase& database_;
    ADODB::_ParameterPtr parameter_;
};


/**
 * @class AdoIntegerParameter
 *
 * int_teger 타입을 위한 ADO Parameter
 */
template <typename BaseType, ADODB::DataTypeEnum AdoType>
class AdoNumericParameter : public AdoParameter,
    public core::ThreadSafeMemoryPoolMixin<AdoNumericParameter<BaseType, AdoType> >
{
public:
    AdoNumericParameter(AdoCommand& cmd,
        ADODB::ParameterDirectionEnum direction = ADODB::adParamInput) :
        AdoParameter(cmd, AdoType, direction, sizeof(BaseType)) {}

    template <typename T>
    void setValue(T value) {
        const _variant_t vtVal(static_cast<BaseType>(value));

        try {
            parameter_->Size = sizeof(BaseType);
            parameter_->Value = vtVal;
        }
        catch(_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    template <typename T>
    void getValue(T& value) {
        try {
            getInteger(parameter_->Value, value);
        }
        catch(_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }

    void getValue(float64_t& value) {
        try {
            getFloat(parameter_->Value, value);
        }
        catch(_com_error& e) {
            database_.setLastError(e, __FUNCTION__, __LINE__);
            throw AdoException();
        }
    }
};


/**
* @class AdoDateParameter
*
* Date 타입을 위한 ADO Parameter
*/
class SNE_API AdoDateParameter : public AdoParameter,
    public core::ThreadSafeMemoryPoolMixin<AdoDateParameter>
{
public:
    AdoDateParameter(AdoCommand& cmd,
        ADODB::ParameterDirectionEnum direction = ADODB::adParamInput) :
        AdoParameter(cmd, ADODB::adDate, direction, sizeof(DATE)) {}

    void setValue(time_t value);

    void getValue(time_t& value);
};


/**
* @class AdoVarCharParameter
*
* VarChar 타입을 위한 ADO Parameter
*/
class SNE_API AdoVarCharParameter : public AdoParameter,
    public core::ThreadSafeMemoryPoolMixin<AdoVarCharParameter>
{
public:
    AdoVarCharParameter(AdoCommand& cmd,
        size_t stringLength,
        ADODB::ParameterDirectionEnum direction = ADODB::adParamInput) :
        AdoParameter(cmd, ADODB::adVarChar, direction, stringLength) {}

    void setValue(const std::string & value);

    void getValue(std::string & value);
};


/**
* @class AdoWVarCharParameter
*
* 유니코드 문자열(NVARCHAR) 타입을 위한 ADO Parameter
*/
class SNE_API AdoVarWCharParameter : public AdoParameter,
    public core::ThreadSafeMemoryPoolMixin<AdoVarWCharParameter>
{
public:
    AdoVarWCharParameter(AdoCommand& cmd,
        size_t stringLength,
        ADODB::ParameterDirectionEnum direction = ADODB::adParamInput) :
        AdoParameter(cmd, ADODB::adVarWChar, direction, stringLength) {}

    void setValue(const std::wstring & value);

    void getValue(std::wstring & value);
};


/**
* @class AdoTextParameter
*
* Text 타입을 위한 ADO Parameter
*/
class SNE_API AdoTextParameter : public AdoParameter,
    public core::ThreadSafeMemoryPoolMixin<AdoTextParameter>
{
    enum { maxLength = 2147483647 };
public:
    AdoTextParameter(AdoCommand& cmd,
        size_t stringLength = maxLength,
        ADODB::ParameterDirectionEnum direction = ADODB::adParamInput) :
        AdoParameter(cmd, ADODB::adLongVarChar, direction, stringLength) {}

    void setValue(const std::string & value);

    void getValue(std::string & value);
};


// http://www.w3schools.com/ADO/ado_datatypes.asp 참고
// - Oracle에서는 adNumeric(VT_DECIMAL)을 이용하는 것이 더 깔끔하다

typedef AdoNumericParameter<int8_t, ADODB::adTinyInt>
    AdoTinyIntParameter;
typedef AdoNumericParameter<int16_t, ADODB::adSmallInt>
    AdoSmallIntParameter;
typedef AdoNumericParameter<int32_t, ADODB::adInteger>
    AdoIntegerParameter;
typedef AdoNumericParameter<int64_t, ADODB::adBigInt>
    AdoBigIntParameter;

typedef AdoNumericParameter<uint8_t, ADODB::adUnsignedTinyInt>
    AdoUnsignedTinyIntParameter;
typedef AdoNumericParameter<uint16_t, ADODB::adUnsignedSmallInt>
    AdoUnsignedSmallIntParameter;
typedef AdoNumericParameter<uint32_t, ADODB::adUnsignedInt>
    AdoUnsignedIntParameter;
typedef AdoNumericParameter<uint64_t, ADODB::adUnsignedBigInt>
    AdoUnsignedBigIntParameter;

typedef AdoNumericParameter<int8_t, ADODB::adNumeric>
    AdoNumeric8Parameter;
typedef AdoNumericParameter<int16_t, ADODB::adNumeric>
    AdoNumeric16Parameter;
typedef AdoNumericParameter<int32_t, ADODB::adNumeric>
    AdoNumeric32Parameter;
typedef AdoNumericParameter<int64_t, ADODB::adNumeric>
    AdoNumeric64Parameter;

typedef AdoNumericParameter<uint8_t, ADODB::adNumeric>
    AdoNumericU8Parameter;
typedef AdoNumericParameter<uint16_t, ADODB::adNumeric>
    AdoNumericU16Parameter;
typedef AdoNumericParameter<uint32_t, ADODB::adNumeric>
    AdoNumericU32Parameter;
typedef AdoNumericParameter<uint64_t, ADODB::adNumeric>
    AdoNumericU64Parameter;

typedef AdoNumericParameter<float32_t, ADODB::adSingle>
    AdoFloat32Parameter;
typedef AdoNumericParameter<float64_t, ADODB::adDouble>
    AdoFloat64Parameter;

}} // namespace sne { namespace database {

#endif // #ifdef _WIN32

