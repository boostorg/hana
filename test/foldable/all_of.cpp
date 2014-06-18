/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>

#include "minimal_foldable.hpp"
#include <cassert>
using namespace boost::hana;


struct invalid_value {
    using hana_datatype = invalid_value;
    //! @todo This should be part of Logical's mcd.
    invalid_value operator!() const { assert(false); return *this; }
};
struct invalid_type { };

namespace boost { namespace hana {
    template <>
    struct Logical<invalid_value> : defaults<Logical>::with<invalid_value> {
        template <typename T, typename F>
        static bool eval_if_impl(invalid_value, T, F)
        { assert(false); return true; }
    };
}}

int main() {
    // purely compile-time
    BOOST_HANA_STATIC_ASSERT(all_of(foldable()));
    BOOST_HANA_STATIC_ASSERT( all_of(foldable(int_<1>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<0>)));

    BOOST_HANA_STATIC_ASSERT( all_of(foldable(int_<1>, char_<1>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<1>, char_<0>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<0>, char_<1>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<0>, char_<0>)));

    BOOST_HANA_STATIC_ASSERT( all_of(foldable(int_<1>, char_<1>, long_<1>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<1>, char_<1>, long_<0>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<1>, char_<0>, long_<1>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<1>, char_<0>, long_<0>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<0>, char_<1>, long_<1>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<0>, char_<1>, long_<0>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<0>, char_<0>, long_<1>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<0>, char_<0>, long_<0>)));

    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<0>, invalid_type{})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int_<1>, long_<0>, invalid_type{})));


    // constexpr/runtime
    BOOST_HANA_STATIC_ASSERT( all_of(foldable(int{1})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{0})));

    BOOST_HANA_STATIC_ASSERT( all_of(foldable(int{1}, char{1})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{0}, char{1})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{1}, char{0})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{0}, char{0})));

    BOOST_HANA_STATIC_ASSERT( all_of(foldable(int{1}, char{1}, double{1})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{1}, char{1}, double{0})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{1}, char{0}, double{1})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{1}, char{0}, double{0})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{0}, char{1}, double{1})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{0}, char{1}, double{0})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{0}, char{0}, double{1})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{0}, char{0}, double{0})));

    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{0}, invalid_value{})));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{1}, long{0}, invalid_value{})));


    // mixed
    BOOST_HANA_STATIC_ASSERT( all_of(foldable(int{1}, int_<1>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{1}, int_<0>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{0}, int_<1>)));
    BOOST_HANA_STATIC_ASSERT(!all_of(foldable(int{0}, int_<0>)));
}
