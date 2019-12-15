#pragma once

#include "RpcTypes.h"
#include "Exception.h"
#include <sne/core/stream/Exception.h>
#include <sne/core/container/Containers.h>
#include <algorithm>
#include <functional>

namespace sne { namespace srpc {

/** @addtogroup Stream
* @{
*/

struct NoReservePolicy
{
    template <typename Container>
    static inline void reserve(Container& /*container*/, size_t /*size*/) {}
};


struct VectorReservePolicy
{
    template <typename Container>
    static inline void reserve(Container& container, size_t size) {
        container.reserve(size);
    }
};

/// Standard C++ Library의 Sequence Container에 대한 RpcType
template <typename SeqContainer, typename ReservePolicy, size_t sizeBits>
class RpcContainer : public SeqContainer
{
public:
    typedef SeqContainer Base;
    typedef typename Base::value_type RpcType;
    typedef typename Base::iterator iterator;
public:
    void serialize(core::OStream& ostream) {
        if (this->size() > core::MaxNumber<sizeBits>::value) {
            throw core::StreamingException(__FILE__, __LINE__,
                "container size is too big");
        }

        static_assert(sizeof(uint16_t) == (sizeBits / CHAR_BIT), "");
        ostream.write(static_cast<uint16_t>(this->size()), sizeBits);
        const size_t maxNumber = core::MaxNumber<sizeBits>::value;
        assert(this->size() <= maxNumber);
        iterator pos = this->begin();
        const iterator end = this->end();
        for (size_t s = 0; (pos != end) && (s < maxNumber); ++pos, ++s) {
            ostream << (*pos);
        }
    }

    void serialize(core::IStream& istream) {
        this->clear();

        uint16_t size;
        static_assert(sizeof(size) == (sizeBits / CHAR_BIT), "");
        istream.read(size, sizeBits);
        if (size > core::MaxNumber<sizeBits>::value) {
            throw core::StreamingException(__FILE__, __LINE__,
                "container size is too big");
        }

        ReservePolicy::reserve(*this, size);
        for (uint32_t i = 0; i < size; ++i) {
            RpcType rt;
            istream >> rt;
            this->push_back(rt);
        }
    }
};


/// std::vector에 대한 RpcType
template <typename RpcType, size_t sizeBits = SNE_DEFAULT_CONTAILER_SIZE_BITS>
class RVector :
    public RpcContainer<core::Vector<RpcType>, VectorReservePolicy, sizeBits>
{};


/// std::list에 대한 RpcType
template <typename RpcType, size_t sizeBits = SNE_DEFAULT_CONTAILER_SIZE_BITS>
class RList :
    public RpcContainer<core::List<RpcType>, NoReservePolicy, sizeBits>
{};


/// std::*set에 대한 RpcType
template <typename BaseType, size_t sizeBits = SNE_DEFAULT_CONTAILER_SIZE_BITS>
class RBaseSet : public BaseType
{
public:
    typedef BaseType Base;
    typedef typename Base::iterator iterator;
    typedef typename Base::key_type RpcType;
public:
    void serialize(core::OStream& ostream) {
        if (this->size() > core::MaxNumber<sizeBits>::value) {
            throw core::StreamingException(__FILE__, __LINE__,
                "container size is too big");
        }

        static_assert(sizeof(uint16_t) == (sizeBits / CHAR_BIT), "");
        ostream.write(static_cast<uint16_t>(this->size()), sizeBits);
        const size_t maxNumber = core::MaxNumber<sizeBits>::value;
        assert(this->size() <= maxNumber);
        iterator pos = this->begin();
        const iterator end = this->end();
        for (size_t s = 0; (pos != end) && (s < maxNumber); ++pos, ++s) {
            ostream << (*pos);
        }
    }

