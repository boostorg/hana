/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/seq.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/integral_constant.hpp>
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


namespace boost { namespace hana {
    namespace test {
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
    }

    template <>
    struct enabled_operators<test::Seq>
        : Iterable
    { };
}}


int main() {
    test::check_datatype<test::Seq>();
    using test::x;

    // Iterable (everything else is checked by the tests for List)
    {
        auto iterable = test::seq;

        // operators
        {
            // T&
            {
                auto lvalue = iterable(x<0>);

                auto const& result = at(size_t<0>, lvalue);
                (void)result;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    lvalue[size_t<0>],
                    at(size_t<0>, lvalue)
                ));
            }

            // T const&
            {
                auto const const_lvalue = iterable(x<0>);

                auto const& result = at(size_t<0>, const_lvalue);
                (void)result;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    const_lvalue[size_t<0>],
                    at(size_t<0>, const_lvalue)
                ));
            }

            // T&&
            {
                auto rvalue = [=] { return iterable(x<0>); };

                auto&& result = at(size_t<0>, rvalue());
                (void)result;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    rvalue()[size_t<0>],
                    at(size_t<0>, rvalue())
                ));
            }

            // T const&&
            {
                auto const_rvalue = [=]() -> decltype(iterable(x<0>)) const {
                    return iterable(x<0>);
                };

                auto const&& result = at(size_t<0>, const_rvalue());
                (void)result;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    const_rvalue()[size_t<0>],
                    at(size_t<0>, const_rvalue())
                ));
            }
        }
    }
}
