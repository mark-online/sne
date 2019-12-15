#pragma once

#include "../s2s/S2sCertificate.h"
#include <boost/noncopyable.hpp>

namespace sne { namespace server {

/**
 * @class ClientSideBridgeCallback
 */
class ClientSideBridgeCallback : public boost::noncopyable
{
public:
    virtual ~ClientSideBridgeCallback() {}

    /// 상대 서버와의 접속이 끊겼다
    virtual void disconnected() = 0;

    /// 인증 요청에 대한 응답
    virtual void onAuthenticate(bool authenticated) = 0;
};

}} // namespace sne { namespace server {
