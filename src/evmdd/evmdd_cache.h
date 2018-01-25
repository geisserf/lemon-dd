#ifndef EVMDD_CACHE_H
#define EVMDD_CACHE_H

#include <map>

// Currently we use a standard map for caching. If caching becomes a major
// bottleneck we may want to switch to a unordered_map and implement our own
// hash function for EVMDDs.
template <typename E, typename L, typename R>
using CacheMap = std::map<std::pair<L, R>, E>;

// Cache used for apply-operations of type L OP R -> E, where L,R,E are EVMDDs
// and OP is a function mapping from carrier types of L and R to the carrier
// type of E.
template <typename E, typename L, typename R, typename OP>
class EvmddCache {
public:
    static typename CacheMap<E, L, R>::iterator find(L const &l, R const &r) {
        return cache.find(std::pair<L, R>(l, r));
    }

    static void add(L const &l, R const &r, E const &result) {
        cache[std::pair<L, R>(l, r)] = result;
    }

    static typename CacheMap<E, L, R>::iterator end() {
        return cache.end();
    }

private:
    static CacheMap<E, L, R> cache;
};

template <typename E, typename L, typename R, typename OP>
CacheMap<E, L, R> EvmddCache<E, L, R, OP>::cache = CacheMap<E, L, R>();

#endif /* EVMDD_CACHE_H */
