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

namespace check_adl {
    template <bool b = false>
    struct invalid { static_assert(b, "invalid must not be instantiated"); };

    template <typename T> void adl(T) { }
    void foo() {
        // ADL kicks in but `invalid<>` must not instantiated
        adl(tuple_t<invalid<>>);
    }
}


int main() {
    test::check_datatype<Tuple>();
    using test::x;

    // move-only friendliness and reference semantics
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

    // Comparable
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<>, tuple_t<>));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0>, tuple_t<>)));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<>, tuple_t<x0>)));
            BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0>, tuple_t<x0>));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0, x1>, tuple_t<x0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0>, tuple_t<x0, x1>)));
            BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0, x1>, tuple_t<x0, x1>));
            BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0, x1, x2>, tuple_t<x0, x1, x2>));
        }
    }

    // Foldable
    {
        // unpack
        {
            auto f = test::injection([]{});

            // tuple
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

            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<>, f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0>, f),
                f(type<x0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1>, f),
                f(type<x0>, type<x1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1, x2>, f),
                f(type<x0>, type<x1>, type<x2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1, x2, x3>, f),
                f(type<x0>, type<x1>, type<x2>, type<x3>)
            ));
        }
    }

    // Iterable
    {
        ///////////////////
        // tuple
        //////////////////
        // operators
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tuple(x<0>, x<1>)[int_<0>],
                x<0>
            ));
        }


        ///////////////////
        // tuple_t
        //////////////////
        // head
        {
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_t<x0>), type<x0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_t<x0, x1>), type<x0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_t<x0, x1, x2>), type<x0>));
        }

        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(is_empty(tuple_t<>));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_t<x0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_t<x0, x1>)));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(equal(tail(tuple_t<x0>), tuple_t<>));
            BOOST_HANA_CONSTANT_CHECK(equal(tail(tuple_t<x0, x1>), tuple_t<x1>));
            BOOST_HANA_CONSTANT_CHECK(equal(tail(tuple_t<x0, x1, x2>), tuple_t<x1, x2>));
        }

        //////////////////
        // tuple_c
        /////////////////

        // head
        {
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_c<int, 0>), int_<0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_c<int, 0, 1>), int_<0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_c<int, 0, 1, 2>), int_<0>));
        }

        // is_empty
        {
            BOOST_HANA_CONSTANT_CHECK(is_empty(tuple_c<int>));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0, 1>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0, 1, 2>)));
        }

        // tail
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_c<int, 0>),
                tuple_c<int>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_c<int, 0, 1>),
                tuple_c<int, 1>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_c<int, 0, 1, 2>),
                tuple_c<int, 1, 2>
            ));
        }
    }

    // MonadPlus
    {
        // prepend
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(integral_constant<int, 0>, tuple_c<long>),
                tuple_c<long, 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(integral_constant<int, 0>, tuple_c<unsigned int, 1>),
                tuple_c<unsigned int, 0, 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(integral_constant<long, 0>, tuple_c<long long, 1, 2>),
                tuple_c<long long, 0, 1, 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(integral_constant<unsigned long, 0>, tuple_c<unsigned long, 1, 2, 3>),
                tuple_c<unsigned long, 0, 1, 2, 3>
            ));
        }

        // nil
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                nil<IntegerList>(),
                tuple_c<int>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                nil<IntegerList>(),
                tuple_c<long>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                nil<IntegerList>(),
                tuple_c<void>
            ));
        }
    }
}
