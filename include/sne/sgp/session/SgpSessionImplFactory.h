#pragma once

#include "SgpSessionConfig.h"
#include <sne/Common.h>
#include <memory>

namespace sne { namespace base {
class SessionImpl;
}} // namespace sne { namespace base {

namespace sne { namespace sgp {

/** @addtogroup sgp
* @{
*/

/**
 * @class SgpSessionImplFactory
 *
 * 네트웍 연결 세션 Implemetation Factory
 * - Reactor 기반 또는 Proactor 기반 세션을 생성한다
 */
struct SNE_API SgpSessionImplFactory
{
    static std::unique_ptr<base::SessionImpl>
        create(const SgpSessionConfig& config);
};

/** @} */ // addtogroup sgp

}} // namespace sne { namespace sgp {
