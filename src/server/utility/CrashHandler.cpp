#include "ServerPCH.h"

#ifdef _WIN32

#pragma warning(disable:4091)

#include <sne/server/utility/CrashHandler.h>
#include <sne/base/utility/Logger.h>
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/StackTracer.h>
#include <DbgHelp.h>
#include <tchar.h>

#pragma comment (lib, "DbgHelp.lib")

namespace sne { namespace server {

namespace
{

char s_dumpDir[_MAX_PATH];
char s_dumpFilenamePrefix[_MAX_PATH];
char s_basename[_MAX_PATH];

// The type for the filter function called by the Crash Handler API.
typedef LONG (__stdcall *PFNCHFILTFN)(EXCEPTION_POINTERS* pep);

// The custom unhandled exception filter (crash handler)
PFNCHFILTFN realExceptionFilter = NULL;

// The original unhandled exception filter
LPTOP_LEVEL_EXCEPTION_FILTER originalExceptionFilter = NULL;


inline void crashNow()
{
    ::RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);
}


inline void setBaseFilename()
{
    sprintf(s_basename, "%s\\%s_%u_%u_%u",
        s_dumpDir,
        s_dumpFilenamePrefix,
        static_cast<int>(time(0)),
        ::GetCurrentProcessId(),
        ::GetCurrentThreadId());
}


inline const char* makeDumpFilename(char* filename)
{
    strcpy(filename, s_basename);
    strcat(filename, ".DMP");
    return filename;
}


inline const char* makeLogFilename(char* filename)
{
    strcpy(filename, s_basename);
    strcat(filename, ".TXT");
    return filename;
}


BOOL CALLBACK miniDumpCallback(PVOID /*pParam*/,
    const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput) 
{
    // Check parameters 
    if ((! pInput) || (! pOutput)) {
        return FALSE;
    }

    // Process the callbacks
    switch (pInput->CallbackType) {
    case IncludeModuleCallback: // Include the module into the dump
    case IncludeThreadCallback: // Include the thread into the dump
    case ThreadCallback: // Include all thread information into the dump
    case ThreadExCallback: // Include this information
        return TRUE;
    case ModuleCallback: // Are data sections available for this module?
        return TRUE;
    //case MemoryCallback:
    //case CancelCallback:
    }

    return FALSE;
}


void writeReport(EXCEPTION_POINTERS* pExPtrs)
{
    char logFilename[_MAX_PATH];
    const HANDLE hFile =
        ::CreateFileA(makeLogFilename(logFilename), GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
    if (hFile != INVALID_HANDLE_VALUE) {
        base::StackTracer stackTracer(hFile);
        stackTracer.trace(pExPtrs->ContextRecord);
        (void)::CloseHandle(hFile);
    }
}


// http://www.debuginfo.com/articles/effminidumps2.html#mididump
void writeMiniDump(EXCEPTION_POINTERS* pep)
{
    const MINIDUMP_TYPE dumpType = static_cast<MINIDUMP_TYPE>(
        MiniDumpWithPrivateReadWriteMemory | MiniDumpWithDataSegs |
        MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo |
        MiniDumpWithThreadInfo | MiniDumpWithUnloadedModules);

    if (! pep) {
        // Generate exception to get proper context in dump
        __try {
            ::RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);
        } 
        __except (writeMiniDump(GetExceptionInformation()),
            EXCEPTION_CONTINUE_EXECUTION) {}

        return;
    }
    else {
        char dumpFilename[_MAX_PATH];
        const HANDLE hFile =
            ::CreateFileA(makeDumpFilename(dumpFilename), GENERIC_WRITE, 0, NULL,
                CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            MINIDUMP_EXCEPTION_INFORMATION mdei;
            memset(&mdei, 0x00, sizeof(mdei));
            mdei.ThreadId = ::GetCurrentThreadId();
            mdei.ExceptionPointers = pep;
            mdei.ClientPointers = TRUE;

            MINIDUMP_CALLBACK_INFORMATION mci;
            mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)miniDumpCallback;
            mci.CallbackParam = NULL;

            const BOOL succeeded =
                ::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(),
                    hFile, dumpType, &mdei, NULL, &mci);
            (void)::CloseHandle(hFile);

            if (succeeded) {
                SNE_LOG_ERROR("Core Dumped(%s)!!!", dumpFilename);
                return;
            }
        }

