// C related utilities

#pragma once

#include <cstdio>
#include <cstdarg>

namespace sne { namespace core {

// ACE/OS_NS_stdio.inl에서 코드를 도용!!
inline int vsnprintf(char *buffer, size_t maxlen, const char *format, va_list ap)
{
    int result;
#ifdef _WIN32
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4996)
#endif
    result = ::_vsnprintf(buffer, maxlen, format, ap);
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
    // Win32 doesn't regard a full buffer with no 0-terminate as an overrun.
    if (result == static_cast<int>(maxlen)) {
        buffer[maxlen - 1] = '\0';
    }

    // Win32 doesn't 0-terminate the string if it overruns maxlen.
    if (result == -1) {
        buffer[maxlen - 1] = '\0';
    }
#else
    result = ::vsnprintf(buffer, maxlen, format, ap);
#endif

    // In out-of-range conditions, C99 defines vsnprintf() to return the number
    // of characters that would have been written if enough space was available.
    // Earlier variants of the vsnprintf() (e.g. UNIX98) defined it to return
    // -1. This method follows the C99 standard, but needs to guess at the
    // value; uses maxlen + 1.
    if (result == -1) {
        result = static_cast <int>(maxlen + 1);
    }

    return result;
}


// ACE/OS_NS_stdio.inl에서 코드를 도용!!
inline int vsnprintf(wchar_t *buffer, size_t maxlen, const wchar_t *format, va_list ap)
{
    int result;
#ifdef _WIN32
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4996)
#endif
    // Microsoft's vswprintf() doesn't have the maxlen argument that
    // XPG4/UNIX98 define. They do, however, recommend use of _vsnwprintf()
    // as a substitute, which does have the same signature as the UNIX98 one.
    result = ::_vsnwprintf(buffer, maxlen, format, ap);
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
    // Win32 doesn't regard a full buffer with no 0-terminate as an overrun.
    if (result == static_cast<int>(maxlen)) {
        buffer[maxlen - 1] = '\0';
    }

    // Win32 doesn't 0-terminate the string if it overruns maxlen.
    if (result == -1) {
        buffer[maxlen - 1] = '\0';
    }
#else
    result = ::vswprintf(buffer, maxlen, format, ap);
#endif

    // In out-of-range conditions, C99 defines vsnprintf() to return the number
    // of characters that would have been written if enough space was available.
    // Earlier variants of the vsnprintf() (e.g. UNIX98) defined it to return
    // -1. This method follows the C99 standard, but needs to guess at the
    // value; uses maxlen + 1.
    if (result == -1) {
        result = static_cast <int>(maxlen + 1);
    }

    return result;
}

}} // namespace sne { namespace core {
