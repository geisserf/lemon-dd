#ifndef EVMDD_CACHE_H
#define EVMDD_CACHE_H

#include <map>

template <typename E, typename L, typename R>
using CacheMap = std::map<std::pair<L, R>, E>;

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