    void serialize(core::IStream& istream) {
        this->clear();

        uint16_t size;
        static_assert(sizeof(size) == (sizeBits / CHAR_BIT), "");
        istream.read(size, sizeBits);
        if (size > core::MaxNumber<sizeBits>::value) {
            throw core::StreamingException(__FILE__, __LINE__,
                "container size is too big");
        }
        for (uint32_t i = 0; i < size; ++i) {
            RpcType rt;
            istream >> rt;
            this->insert(rt);
        }
    }
};

/// std::set에 대한 RpcType
template <typename RpcType>
class RSet : public RBaseSet<core::Set<RpcType> >
{
};

/// std::multiset에 대한 RpcType
template <typename RpcType>
class RMultiSet : public RBaseSet<core::MultiSet<RpcType> >
{
};


/// HashSet에 대한 RpcType
template <typename RpcType>
class RHashSet : public RBaseSet<core::HashSet<RpcType> >
{
};

/// HashMultiSet에 대한 RpcType
template <typename RpcType>
class RHashMultiSet : public RBaseSet<core::HashMultiSet<RpcType> >
{
};


/// std::*map에 대한 RpcType
template <typename BaseType, typename K, typename V, size_t sizeBits>
class RBaseMap : public BaseType
{
public:
    typedef BaseType Base;
    typedef typename Base::value_type value_type;
    typedef typename Base::iterator iterator;
public:
    void serialize(core::OStream& ostream) {
        if (this->size() > core::MaxNumber<sizeBits>::value) {
            throw core::StreamingException(__FILE__, __LINE__,
                "container size is too big");
        }

        ostream.write(static_cast<uint32_t>(this->size()), sizeBits);
        const size_t maxNumber = core::MaxNumber<sizeBits>::value;
        assert(this->size() <= maxNumber);
        iterator pos = this->begin();
        const iterator end = this->end();
        size_t s = 0;
        while ((pos != end) && (s < maxNumber)) {
            ostream << (*pos).first << (*pos).second;
            ++pos;
            ++s;
        }
    }

    void serialize(core::IStream& istream) {
        this->clear();

        uint32_t size;
        istream.read(size, sizeBits);
        if (size > core::MaxNumber<sizeBits>::value) {
            throw core::StreamingException(__FILE__, __LINE__,
                "container size is too big");
        }
        K key;
        V value;
        for (uint32_t i = 0; i < size; ++i) {
            istream >> key >> value;
            this->insert(value_type(key, value));
        }
    }
};


/// std::map에 대한 RpcType
template <typename K, typename V, size_t sizeBits = SNE_DEFAULT_CONTAILER_SIZE_BITS>
class RMap : public RBaseMap<core::Map<K, V>, K, V, sizeBits>
{
};

/// std::multimap에 대한 RpcType
template <typename K, typename V, size_t sizeBits = SNE_DEFAULT_CONTAILER_SIZE_BITS>
class RMultiMap : public RBaseMap<core::MultiMap<K, V>, K, V, sizeBits>
{
};


/// HashMap에 대한 RpcType
template <typename K, typename V, size_t sizeBits = SNE_DEFAULT_CONTAILER_SIZE_BITS>
class RHashMap : public RBaseMap<core::HashMap<K, V>, K, V, sizeBits>
{
};

/// HashMultiMap에 대한 RpcType
template <typename K, typename V, size_t sizeBits = SNE_DEFAULT_CONTAILER_SIZE_BITS>
class RHashMultiMap : public RBaseMap<core::HashMultiMap<K, V>, K, V, sizeBits>
{
};


/// Array에 대한 RpcType
template<class T, std::size_t N>
class RArray : public std::array<T, N>
{
public:
    void serialize(core::OStream& ostream) {
        for (size_t i = 0; i < N; ++i) {
            ostream << this->data()[i];
        }
    }

    void serialize(core::IStream& istream) {
        for (size_t i = 0; i < N; ++i) {
            istream >> this->data()[i];
        }
    }
};

/** @} */ // addtogroup Stream

}} // namespace sne { namespace srpc
