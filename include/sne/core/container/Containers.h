#pragma once

// 컨테이너 타입(STL)을 정의한다

#include "../memory/StdAllocator.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702)
#endif
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

namespace sne { namespace core {

/** @addtogroup containers
* @{
*/

// = Sequence containers

/// Deque - std::deque
template <typename T>
class Deque : public std::deque<T, SneNodeAllocator<T> >
{};

/// List - std::list
template <typename T>
class List : public std::list<T, SneNodeAllocator<T> >
{};

/// Vector - std::vector
template <typename T>
class Vector : public std::vector<T, SneNormalAllocator<T> >
{};

/// PriorityQueue - std::priority_queue
template <typename T, typename Container = Deque<T>,
    typename Compare = std::less<typename Container::value_type> >
class PriorityQueue : public std::priority_queue<T, Container, Compare>
{};

/// Queue - std::queue
template <typename T, typename Container = Deque<T> >
class Queue : public std::queue<T, Container>
{};

/// Stack - std::stack
template <typename T, typename Container = Deque<T> >
class Stack : public std::stack<T, Container>
{};

// = Associative containers

/// Map - std::map
template <typename Key, typename Type, typename Traits = std::less<Key> >
class Map : public std::map<Key, Type, Traits,
    SneNodeAllocator<std::pair<const Key, Type> > >
{};

/// MultiMap - std::multimap
template <typename Key, typename Type, typename Traits = std::less<Key> >
class MultiMap : public std::multimap<Key, Type, Traits,
    SneNodeAllocator<std::pair<const Key, Type> > >
{};

/// Set - std::set
template <typename Key, typename Traits = std::less<Key> >
class Set : public std::set<Key, Traits, SneNodeAllocator<Key> >
{};

/// MultiSet - std::multiset
template <typename Key, typename Traits = std::less<Key> >
class MultiSet : public std::multiset<Key, Traits, SneNodeAllocator<Key> >
{};


/// HashMap - std::unordered_map
template <typename Key, typename Mapped,
    typename Hash = std::hash<Key>,
    typename Pred = std::equal_to<Key> >
class HashMap : public std::unordered_map<Key, Mapped, Hash, Pred,
    SneNodeAllocator<std::pair<const Key, Mapped> > >
{};

/// HashMutliMap - std::unordered_multimap
template <typename Key, typename Mapped,
    typename Hash = std::hash<Key>,
    typename Pred = std::equal_to<Key> >
class HashMultiMap : public std::unordered_multimap<Key, Mapped, Hash, Pred,
    SneNodeAllocator<std::pair<const Key, Mapped> > >
{};


/// HashSet - boost::unordered_set
template <typename Key,
    typename Hash = std::hash<Key>,
    typename Pred = std::equal_to<Key> >
class HashSet :
    public std::unordered_set<Key, Hash, Pred, SneNodeAllocator<Key> >
{
};

/// HashMultiSet - boost::unordered_multiset
template <typename Key,
    typename Hash = std::hash<Key>,
    typename Pred = std::equal_to<Key> >
class HashMultiSet :
    public std::unordered_multiset<Key, Hash, Pred, SneNodeAllocator<Key> >
{};

/** @} */ // addtogroup containers

}} // namespace sne { namespace core {
