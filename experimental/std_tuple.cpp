/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


//////////////////////////////////////////////////////////////////////////////
// tuple
//////////////////////////////////////////////////////////////////////////////
template <typename ...T>
/* constexpr */ decltype(auto) make_storage(T&& ...t) {
    return [t...](auto&& f) -> decltype(auto) {
        return std::forward<decltype(f)>(f)(t...);
    };
}

template <typename ...T>
struct tuple {
    using Storage = decltype(make_storage(std::declval<T>()...));
    Storage unpack_into;

    template <typename ...U>
    /* constexpr */ explicit tuple(U&& ...u)
        : unpack_into(make_storage(std::forward<U>(u)...))
    { };
};

template <typename ...T>
/* constexpr */ tuple<std::decay_t<T>...> make_tuple(T&& ...t) {
    return tuple<std::decay_t<T>...>{std::forward<T>(t)...};
}

//////////////////////////////////////////////////////////////////////////////
// get
//////////////////////////////////////////////////////////////////////////////
template <std::size_t n, typename = std::make_index_sequence<n>>
struct get_impl;

template <std::size_t n, std::size_t ...ignore>
struct get_impl<n, std::index_sequence<ignore...>> {
    template <typename Nth>
    static constexpr decltype(auto) apply
    (decltype(ignore, (void const*)0)..., Nth* nth, ...)
    { return nth; }

    template <typename ...T>
    constexpr decltype(auto) operator()(T&& ...t) const
    { return apply(&t...); }
};

template <std::size_t n, typename ...T>
constexpr decltype(auto) get(tuple<T...>& ts) {
    return *ts.unpack_into(get_impl<n>{});
}

template <std::size_t n, typename ...T>
constexpr decltype(auto) get(tuple<T...> const& ts) {
    return *ts.unpack_into(get_impl<n>{});
}

template <std::size_t n, typename ...T>
constexpr decltype(auto) get(tuple<T...>&& ts) {
    return std::move(*ts.unpack_into(get_impl<n>{}));
}


int main() {
    // tuple
    tuple<int, char, double> ts{1, '2', 3.3};

    // make_tuple
    make_tuple();
    make_tuple(1, '2', 3.3);

    // get
    BOOST_HANA_RUNTIME_CHECK(get<0>(ts) == 1);
    BOOST_HANA_RUNTIME_CHECK(get<1>(ts) == '2');
    BOOST_HANA_RUNTIME_CHECK(get<2>(ts) == 3.3);
}
