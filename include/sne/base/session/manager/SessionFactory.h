#pragma once

#include <boost/noncopyable.hpp>

namespace sne { namespace base {

class Session;
class SessionDestroyer;

/**
 * @class SessionFactory
 *
 * Session Factory
 */
class SessionFactory : public boost::noncopyable
{
public:
    virtual ~SessionFactory() {}

    virtual void setSessionDestroyer(SessionDestroyer& sessionDestroyer) = 0;

    /// 새로운 세션을 생성한다.
    virtual std::unique_ptr<Session> create() const = 0;
};

}} // namespace sne { namespace base {
