#pragma once

#include <sne/Common.h>

namespace anyrpc {
class Value;
} // namespace anyrpc {

namespace sne { namespace server { namespace rcon {

/**
* @class Command
* 외부 명령 처리
*/
class Command
{
public:
    virtual ~Command() {}

    /// @param name 등록한 RPC 이름
    /// @param detail 명령 파라미터
    virtual std::string execute(const std::string& name,
        anyrpc::Value& param1, anyrpc::Value& param2,
        anyrpc::Value& param3) const = 0;
};

}}} // namespace sne { namespace server { namespace rcon {
