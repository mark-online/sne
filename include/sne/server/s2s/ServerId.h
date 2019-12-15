#pragma once

namespace sne { namespace server {

/// Server Id
enum class ServerId : uint32_t
{
    invalid = 0
};

inline bool isValid(ServerId serverId)
{
    return serverId != ServerId::invalid;
}

}} // namespace sne { namespace server {
