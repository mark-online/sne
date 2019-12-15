#pragma once

#ifdef _WIN32

#include <sne/Common.h>
#include <boost/noncopyable.hpp>
#include <ctime>

struct tagVARIANT;

namespace sne { namespace core {

/**
 * COM library�� �ʱ�ȭ�Ѵ�.
 * - from MSDN
 */
struct SNE_API InitOle : public boost::noncopyable
{
    InitOle();
    ~InitOle();
};


/// VARIANT time�� ANSI time���� �����Ѵ�
time_t SNE_API variantTimeToAnsiTime(const tagVARIANT& vt);

/// ANSI time�� VARIANT time���� �����Ѵ�
tagVARIANT SNE_API ansiTimeToVariantTime(time_t utc);

/// ������� �����ϵ��� ���� �����忡 �̸��� �����Ѵ�
void SNE_API setThreadName(const std::string& threadName);

/// LFH(Low Fragmentation Heap)�� Ȱ��ȭ��Ų��
void SNE_API enableLowFragmentationHeap();

}} // namespace sne { namespace core {

#endif // _WIN32
