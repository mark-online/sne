#pragma once

#include <boost/noncopyable.hpp>

namespace sne { namespace client {

/**
 * @class ClientSessionCallback
 * SNE Client Session Event Handler
 */
class ClientSessionCallback : public boost::noncopyable
{
public:
    virtual ~ClientSessionCallback() {}

    /// 서버와의 연결이 유효하다(이후에 서버로 요청을 보낼 수 있다)
    /// ClientSession::connect()의 waitUntilValidate에 false를 넘길 경우
    /// 이 이벤트를 사용해야 한다.
    virtual void onValidated() {}

    /// 서버로 부터 접속이 해제되었다
    virtual void onDisconnected() = 0;
};

}} // namespace sne { namespace client {
