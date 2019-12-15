#pragma once

namespace sne { namespace server {

/// Client session 식별자
enum class ClientId : uint32_t
{
    invalid = 0
};


inline bool isValid(ClientId clientId)
{
    return clientId != ClientId::invalid;
}

}} // namespace sne { namespace server {
