#ifndef PACK_H
#define PACK_H

#include <tuple>
#include <typeinfo>
#include <type_traits>


namespace pack_detail {
    // 1.1 Give number of types contained in Args...
    template <typename... Args>
    constexpr int packSize = sizeof...(Args);


    // 1.2 return Args[QueryIndex]
    template <int QueryIndex, typename... Args>
    struct _packIndexToType;

    template <int QueryIndex, typename Arg1, typename... Args>
    struct _packIndexToTypeHelper;

    template <int QueryIndex, typename... Args>
    using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;

    template <int QueryIndex, typename Arg1, typename... Args>
    struct _packIndexToTypeHelper/*<QueryIndex, Arg1, Args...>*/ {
        using type = std::conditional_t<QueryIndex == 0, typename Arg1::type,
                                        typename _packIndexToTypeHelper<QueryIndex - 1, Args...>::type>;
    };

    template <int QueryIndex, typename... Args>
    struct _packIndexToType/*<QueryIndex, Args...>*/ {
        static_assert(QueryIndex >= 0, "Index must be nonnegative.");
        static_assert(QueryIndex < packSize<Args...>, "Index larger than number of elements.");

        using type = typename _packIndexToTypeHelper<QueryIndex - 1, Args...>::type;
    };


    // 1.3 Store the first index at which QueryType occurs
    template <int Accumulated, typename QueryType, typename... Args>
    constexpr int _packTypeToIndex = (std::is_same_v<packIndexToType<Accumulated, Args...>(), QueryType>) ? Accumulated
                                        : (Accumulated >= packSize<Args...>) ? -1
                                        : _packTypeToIndex<Accumulated + 1, QueryType, Args...>;

    template <typename QueryType, typename... Args>
    constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;

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
