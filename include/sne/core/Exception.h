#pragma once

#include "utility/CppUtils.h"
#include <stdexcept>

namespace sne { namespace core {

/** @addtogroup Exception
* @{
*/

/**
 * @class Exception
 *
 * root runtime_error class.
 */
class Exception : public std::runtime_error
{
public:
    Exception(const char* file, int fileno, const char* msg) :
        std::runtime_error("") {
        what_ = formatString("Exception: (%s:%d), %s", file, fileno, msg);
    }

    virtual const char* what() const noexcept override {
        return what_.c_str();
    }
private:
    std::string what_;
};

/** @} */ // addtogroup Exception

}} // namespace sne { namespace core {
