/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <boost/hana/integral_constant.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/iterable.hpp>
#include <laws/orderable.hpp>
#include <laws/sequence.hpp>
#include <laws/traversable.hpp>

#include <type_traits>
#include <utility>
using namespace boost::hana;


template <int i>
using eq = test::ct_eq<i>;

template <int i>
using ord = test::ct_ord<i>;

struct x0; struct x1; struct x2; struct x3;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Move-only friendliness and reference semantics
    //////////////////////////////////////////////////////////////////////////
    {
        struct movable {
            movable() = default;
            movable(movable const&) = delete;
            movable(movable&&) = default;
        };

        {
            auto xs = make<Tuple>(movable{});
            auto by_val = [](auto) { };

            by_val(std::move(xs));
            by_val(head(std::move(xs)));
            by_val(at_c<0>(std::move(xs)));
            by_val(last(std::move(xs)));
        }

        {
            auto const& xs = make<Tuple>(movable{});
            auto by_const_ref = [](auto const&) { };

            by_const_ref(xs);
            by_const_ref(head(xs));
            by_const_ref(at_c<0>(xs));
            by_const_ref(last(xs));
        }

        {
            auto xs = make<Tuple>(movable{});
            auto by_ref = [](auto&) { };

            by_ref(xs);
            by_ref(head(xs));
            by_ref(at_c<0>(xs));
            by_ref(last(xs));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // default-constructibility
    //////////////////////////////////////////////////////////////////////////
    {
        _tuple<> z1{}; (void)z1;
        _tuple<int> z2{}; (void)z2;
        _tuple<int, char> z3{}; (void)z3;
        _tuple<int, char, float> z4{}; (void)z4;

        using Types = decltype(tuple_t<x0, x1>);
        Types default_{}; (void)default_;
    }

    //////////////////////////////////////////////////////////////////////////
    // `decltype(tuple_t<T...>)` should inherit `_tuple_t<T...>`
    //////////////////////////////////////////////////////////////////////////
    {
        static_assert(std::is_base_of<
            _tuple_t<x0, x1>, decltype(tuple_t<x0, x1>)
        >{}, "");
    }

    //////////////////////////////////////////////////////////////////////////
    // Setup for the laws below
    //////////////////////////////////////////////////////////////////////////
    auto eq_tuples = make<Tuple>(
          make<Tuple>()
        , make<Tuple>(eq<0>{})
        , make<Tuple>(eq<0>{}, eq<1>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}, eq<5>{})
    );

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        // unpack
        {
            test::_injection<0> f{};

            // tuple
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<Tuple>(), f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<Tuple>(eq<0>{}), f),
                f(eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<Tuple>(eq<0>{}, eq<1>{}), f),
                f(eq<0>{}, eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}), f),
                f(eq<0>{}, eq<1>{}, eq<2>{})
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

        test::TestFoldable<Tuple>{eq_tuples};
    }

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        // head
        {
            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_t<x0>), type<x0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_t<x0, x1>), type<x0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_t<x0, x1, x2>), type<x0>));

            // tuple_c
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_c<int, 0>), int_<0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_c<int, 0, 1>), int_<0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_c<int, 0, 1, 2>), int_<0>));
        }

        // is_empty
        {
            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(is_empty(tuple_t<>));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_t<x0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_t<x0, x1>)));

            // tuple_c
            BOOST_HANA_CONSTANT_CHECK(is_empty(tuple_c<int>));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0, 1>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0, 1, 2>)));
        }

        // tail
        {
            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(equal(tail(tuple_t<x0>), tuple_t<>));
            BOOST_HANA_CONSTANT_CHECK(equal(tail(tuple_t<x0, x1>), tuple_t<x1>));
            BOOST_HANA_CONSTANT_CHECK(equal(tail(tuple_t<x0, x1, x2>), tuple_t<x1, x2>));

            // tuple_c
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

        test::TestIterable<Tuple>{eq_tuples};
    }

    //////////////////////////////////////////////////////////////////////////
    // Traversable
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestTraversable<Tuple>{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestSequence<Tuple>{};
    }
}
