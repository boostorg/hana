/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/list.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename ...T>
constexpr auto list_t = list(type<T>...);

int main() {
    static_assert(
        fmap(trait::add_pointer, list_t<void, int, char>)
        ==
        list_t<void*, int*, char*>
    , "");

    static_assert(
        head(fmap(trait::add_pointer, list_t<void, int, char>))
        ==
        type<void*>
    , "");

    static_assert(std::is_same<
        decltype(
            head(fmap(trait::add_pointer, list_t<void, int, char>))
        )::type,
        void*
    >::value, "");
}
