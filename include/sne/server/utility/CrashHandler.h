#pragma once

#include <sne/Common.h>

namespace sne { namespace server {

/**
 * BugSlayer CrashHandler(MiniCore Dump)를 설정한다
 * - 덤프파일은 dir\\filenamePrefix_pid.DMP 형식으로 저장된다
 * @parm dir 덤프파일을 기록할 디렉토리(끝에 \가 붙어야 한다)
 * @parm filenamePrefix 덤프파일명 앞에 붙을 접두어
 */
void SNE_API setCrashHandler(const std::string& dir,
    const std::string& filenamePrefix);


/// CRT 함수에 잘못된 인자값이 넘어갈 경우 발생하는 핸들러를 설정하여 로그를
/// 남기도록 한다
void SNE_API setCrtInvalidParameterHandler();


/// Pure virtual function call handler
void SNE_API setPureVirtualFunctionCallHandler();

}} // namespace sne { namespace server {
