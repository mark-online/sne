#include "CorePCH.h"

#ifdef _WIN32

#include <sne/core/utility/Win32Util.h>
#include <sne/core/container/Containers.h>

// You must include the #define _WIN32_DCOM preprocessor directive at the
// beginning of your code to be able to use CoInitializeEx.
#define _WIN32_DCOM
#include <objbase.h>

namespace sne { namespace core {

namespace {

typedef struct tagTHREADNAME_INFO
{
    DWORD dwType_; // must be 0x1000
    LPCSTR szName_; // pointer to name (in user addr space)
    DWORD dwThreadID_; // thread ID (-1=caller thread)
    DWORD dwFlags_; // reserved for future use, must be zero

    tagTHREADNAME_INFO(LPCSTR szName, DWORD dwThreadID) :
        dwType_(0x1000),
        szName_(szName),
        dwThreadID_(dwThreadID),
        dwFlags_(0) {}
} THREADNAME_INFO;

} // namespace

InitOle::InitOle()
{
    ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
}


InitOle::~InitOle()
{
    ::CoUninitialize();
}


time_t variantTimeToAnsiTime(const tagVARIANT& vt)
{
    assert(vt.vt == VT_DATE);
    if (vt.vt != VT_DATE) {
        return 0;
    }

    SYSTEMTIME sysTime;
    const int status = ::VariantTimeToSystemTime(vt.date, &sysTime);
    assert(status != 0); status;

    struct tm theTm;
    theTm.tm_sec = sysTime.wSecond;
    theTm.tm_min = sysTime.wMinute;
    theTm.tm_hour = sysTime.wHour;
    theTm.tm_mday = sysTime.wDay;
    theTm.tm_mon = sysTime.wMonth - 1;        // tm_mon is 0 based
    theTm.tm_year = sysTime.wYear - 1900;     // tm_year is 1900 based
    theTm.tm_isdst = -1;
    return mktime(&theTm);
}


tagVARIANT ansiTimeToVariantTime(time_t utc)
{
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4996)
#endif

    tm* localtm = localtime(&utc);
    assert(localtm != nullptr);
    if (! localtm) {
        const time_t curTime = time(0);
        localtm = localtime(&curTime);
        assert(localtm != nullptr);
    }

    SYSTEMTIME sysTime;
    sysTime.wYear = static_cast<WORD>(1900 + localtm->tm_year);
    sysTime.wMonth = static_cast<WORD>(localtm->tm_mon + 1);
    sysTime.wDayOfWeek = 0;
    sysTime.wDay = static_cast<WORD>(localtm->tm_mday);
    sysTime.wHour = static_cast<WORD>(localtm->tm_hour);
    sysTime.wMinute = static_cast<WORD>(localtm->tm_min);
    sysTime.wSecond = static_cast<WORD>(localtm->tm_sec);
    sysTime.wMilliseconds = 0;

    tagVARIANT vt;
    vt.vt = VT_DATE;
    const int status = ::SystemTimeToVariantTime(&sysTime, &vt.date);
    assert(status != 0); status;
    return vt;

#ifdef _MSC_VER
#  pragma warning (pop)
#endif
}


// http://earthfall.egloos.com/2280760
void setThreadName(const std::string& threadName)
{
    const DWORD MS_VC_EXCEPTION = 0x406D1388;

    if (threadName.empty()) {
        return;
    }

    if (! ::IsDebuggerPresent()) {
        return;
    }

    const THREADNAME_INFO info(threadName.c_str(), ::GetCurrentThreadId());

    __try {
        const DWORD numberOfArguments = sizeof(info)/sizeof(DWORD);
#ifdef _WIN64
        ::RaiseException(MS_VC_EXCEPTION, 0, numberOfArguments, (ULONG_PTR*)&info);
#else
        ::RaiseException(MS_VC_EXCEPTION, 0, numberOfArguments, (DWORD*)&info);
#endif
    }
    __except (EXCEPTION_CONTINUE_EXECUTION) {}
}


void enableLowFragmentationHeap()
{
    //const ULONG HEAP_STANDARD = 0;
    //const ULONG HEAP_LAL = 1;
    const ULONG HEAP_LFH = 2;

    const DWORD heapCount = ::GetProcessHeaps(0, 0);
    std::vector<HANDLE> handles(heapCount, NULL);
    ::GetProcessHeaps(heapCount, handles.data());

    ULONG heapFragValue = HEAP_LFH;
    for (size_t i = 0; i < heapCount; ++i) {
        ::HeapSetInformation(handles[i], HeapCompatibilityInformation,
            &heapFragValue, sizeof(heapFragValue));
    }
}

}} // namespace sne { namespace core {

#endif // #ifdef _WIN32
