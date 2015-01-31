/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>
#include <test/injection.hpp>
#include <utility>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto objects<Tuple> = tuple(
            tuple()
          , tuple(x<0>)
          , tuple(x<0>, x<1>)
          , tuple(x<0>, x<1>, x<2>)
          , tuple(x<0>, x<1>, x<2>, x<3>)
          , tuple(x<0>, x<1>, x<2>, x<3>, x<4>)
          , tuple(x<0>, x<1>, x<2>, x<3>, x<4>, x<5>)
    );

    template <>
    auto instances<Tuple> = tuple(
          type<List>
        , type<Functor>
        , type<Foldable>
        , type<Iterable>
    );
}}}


int main() {
    test::check_datatype<Tuple>();
    using test::x;

    // move-only friendlyness and reference semantics
    {
        struct movable {
            movable() = default;
            movable(movable const&) = delete;
            movable(movable&&) = default;
        };

        {
            auto xs = tuple(movable{});
            auto by_val = [](auto) { };

            by_val(std::move(xs));
            by_val(head(std::move(xs)));
            by_val(at_c<0>(std::move(xs)));
            by_val(last(std::move(xs)));
        }

        {
            auto const& xs = tuple(movable{});
            auto by_const_ref = [](auto const&) { };

            by_const_ref(xs);
            by_const_ref(head(xs));
            by_const_ref(at_c<0>(xs));
            by_const_ref(last(xs));
        }

        {
            auto xs = tuple(movable{});
            auto by_ref = [](auto&) { };

            by_ref(xs);
            by_ref(head(xs));
            by_ref(at_c<0>(xs));
            by_ref(last(xs));
        }
    }

    // Foldable
    {
        // unpack
        {
            auto f = test::injection([]{});

            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple(), f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple(x<0>), f),
                f(x<0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple(x<0>, x<1>), f),
                f(x<0>, x<1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple(x<0>, x<1>, x<2>), f),
                f(x<0>, x<1>, x<2>)
            ));
        }
    }

    // Iterable
    {
        // operators
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tuple(x<0>, x<1>)[int_<0>],
                x<0>
            ));
        }
    }
}