        SNE_LOG_ERROR("Core Dump Failed(%s)!!!", dumpFilename);
    }
}


LONG __stdcall crashHandlerExceptionFilter(EXCEPTION_POINTERS* pExPtrs)
{
    LONG lRet = EXCEPTION_CONTINUE_SEARCH;

    __try {
        if (NULL != realExceptionFilter) {
            lRet = realExceptionFilter(pExPtrs);
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        lRet = EXCEPTION_CONTINUE_SEARCH;
    }
    return lRet;
}


bool setCrashHandlerFilter(PFNCHFILTFN pFn)
{
    assert(pFn != NULL);
    assert(! originalExceptionFilter);

    realExceptionFilter = pFn;

    // If a custom crash handler isn't already in use, enable
    // CrashHandlerExceptionFilter and save the original unhandled
    // exception filter.
    if (! originalExceptionFilter) {
        originalExceptionFilter =
            ::SetUnhandledExceptionFilter(crashHandlerExceptionFilter);
    }
    return true;
}


DWORD WINAPI dumpThread(LPVOID param)
{
    void writeMiniDump(EXCEPTION_POINTERS* pExPtrs);

    EXCEPTION_POINTERS* pExPtrs = reinterpret_cast<EXCEPTION_POINTERS*>(param);

    writeMiniDump(pExPtrs);
    return 0;
}


inline void dumpWithOtherThread(EXCEPTION_POINTERS* pExPtrs)
{
    const HANDLE hThread =
        ::CreateThread(NULL, 102400, dumpThread, pExPtrs, 0, NULL);
    (void)::WaitForSingleObject(hThread, INFINITE);
    (void)::CloseHandle(hThread);
}


LONG __stdcall theCrashHandler(EXCEPTION_POINTERS* pExPtrs)
{
    // 스택 오버플로 발생 시에는 별도의 스레드를 생성해서 덤프를 기록한다.
    // http://serious-code.net/moin.cgi/CppDebuggingTips#head-542083e07fc1db7784e0c88b48594020973726bd
    if (pExPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW) {
        dumpWithOtherThread(pExPtrs);
    }
    else {
        setBaseFilename();

        writeReport(pExPtrs);
        writeMiniDump(pExPtrs);
    }

    return EXCEPTION_EXECUTE_HANDLER;
}


void theInvalidParameterHandler(const wchar_t* expression,
    const wchar_t* function, const wchar_t* file, unsigned int line,
    uintptr_t /*pReserved*/)
{
    // Release 모드에서는 모든 인자값이 0
    SNE_LOG_ERROR("CRT error: Invalid parameter detected in function %S."
        " File: %S Line: %d\nExpression: %S",
        function, file, line, expression);

    crashNow();
}


void thePureCallHandler()
{
    SNE_LOG_ERROR("Pure virtual function is called!");

    crashNow();
}

} // namespace

void setCrashHandler(const std::string& dir, const std::string& filenamePrefix)
{
    SNE_ASSERT(! dir.empty());
    SNE_ASSERT(! filenamePrefix.empty());

    strcpy(s_dumpDir, dir.c_str());
    strcpy(s_dumpFilenamePrefix, filenamePrefix.c_str());
    (void)setCrashHandlerFilter(&theCrashHandler);
}

// http://ricanet.com/new/view.php?id=blog/080403
void setCrtInvalidParameterHandler()
{
    (void)_set_invalid_parameter_handler(theInvalidParameterHandler);
}

// http://easyisright.net/593
void setPureVirtualFunctionCallHandler()
{
    (void)_set_purecall_handler(thePureCallHandler);
}

}} // namespace sne { namespace server {

#endif // _WIN32
