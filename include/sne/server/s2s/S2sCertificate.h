#pragma once

namespace sne { namespace server {

enum {
    maxS2sCertificateLength = 10 ///< maximum length of certificate
};

/// S2S 인증서
typedef std::string S2sCertificate;

}} // namespace sne { namespace server {
