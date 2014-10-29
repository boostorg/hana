/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <test/cnumeric/constant.hpp>
using namespace boost::hana;


template <int ...>
auto constant_true = test::cnumeric<bool, true>;

template <int ...>
BOOST_HANA_CONSTEXPR_LAMBDA bool constexpr_true = []{ return true; }();

template <int ...>
bool runtime_true = true;


BOOST_HANA_CONSTANT_ASSERT(constant_true<>);
BOOST_HANA_CONSTANT_ASSERT(constant_true<0, 1>);

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(constexpr_true<>);
    BOOST_HANA_CONSTEXPR_ASSERT(constexpr_true<0, 1>);

    BOOST_HANA_RUNTIME_ASSERT(runtime_true<>);
    BOOST_HANA_RUNTIME_ASSERT(runtime_true<0, 1>);

    BOOST_HANA_ASSERT(runtime_true<>);
    BOOST_HANA_ASSERT(runtime_true<0, 1>);
    BOOST_HANA_ASSERT(constant_true<>);
    BOOST_HANA_ASSERT(constant_true<0, 1>);

    // make sure the side effects are performed by BOOST_HANA_CONSTANT_ASSERT
    // and BOOST_HANA_ASSERT
    {
        {
            bool executed = false;
            auto f = [&] {
                executed = true;
                return constant_true<>;
            };
            BOOST_HANA_CONSTANT_ASSERT(f());
            BOOST_HANA_RUNTIME_ASSERT(executed);
        }

        {
            bool executed_f = false, executed_g = false;
            auto f = [&] {
                executed_f = true;
                return constant_true<>;
            };

            auto g = [&] {
                executed_g = true;
                return runtime_true<>;
            };
            BOOST_HANA_ASSERT(f());
            BOOST_HANA_RUNTIME_ASSERT(executed_f);

            BOOST_HANA_ASSERT(g());
            BOOST_HANA_RUNTIME_ASSERT(executed_g);
        }
    }

    // make sure we can put lambdas in the assertions
    {
        BOOST_HANA_CONSTANT_ASSERT([]{ return constant_true<>; }());
        BOOST_HANA_CONSTEXPR_ASSERT([]{ return constexpr_true<>; }());
        BOOST_HANA_RUNTIME_ASSERT([]{ return runtime_true<>; }());
        BOOST_HANA_ASSERT([] { return constant_true<>; }());
        BOOST_HANA_ASSERT([] { return runtime_true<>; }());
    }
}
