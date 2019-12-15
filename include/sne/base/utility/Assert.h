#pragma once

#include <sne/Common.h>
#include <boost/current_function.hpp>

namespace sne { namespace base {

/// assertion이 발생했을 때 호출할 핸들러 타입
using AssertHandler = void (*)();

/// assertion이 발생했을 때 호출할 핸들러를 설정한다 (not thread-safe)
void SNE_API setAssertHandler(AssertHandler newHandler);

namespace detail
{

/// don't call me!
void SNE_API asserted(const char* what, const char* exp, const char* func,
    const char* filename, unsigned int line_no);

} // namespace detail

}} // namespace sne { namespace base {

#define SNE_ASSERT(exp) (void)((exp) || \
    (sne::base::detail::asserted("ASSERT", #exp, BOOST_CURRENT_FUNCTION, \
        __FILE__, __LINE__), 0) )

#define SNE_PRECONDITION(exp) (void)((exp) || \
    (sne::base::::detail::asserted("PRECONDITION", #exp, BOOST_CURRENT_FUNCTION, \
        __FILE__, __LINE__), 0) )

#define SNE_POSTCONDITION(exp) (void)((exp) || \
    (sne::base::::detail::asserted("POSTCONDITION", #exp, BOOST_CURRENT_FUNCTION, \
        __FILE__, __LINE__), 0) )
