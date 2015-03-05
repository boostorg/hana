/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/integral_constant.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/logical.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    auto ints = make<Tuple>(
        std::integral_constant<int, -10>{},
        std::integral_constant<int, -2>{},
        std::integral_constant<int, 0>{},
        std::integral_constant<int, 1>{},
        std::integral_constant<int, 3>{}
    );

    auto bools = make<Tuple>(std::true_type{}, std::false_type{});

    //////////////////////////////////////////////////////////////////////////
    // Logical
    //////////////////////////////////////////////////////////////////////////
    {
        auto t = test::ct_eq<3>{};
        auto e = test::ct_eq<4>{};

        // eval_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(std::true_type{}, always(t), always(e)),
                t
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(std::false_type{}, always(t), always(e)),
                e
            ));
        }

        // not_
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                not_(std::true_type{}),
                std::false_type{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                not_(std::false_type{}),
                std::true_type{}
            ));
        }

        // laws
        test::TestLogical<ext::std::IntegralConstant<int>>{ints};
        test::TestLogical<ext::std::IntegralConstant<bool>>{bools};
    }
}
