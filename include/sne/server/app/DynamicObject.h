#pragma once

#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

namespace sne { namespace server {

/**
 * @class DynamicObject
 *
 * 동적으로 로딩하기 위한 베이스 클래스
 */
class DynamicObject
{
public:
    virtual ~DynamicObject() {}

public:
    virtual bool initialize(const std::vector<std::string>& args) = 0;
    virtual void finalize() = 0;

    bool initialize(const std::string& args) {
        std::vector<std::string> splited;
        boost::split(splited, args, [](char c) { return c == ' '; });
        return initialize(splited);
    }
};

}} // namespace sne { namespace server {
