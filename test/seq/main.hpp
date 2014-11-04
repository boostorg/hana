/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/seq.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>

// tested instances
#include <test/auto/foldable.hpp>
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>
#include <test/auto/traversable.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Seq> = tuple(
        type<List>,
        type<Foldable>,
        type<Iterable>,
        type<Traversable>
    );

    template <>
    auto objects<Seq> = tuple(
        seq(),
        seq(x<0>),
        seq(x<0>, x<1>),
        seq(x<0>, x<1>, x<2>),
        seq(x<0>, x<1>, x<2>, x<3>),
        seq(x<0>, x<1>, x<2>, x<3>, x<4>),
        seq(x<0>, x<1>, x<2>, x<3>, x<4>, x<5>)
    );
}}}


int main() {
    test::check_datatype<test::Seq>();
    using test::x;

    // Iterable (everything else is checked by the tests for List)
    {
        auto iterable = test::seq;

        // operators
        {
            auto const const_lvalue = iterable(x<0>);
            auto lvalue = iterable(x<0>);
            auto rvalue = [=] { return iterable(x<0>); };
            auto const_rvalue = [=]() -> decltype(iterable(x<0>)) const {
                return iterable(x<0>);
            };

            BOOST_HANA_CONSTANT_ASSERT(equal(
                lvalue[size_t<0>],
                at(size_t<0>, lvalue)
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                const_lvalue[size_t<0>],
                at(size_t<0>, const_lvalue)
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                rvalue()[size_t<0>],
                at(size_t<0>, rvalue())
            ));

            BOOST_HANA_CONSTANT_ASSERT(equal(
                const_rvalue()[size_t<0>],
                at(size_t<0>, const_rvalue())
            ));
        }
    }
}
