#pragma once

#include <sne/core/Exception.h>
#include <sne/core/utility/CppUtils.h>
#include <sne/Common.h>
#include <boost/noncopyable.hpp>
#ifdef _MSC_VER
#  pragma warning ( push )
#  pragma warning ( disable: 4127 4701 4702 4819 )
#endif
#include <boost/lexical_cast.hpp>
#ifdef _MSC_VER
#  pragma warning ( pop )
#endif

namespace sne { namespace server {

/**
 * @class NotDefinedConfigError
 * 컨픽이 정의되지 않을 경우 발생
 */
class NotDefinedConfigError : public core::Exception
{
public:
    NotDefinedConfigError(const char* file, int fileno,
        const char* section, const char* key) :
        core::Exception(file, fileno,
            core::formatString("Config(%s/%s) is not defined.",
                section, key).c_str()) {}
};


/**
 * @class ConfigReader
 *
 * 컨픽 파일 reader
 */
class SNE_API ConfigReader : public boost::noncopyable
{
public:
    virtual ~ConfigReader() {}

    /// 설정 파일을 파싱한다.
    virtual bool open(const std::string& filename) = 0;

    /**
     * 설정 파일의 sectionName 섹션의 keyName의 값을 읽어옵니다.
     * @exception NotDefinedConfigError 항목을 못 찾을 경우
     */
    virtual std::string getString(const char* sectionName,
        const char* keyName) = 0;

    /**
     * 설정 파일의 sectionName 섹션의 keyName의 값을 읽어옵니다.
     * @exception NotDefinedConfigError 항목을 못 찾을 경우
     */
    template <typename T>
    T getNumeric(const char* sectionName, const char* keyName) {
        std::string keyValue = getString(sectionName, keyName);
        try {
            return boost::lexical_cast<T>(keyValue);
        }
        catch(const boost::bad_lexical_cast &) {}

        throw NotDefinedConfigError(__FILE__, __LINE__, sectionName, keyName);
    }
};

}} // namespace sne { namespace server {
