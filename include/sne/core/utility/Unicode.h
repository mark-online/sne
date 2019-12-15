#pragma once

#include <sne/Common.h>

namespace sne { namespace core {

/// convert UCS-2(4) to UTF-8    
std::string SNE_API toUtf8(const std::wstring& ucs);

/// convert UTF-8 to UCS-2(4)    
std::wstring SNE_API fromUtf8(const std::string& utf8);


std::wstring SNE_API toUnicode(const std::string& ansi,
    const std::locale& loc = std::locale()) /*throw std::runtime_error*/;
std::string SNE_API fromUnicode(const std::wstring& unicode,
    const std::locale& loc = std::locale()) /*throw std::runtime_error*/;

}} // namespace sne { namespace core
