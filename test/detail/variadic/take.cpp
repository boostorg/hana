/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/take.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


auto check = [](auto take) {
    auto result = take(tuple);
    return [=](auto ...xs) {
        BOOST_HANA_CONSTANT_ASSERT(equal(tuple(xs...), result));
    };
};

int main() {
    namespace vd = detail::variadic;
    using test::x;

    {
        check(vd::take<0>())
            ();

        check(vd::take<0>(x<1>))
            ();

        check(vd::take<0>(x<1>, x<2>))
            ();
    }
    {
        check(vd::take<1>(x<1>))
            (x<1>);

        check(vd::take<1>(x<1>, x<2>))
            (x<1>);

        check(vd::take<1>(x<1>, x<2>, x<3>))
            (x<1>);
    }
    {
        check(vd::take<8>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>);

        check(vd::take<8>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>);
    }
}
