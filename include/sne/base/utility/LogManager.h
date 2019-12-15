#pragma once

#include <sne/Common.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace base {

/** @addtogroup utility
* @{
*/

/**
* @class LogManager
* BOOST Log Manager
* - 기본적으로 Log는 비활성화 상태임.
*/
class SNE_API LogManager : public boost::noncopyable
{
public:
    LogManager();
    ~LogManager();

    /// disable the log
    void silent();

    /// enable the log
    void unsilent();

    /// log all message
    void verbose();

    /// log all message except debug message
    void noDebug();

    void setSeverityLevel(const std::string& strLevel);

public:
    void enableConsoleLog();
    void enableFileLog(const std::string& filePath, size_t rotationSize);
};

/** @} */ // addtogroup utility

}} /// namespace sne { namespace base {
