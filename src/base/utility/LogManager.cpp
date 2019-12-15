#include "BasePCH.h"

#ifdef _MSC_VER
#  pragma warning (disable: 4100 4503 4714)
#endif

#include <sne/base/utility/LogManager.h>
#include <sne/base/utility/Logger.h>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/core/null_deleter.hpp>
#include <iostream>
#include <fstream>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace aux = boost::log::aux;

namespace {

inline const char* getSeverityLevelString(sne::base::SeverityLevel lvl)
{
    static const char* const strLevel[] = {
        "trace", "debug", "info", "warning", "error", "fatal"
    };
    if (static_cast<size_t>(lvl) < (sizeof(strLevel) / sizeof(*strLevel))) {
        return strLevel[lvl];
    }
    return strLevel[0];
}

template <typename CharT, typename TraitsT>
inline std::basic_ostream<CharT, TraitsT>& operator<<(
    std::basic_ostream<CharT, TraitsT>& strm, sne::base::SeverityLevel lvl)
{
    strm << getSeverityLevelString(lvl);
    return strm;
}

// inline logging::formatting_ostream& operator<<(
//     logging::formatting_ostream& strm,
//     const logging::to_log_manip<sne::base::SeverityLevel>& manip)
// {
//     const sne::base::SeverityLevel lvl = manip.get();
//     strm << getSeverityLevelString(lvl);
//     return strm;
// }

} // namespace {

namespace sne { namespace base {

namespace {

    void addCommonAttributes()
    {
        boost::shared_ptr<logging::core> pCore = logging::core::get();
        //pCore->add_global_attribute(
        //    aux::default_attribute_names::line_id(),
        //    attrs::counter< unsigned int >(1));
        pCore->add_global_attribute(
            aux::default_attribute_names::timestamp(),
            attrs::local_clock());
        //pCore->add_global_attribute(
        //    aux::default_attribute_names::process_id(),
        //    attrs::current_process_id());
#if !defined(BOOST_LOG_NO_THREADS)
        pCore->add_global_attribute(
            aux::default_attribute_names::thread_id(),
            attrs::current_thread_id());
#endif
    }


    template <typename BackEnd>
    void addLogSink(boost::shared_ptr<sinks::synchronous_sink<BackEnd>>& sink)
    {
        sink->set_formatter(
            expr::format("[%1%] %2% %3% - %4%")
            % expr::attr<SeverityLevel>("Severity")
            //% expr::attr<boost::posix_time::ptime>("TimeStamp")
            % expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
            % expr::attr<attrs::current_thread_id::value_type>("ThreadID")
            % expr::smessage);
        logging::core::get()->add_sink(sink);
    }

    SeverityLevel getServerityLevel(const std::string& strLevel) {
        if (strLevel == "trace") {
            return trace;
        }
        if (strLevel == "debug") {
            return debug;
        }
        if (strLevel == "info") {
            return info;
        }
        if (strLevel == "warning") {
            return warning;
        }
        if (strLevel == "error") {
            return error;
        }
        if (strLevel == "fatal") {
            return fatal;
        }
        return debug;
    }

} // namespace {

// = LogManager

LogManager::LogManager()
{
    addCommonAttributes();
    noDebug();
}


LogManager::~LogManager()
{
}


void LogManager::silent()
{
    detail::enableLogger(false);
}


void LogManager::unsilent()
{
    detail::enableLogger(true);
}


void LogManager::verbose()
{
    setSeverityLevel("trace");
}


void LogManager::noDebug()
{
    setSeverityLevel("info");
}


void LogManager::setSeverityLevel(const std::string& strLevel)
{
    SeverityLevel level = getServerityLevel(strLevel);
    logging::core::get()->set_filter(
        expr::attr<SeverityLevel>("Severity") >= level);
}


void LogManager::enableConsoleLog()
{
    boost::shared_ptr<sinks::synchronous_sink<sinks::text_ostream_backend>> sink(
        boost::make_shared<sinks::synchronous_sink<sinks::text_ostream_backend>>());
    sink->locked_backend()->add_stream(
        boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
    addLogSink(sink);
}


void LogManager::enableFileLog(const std::string& filePath, size_t rotationSize)
{
    // 매일 오전 6:00에 rotation
    boost::shared_ptr< sinks::text_file_backend > backend =
        boost::make_shared<sinks::text_file_backend>(
            keywords::file_name = filePath,
            keywords::rotation_size = rotationSize,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(6, 0, 0),
            keywords::auto_flush = true,
            keywords::open_mode = std::ios_base::app);

    // Wrap it into the frontend and register in the core.
    // The backend requires synchronization in the frontend.
    typedef sinks::synchronous_sink< sinks::text_file_backend > sink_t;
    boost::shared_ptr<sink_t> sink = boost::make_shared<sink_t>(backend);
    addLogSink(sink);
}

}} // namespace sne { namespace base {
