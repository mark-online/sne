#pragma once

#ifdef _WIN32

#include <sne/Common.h>
#include <boost/noncopyable.hpp>
#include <ctime>

struct tagVARIANT;

namespace sne { namespace core {

/**
 * COM library를 초기화한다.
 * - from MSDN
 */
struct SNE_API InitOle : public boost::noncopyable
{
    InitOle();
    ~InitOle();
};


/// VARIANT time을 ANSI time으로 변경한다
time_t SNE_API variantTimeToAnsiTime(const tagVARIANT& vt);

/// ANSI time을 VARIANT time으로 변경한다
tagVARIANT SNE_API ansiTimeToVariantTime(time_t utc);

/// 디버깅이 용이하도록 현재 쓰레드에 이름을 설정한다
void SNE_API setThreadName(const std::string& threadName);

/// LFH(Low Fragmentation Heap)을 활성화시킨다
void SNE_API enableLowFragmentationHeap();

}} // namespace sne { namespace core {

#endif // _WIN32
