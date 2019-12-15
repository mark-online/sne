#include "BasePCH.h"

#ifdef _WIN32

#include <sne/base/utility/StackTracer.h>
#include <sne/base/utility/Logger.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4996 4740)
#endif
#include "StackWalker.cpp"
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

#pragma comment (lib, "DbgHelp.lib")

namespace sne { namespace base {

namespace detail {

/**
 * @class StackTracerImpl
 */
class StackTracerImpl : public StackWalker
{
public:
    StackTracerImpl(HANDLE hFile) :
        hFile_(hFile) {}

private:
    virtual void OnOutput(LPCSTR szText) override {
        if (hFile_ != INVALID_HANDLE_VALUE) {
            DWORD cbWritten;
            WriteFile(hFile_, szText, lstrlenA(szText), &cbWritten, 0);
        }
        else {
            SNE_LOG_ERROR("\t%s", chomp(szText));
        }
    }

private:
    // StackWalker의 내부 구현이 바뀔 경우 아래 코드가 문제가 될 수 있다!!
    LPCSTR chomp(LPCSTR szText)
    {
        const size_t len = strlen(szText);
        if (szText[len-1] == '\n') {
            const_cast<LPSTR>(szText)[len-1] = '\0';
        }
        return szText;
    }

private:
    HANDLE hFile_;
};

} // namespace detail

// = StackTracer

StackTracer::StackTracer(HANDLE hFile) :
    impl_(std::make_unique<detail::StackTracerImpl>(hFile))
{
}


StackTracer::~StackTracer()
{
}


void StackTracer::trace(const CONTEXT* context)
{
    (void)impl_->ShowCallstack(GetCurrentThread(), context);
}

}} // namespace sne { namespace base {

#endif // #ifdef _WIN32
