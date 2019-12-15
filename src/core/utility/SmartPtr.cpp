#include "CorePCH.h"

#ifdef SNE_USE_SMARTPTR

#include <sne/core/utility/SmartPtr.h>
#include <boost/interprocess/detail/atomic.hpp>

namespace sne { namespace core {

void ThreadSafeSmartPtrTraits::addReference(SharedObject* rawPtr)
{
    if (rawPtr != nullptr) {
        boost::interprocess::detail::atomic_inc32(&rawPtr->referenceCount_);
    }
}


void ThreadSafeSmartPtrTraits::removeReference(SharedObject* rawPtr)
{
    if (rawPtr != nullptr) {
        if (1 == boost::interprocess::detail::atomic_dec32(
            &rawPtr->referenceCount_)) {
            delete rawPtr;
        }
    }
}

}} // namespace sne { namespace core {

#endif // SNE_USE_SMARTPTR