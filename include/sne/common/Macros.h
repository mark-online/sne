#pragma once

#include <climits>

#if defined(_MSC_VER)
#   ifdef SNE_DLL
#       ifdef SNE_EXPORT
#           define SNE_API __declspec(dllexport)
#       else
#           define SNE_API __declspec(dllimport)
#       endif
#   else
#       define SNE_API
#   endif // SNE_DLL
#endif

#if !defined(SNE_API)
#   define SNE_API
#endif


/// std::shared_ptr에서 quick_allocator를 사용하도록 설정
//#define BOOST_SP_USE_QUICK_ALLOCATOR

// TODO: 활성화할 경우 macOS에서 테스트 실패함(RpcCommandTest.testMarshal)
//#define SNE_USE_BOOST_POOL_ALLOCATOR
//#define SNE_USE_BOOST_FAST_POOL_ALLOCATOR

//#define SNE_USE_BOOST_MEMORY_POOL

#define SNE_DEFAULT_CONTAILER_SIZE_BITS (CHAR_BIT * 2)
#define SNE_DEFAULT_STRING_SIZE_BITS (CHAR_BIT * 2)
