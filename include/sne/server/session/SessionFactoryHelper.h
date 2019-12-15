#pragma once

#include <memory>

namespace sne { namespace base {
class SessionFactory;
class IoContextTask;
}} // namespace sne { namespace base {

namespace sne { namespace server {

struct ServerSpec;

/**
 * @class SessionFactoryHelper
 *
 * SessionFactory helper
 */
class SessionFactoryHelper : public boost::noncopyable
{
public:
    virtual ~SessionFactoryHelper() {}

    /**
    * SessionFactory를 얻는다.
    * @param serverSpec 상세 정보
    */
    virtual std::unique_ptr<base::SessionFactory> createSessionFactory(
        const ServerSpec& serverSpec, base::IoContextTask& ioContextTask) = 0;
};

}} // namespace sne { namespace server {
