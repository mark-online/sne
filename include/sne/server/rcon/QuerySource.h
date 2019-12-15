#pragma once

#include <sne/Common.h>

namespace sne { namespace server { namespace rcon {

/**
 * @class QuerySource
 *
 * Montior QuerySource
 */
class SNE_API QuerySource
{
public:
    virtual ~QuerySource() {}

    /// @param name 등록한 RPC 이름
    /// @param detail 검색할 세부 사항
    virtual std::string query(const std::string& name,
        const std::string& detail) const = 0;
};

}}} // namespace sne { namespace server { namespace rcon {
