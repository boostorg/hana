/*
This file is used to as a dirty worksheet for prototyping and checking stuff
that does not need to be documented. Always leave this empty.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integer_list.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#include <type_traits>
namespace hana = boost::hana;


// A trait representing whether a data type has a single C++ type. This
// is entirely optional, but can enable some optimizations.
template <typename T, typename Enable = void>
struct is_homogeneous {
    static constexpr bool value = false;
};

template <typename T>
struct is_homogeneous<T, std::enable_if_t<std::is_arithmetic<T>::value>> {
    static constexpr bool value = true;
};


// a list of Types
template <typename ...xs>
constexpr auto list_t = hana::type_list<xs...>;

// a list of homogeneous Constants
template <typename X, X ...xs>
constexpr auto list_c = hana::integer_list<X, xs...>;


template <typename T, typename Enable = void>
struct list_of {
    template <typename ...X>
    constexpr auto operator()(X ...x) const
    { return hana::list(x...); }
};

template <>
struct list_of<hana::Type> {
    template <typename ...X>
    constexpr auto operator()(X ...) const
    { return list_t<typename X::type...>; }
};

template <typename T>
struct list_of<T, std::enable_if_t<is_homogeneous<T>::value>> {
    template <typename ...X>
    constexpr auto operator()(X ...x) const
    { return hana::list(x...); } // would use an array
};




struct _list {
    template <typename T, typename ...X>
    static constexpr auto of(X ...x)
    { return list_of<T>{}(x...); }

    struct Any;
    template <typename ...X>
    constexpr auto operator()(X ...x) const
    { return of<Any>(x...); }
};

constexpr _list list{};


template <typename ...> struct a_type { };
auto show = [](auto ...x) {
    using k = a_type<
        typename a_type<decltype(x)>::show...
    >;
};

int main() {
    list(1, 2, 3); // normal version

    // uses a list_t internally
    list.of<hana::Type>(hana::type<int>, hana::type<void>, hana::type<char>);

    auto z = list.of<hana::Type>;
    // z(hana::type<int>);
    // show(z);


    // uses an array internally
    list.of<int>(1, 2, 3);
}
