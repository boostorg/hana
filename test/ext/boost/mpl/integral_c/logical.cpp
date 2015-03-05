/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/mpl/integral_c.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/logical.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
using namespace boost::hana;
namespace mpl = boost::mpl;


int main() {
    auto ints = make<Tuple>(
        mpl::int_<-2>{}, mpl::integral_c<int, 0>{}, mpl::integral_c<int, 3>{}
    );

    auto bools = make<Tuple>(
        mpl::true_{}, mpl::false_{},
        mpl::integral_c<bool, true>{}, mpl::integral_c<bool, false>{}
    );

    //////////////////////////////////////////////////////////////////////////
    // Logical
    //////////////////////////////////////////////////////////////////////////
    {
        auto t = test::ct_eq<3>{};
        auto e = test::ct_eq<4>{};

        // eval_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(mpl::true_{}, always(t), always(e)),
                t
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                eval_if(mpl::false_{}, always(t), always(e)),
                e
            ));
        }

        // not_
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                not_(mpl::true_{}),
                mpl::false_{}
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                not_(mpl::false_{}),
                mpl::true_{}
            ));
        }

        // laws
        test::TestLogical<ext::boost::mpl::IntegralC<int>>{ints};
        test::TestLogical<ext::boost::mpl::IntegralC<bool>>{bools};
    }
}
