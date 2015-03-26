/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/split_at.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
using namespace boost::hana;


auto check = [](auto split) {
    auto result = split([](auto ...xs) {
        return [=](auto ...ys) {
            return pair(make<Tuple>(xs...), make<Tuple>(ys...));
        };
    });

    return [=](auto ...xs) {
        BOOST_HANA_CONSTANT_CHECK(equal(make<Tuple>(xs...), first(result)));
        return [=](auto ...ys) {
            BOOST_HANA_CONSTANT_CHECK(equal(make<Tuple>(ys...), second(result)));
        };
    };
};

int main() {
    namespace vd = detail::variadic;
    using test::ct_eq;

    {
        check(vd::split_at<0>())
            ()
            ();

        check(vd::split_at<0>(ct_eq<1>{}))
            ()
            (ct_eq<1>{});

        check(vd::split_at<0>(ct_eq<1>{}, ct_eq<2>{}))
            ()
            (ct_eq<1>{}, ct_eq<2>{});

        check(vd::split_at<0>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}))
            ()
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{});
    }
    {
        check(vd::split_at<1>(ct_eq<1>{}))
            (ct_eq<1>{})
            ();

        check(vd::split_at<1>(ct_eq<1>{}, ct_eq<2>{}))
            (ct_eq<1>{})
            (ct_eq<2>{});

        check(vd::split_at<1>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}))
            (ct_eq<1>{})
            (ct_eq<2>{}, ct_eq<3>{});

        check(vd::split_at<1>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}))
            (ct_eq<1>{})
            (ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{});
    }
    {
        check(vd::split_at<2>(ct_eq<1>{}, ct_eq<2>{}))
            (ct_eq<1>{}, ct_eq<2>{})
            ();

        check(vd::split_at<2>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}))
            (ct_eq<1>{}, ct_eq<2>{})
            (ct_eq<3>{});

        check(vd::split_at<2>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}))
            (ct_eq<1>{}, ct_eq<2>{})
            (ct_eq<3>{}, ct_eq<4>{});

        check(vd::split_at<2>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}))
            (ct_eq<1>{}, ct_eq<2>{})
            (ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{});
    }
    {
        check(vd::split_at<7>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{})
            ();

        check(vd::split_at<7>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{})
            (ct_eq<8>{});

        check(vd::split_at<7>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{})
            (ct_eq<8>{}, ct_eq<9>{});

        check(vd::split_at<7>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{}, ct_eq<10>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{})
            (ct_eq<8>{}, ct_eq<9>{}, ct_eq<10>{});
    }
    {
        check(vd::split_at<8>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{})
            ();

        check(vd::split_at<8>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{})
            (ct_eq<9>{});

        check(vd::split_at<8>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{}, ct_eq<10>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{})
            (ct_eq<9>{}, ct_eq<10>{});
    }
    {
        check(vd::split_at<9>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{})
            ();

        check(vd::split_at<9>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{}, ct_eq<10>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{})
            (ct_eq<10>{});

        check(vd::split_at<9>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{}, ct_eq<10>{}, ct_eq<11>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{})
            (ct_eq<10>{}, ct_eq<11>{});
    }
}
