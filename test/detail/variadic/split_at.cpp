/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/split_at.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


auto check = [](auto split) {
    auto result = split([](auto ...xs) {
        return [=](auto ...ys) {
            return pair(tuple(xs...), tuple(ys...));
        };
    });

    return [=](auto ...xs) {
        BOOST_HANA_CONSTANT_ASSERT(equal(tuple(xs...), first(result)));
        return [=](auto ...ys) {
            BOOST_HANA_CONSTANT_ASSERT(equal(tuple(ys...), second(result)));
        };
    };
};

int main() {
    namespace vd = detail::variadic;
    using test::x;

    {
        check(vd::split_at<0>())
            ()
            ();

        check(vd::split_at<0>(x<1>))
            ()
            (x<1>);

        check(vd::split_at<0>(x<1>, x<2>))
            ()
            (x<1>, x<2>);

        check(vd::split_at<0>(x<1>, x<2>, x<3>))
            ()
            (x<1>, x<2>, x<3>);
    }
    {
        check(vd::split_at<1>(x<1>))
            (x<1>)
            ();

        check(vd::split_at<1>(x<1>, x<2>))
            (x<1>)
            (x<2>);

        check(vd::split_at<1>(x<1>, x<2>, x<3>))
            (x<1>)
            (x<2>, x<3>);

        check(vd::split_at<1>(x<1>, x<2>, x<3>, x<4>))
            (x<1>)
            (x<2>, x<3>, x<4>);
    }
    {
        check(vd::split_at<2>(x<1>, x<2>))
            (x<1>, x<2>)
            ();

        check(vd::split_at<2>(x<1>, x<2>, x<3>))
            (x<1>, x<2>)
            (x<3>);

        check(vd::split_at<2>(x<1>, x<2>, x<3>, x<4>))
            (x<1>, x<2>)
            (x<3>, x<4>);

        check(vd::split_at<2>(x<1>, x<2>, x<3>, x<4>, x<5>))
            (x<1>, x<2>)
            (x<3>, x<4>, x<5>);
    }
    {
        check(vd::split_at<7>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>)
            ();

        check(vd::split_at<7>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>)
            (x<8>);

        check(vd::split_at<7>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>)
            (x<8>, x<9>);

        check(vd::split_at<7>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>, x<10>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>)
            (x<8>, x<9>, x<10>);
    }
    {
        check(vd::split_at<8>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>)
            ();

        check(vd::split_at<8>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>)
            (x<9>);

        check(vd::split_at<8>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>, x<10>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>)
            (x<9>, x<10>);
    }
    {
        check(vd::split_at<9>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>)
            ();

        check(vd::split_at<9>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>, x<10>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>)
            (x<10>);

        check(vd::split_at<9>(x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>, x<10>, x<11>))
            (x<1>, x<2>, x<3>, x<4>, x<5>, x<6>, x<7>, x<8>, x<9>)
            (x<10>, x<11>);
    }
}
