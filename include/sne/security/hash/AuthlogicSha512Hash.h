#pragma once

#include <sne/Common.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace security {

    /**
    * @class AuthlogicSha512Hash
    *
    * Ruby on Rails Device gem's AuthlogicSha512 hash
    */
    class SNE_API AuthlogicSha512Hash : public boost::noncopyable
    {
    public:
        std::string digest(const std::string& password, const std::string passwordSalt,
            size_t stretches = 10);
    };

}} // namespace sne { namespace security {
