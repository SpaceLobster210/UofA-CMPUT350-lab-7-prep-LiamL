#ifndef PACK_H
#define PACK_H

#include <tuple>
#include <typeinfo>


namespace pack_detail {
    // 1.1 Give number of types contained in Args...
    template <typename... Args>
    constexpr int packSize = sizeof...(Args);


    // 1.2 return Args[QueryIndex]
    template <int QueryIndex, typename... Args>
    struct _packIndexToType<QueryIndex, Args...> {
        static_assert(QueryIndex >= 0, "Index must be nonnegative.");
        static_assert(QueryIndex < packSize<Args...>, "Index larger than number of elements.");

        using type = typename _packIndexToType<QueryIndex - 1, Args...>::type;
    };

    template <int QueryIndex, typename Arg1, typename... Args>
    struct _packIndexToType<QueryIndex - 1, Arg1, Args...> {
        using type = (QueryIndex == 0) ? typename Arg1::type
                        : typename _packIndexToType<QueryIndex - 1, Args...>::type;
    };

    template <int QueryIndex, typename... Args>
    using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;

    // 1.3 Store the first index at which QueryType occurs
    template <typename QueryType, typename... Args>
    constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;

    template <int Accumulated, typename QueryType, typename... Args>
    constexpr int _packTypeToIndex = (std::is_same<packIndexToType<Accumulated, Args...>, QueryType>) ? Accumulated
                                        : (Accumulated >= packSize<Args...>) ? -1
                                        : _packTypeToIndex<Accumulated + 1, QueryType, Args...>;

} // pack_detail


template <typename... Args>
struct Pack {
    // 1.4 Variable holding packSize
    constexpr static int size = pack_detail::packSize<Args...>;

    // 1.5 Nested type alias template holding packIndexToType
    template <int QueryIndex>
    using type = pack_detail::packIndexToType<QueryIndex, Args...>;

    // 1.6 Nested variable template holding packTypeToIndex
    template <typename QueryType>
    constexpr static int index = pack_detail::packTypeToIndex<QueryType, Args...>;
};


#endif  // PACK_H
