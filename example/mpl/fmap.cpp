/*
 * Copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/trait.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(
        fmap(trait::add_pointer, list_t<void, int(), char[10]>)
        ==
        list_t<void*, int(*)(), char(*)[10]>
    );

    BOOST_HANA_STATIC_ASSERT(
        head(fmap(trait::add_pointer, list_t<void, int(), char[10]>))
        ==
        type<void*>
    );

    static_assert(std::is_same<
        decltype(
            head(fmap(trait::add_pointer, list_t<void, int(), char[10]>))
        )::type,
        void*
    >::value, "");
}
