/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/take.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
using namespace boost::hana;


auto check = [](auto take) {
    auto result = take(make<Tuple>);
    return [=](auto ...xs) {
        BOOST_HANA_CONSTANT_CHECK(equal(make<Tuple>(xs...), result));
    };
};

int main() {
    namespace vd = detail::variadic;
    using test::ct_eq;

    {
        check(vd::take<0>())
            ();

        check(vd::take<0>(ct_eq<1>{}))
            ();

        check(vd::take<0>(ct_eq<1>{}, ct_eq<2>{}))
            ();
    }
    {
        check(vd::take<1>(ct_eq<1>{}))
            (ct_eq<1>{});

        check(vd::take<1>(ct_eq<1>{}, ct_eq<2>{}))
            (ct_eq<1>{});

        check(vd::take<1>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}))
            (ct_eq<1>{});
    }
    {
        check(vd::take<8>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{});

        check(vd::take<8>(ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{}, ct_eq<9>{}))
            (ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{}, ct_eq<4>{}, ct_eq<5>{}, ct_eq<6>{}, ct_eq<7>{}, ct_eq<8>{});
    }
}
