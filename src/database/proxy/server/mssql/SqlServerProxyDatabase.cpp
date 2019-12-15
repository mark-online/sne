#include "DatabasePCH.h"
#include "SqlServerProxyDatabase.h"
#include <sne/database/ado/AdoDatabaseUtils.h>
#include <sne/server/config/ConfigReader.h>
#include <sne/server/utility/Profiler.h>
#include <sne/base/utility/Assert.h>
#include <sne/core/container/Containers.h>

namespace sne { namespace database { namespace proxy {

namespace
{

typedef std::shared_ptr<AdoParameter> AdoParameterPtr;


inline AdoParameterPtr createAdoParameter(AdoCommand& cmd,
    const Param& param)
{
    AdoParameterPtr adoParam;

    if (ptInt32 == param.type_) {
        adoParam.reset(new AdoIntegerParameter(cmd));
        static_cast<AdoIntegerParameter&>(*adoParam).
            setValue(param.int32Value_);
    }
    else if (ptUInt32 == param.type_) {
        adoParam.reset(new AdoUnsignedIntParameter(cmd));
        static_cast<AdoUnsignedIntParameter&>(*adoParam).
            setValue(param.uint32Value_);
    }
    else if (ptInt64 == param.type_) {
        adoParam.reset(new AdoBigIntParameter(cmd));
        static_cast<AdoBigIntParameter&>(*adoParam).
            setValue(param.int64Value_);
    }
    else if (ptUInt64 == param.type_) {
        adoParam.reset(new AdoBigIntParameter(cmd));
        static_cast<AdoBigIntParameter&>(*adoParam).
            setValue(param.uint64Value_);
        // MS SQL Server(2008)는 unsigned bigint를 지원하지 않는다
        //adoParam.reset(new AdoUnsignedBigIntParameter(cmd));
        //static_cast<AdoUnsignedBigint_tParameter&>(*adoParam).
        //    setValue(param.uint64Value_);
    }
    else if (ptString == param.type_) {
        adoParam.reset(new AdoVarCharParameter(cmd, param.stringValue_.length()));
        static_cast<AdoVarCharParameter&>(*adoParam).
            setValue(param.stringValue_);
    }
    else if (ptWString == param.type_) {
        adoParam.reset(new AdoVarWCharParameter(cmd, param.wstringValue_.length()));
        static_cast<AdoVarWCharParameter&>(*adoParam).
            setValue(param.wstringValue_);
    }
    else {
        SNE_ASSERT(false && "invalid param-type");
    }
    return adoParam;
}

} // namespace

// = SqlServerProxyDatabase

bool SqlServerProxyDatabase::open(server::ConfigReader& configReader)
{
    const std::string  connectionString =
        configReader.getString("database", "connection-string");
    const uint8_t connectionTimeout = static_cast<uint8_t>(
        configReader.getNumeric<size_t>("database", "connection-timeout"));
    if (! connection_.open(connectionString, connectionTimeout)) {
        SNE_LOG_ERROR(__FUNCTION__ " Failed(%s, %d)",
            connectionString.c_str(), connectionTimeout);
        return false;
    }
    return true;
}


void SqlServerProxyDatabase::close()
{
    connection_.close();
}

// TODO: ADO parameter 최적화(caching)
bool SqlServerProxyDatabase::call(RequestId requestId,
    const ModifyRequest& request)
{
    const std::string  profileName = (__FUNCTION__ ", ") + request.command_;
    server::Profiler profiler(profileName.c_str());

    SNE_LOG_INFO("SqlServerProxyDatabase::call(#%u)",
        requestId);

    if (! request.isValid()) {
        SNE_LOG_ERROR("SqlServerProxyDatabase::call(#%u) FAILED"
            "(invalid requestInfo)",
            requestId);
        return false;
    }

    if (request.type_ != rtStoredProcedure) {
        SNE_LOG_ERROR("SqlServerProxyDatabase::call(#%u) FAILED"
            "(only SP supported)",
            requestId);
        return false;
    }

    core::Vector<AdoParameterPtr> adoParams;
    adoParams.reserve(request.params_.size());

    AdoCommand cmd(getAdoDatabase(), request.command_);

    try {
        for (const Param& param : request.params_) {
            AdoParameterPtr adoParam = createAdoParameter(cmd, param);
            if (! adoParam.get()) {
                SNE_LOG_ERROR("SqlServerProxyDatabase::call(#%u) FAILED"
                    "(invalid param-type(%d)",
                    requestId, param.type_);
                return false;
            }
            adoParams.push_back(adoParam);
        }

        cmd.executeNoRecords();
    }
    catch (const AdoException& e) {
        logDatabaseError(getAdoDatabase(), e);
        return false;
    }

    return true;
}

}}} // namespace sne { namespace database { namespace proxy {
