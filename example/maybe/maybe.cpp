/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    {
        //! [maybe]
        BOOST_HANA_CONSTEXPR_LAMBDA auto inc = [](auto x) { return x + 1; };
        BOOST_HANA_CONSTEXPR_CHECK(maybe('x', inc, just(1)) == 2);
        BOOST_HANA_CONSTEXPR_CHECK(maybe('x', inc, nothing) == 'x');
        //! [maybe]
    }

    {
        //! [is_just]
        BOOST_HANA_CONSTANT_CHECK( is_just(just('x')));
        BOOST_HANA_CONSTANT_CHECK( is_just(just(nothing)));
        BOOST_HANA_CONSTANT_CHECK(!is_just(nothing));
        //! [is_just]
    }

    {
        //! [is_nothing]
        BOOST_HANA_CONSTANT_CHECK( is_nothing(nothing));
        BOOST_HANA_CONSTANT_CHECK(!is_nothing(just('x')));
        BOOST_HANA_CONSTANT_CHECK(!is_nothing(just(nothing)));
        //! [is_nothing]
    }

    {
        //! [from_maybe]
        BOOST_HANA_CONSTEXPR_CHECK(from_maybe('x', just(1)) == 1);
        BOOST_HANA_CONSTEXPR_CHECK(from_maybe('x', nothing) == 'x');
        //! [from_maybe]
    }

    {
        //! [only_when]
        BOOST_HANA_CONSTEXPR_LAMBDA auto even = [](auto x) {
            return x % int_<2> == int_<0>;
        };

        BOOST_HANA_CONSTEXPR_LAMBDA auto half = [](auto x) {
            return x / int_<2>;
        };

        BOOST_HANA_CONSTANT_CHECK(only_when(even, half, int_<4>) == just(int_<2>));
        BOOST_HANA_CONSTANT_CHECK(only_when(even, half, int_<3>) == nothing);
        //! [only_when]
    }

    {
        //! [from_just]
        BOOST_HANA_CONSTEXPR_CHECK(from_just(just('x')) == 'x');
        // from_just(nothing); // compile-time static assertion
        // ! [from_just]
    }

    {
        //! [nothing]
        constexpr auto x = nothing;
        //! [nothing]
        (void)x;
    }

    {
        //! [just]
        BOOST_HANA_CONSTEXPR_LAMBDA auto just_x = just('x');
        //! [just]
        (void)just_x;
    }
}
