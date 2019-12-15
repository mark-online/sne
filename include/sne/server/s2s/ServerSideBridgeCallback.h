#pragma once

#include "../s2s/S2sCertificate.h"

namespace sne { namespace server {

/**
 * @class ServerSideBridgeCallback
 */
class ServerSideBridgeCallback
{
public:
    virtual ~ServerSideBridgeCallback() {}

    /// 클라이언트가 접속하였다
    virtual void connected() = 0;

    /// 클라이언트가 접속을 해제하였다
    virtual void disconnected() = 0;

    /// 세션이 파괴되었다
    virtual void destroyed() = 0;

    /// 클라이언트가 인증을 요청하였다
    virtual void authenticate(const S2sCertificate& certificate) = 0;
};

}} // namespace sne { namespace server {
