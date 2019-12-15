#pragma once

#include <sne/Common.h>
#include <boost/noncopyable.hpp>

#ifdef _WIN32

namespace sne { namespace base {

namespace detail {
class StackTracerImpl;
} // namespace detail

/**
 * @class StackTracer
 * 콜 스택 정보를 SNE_LOG_ERROR()를 이용하여 기록한다
 * - PIMPL idiom
 */
class SNE_API StackTracer : public boost::noncopyable
{
public:
    StackTracer(HANDLE hFile);
    ~StackTracer();

    void trace(const CONTEXT* context = NULL);

private:
    std::unique_ptr<detail::StackTracerImpl> impl_;
};

}} // namespace sne { namespace base {

#endif // #ifdef _WIN32
