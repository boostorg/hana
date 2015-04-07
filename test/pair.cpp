/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/pair.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/orderable.hpp>
#include <laws/product.hpp>
using namespace boost::hana;


using test::ct_eq;
using test::ct_ord;

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Creation
    //////////////////////////////////////////////////////////////////////////
    {
        // Implicit and explicit construction
        {
            constexpr _pair<int, char> p1{1, '2'};      (void)p1;
            constexpr _pair<int, char> p2 = {1, '2'};   (void)p2;
        }

        // default constructibility
        {
            struct default_constr {
                default_constr() = default;
                default_constr(default_constr const&) = delete;
                default_constr(default_constr &&) = delete;
            };
            constexpr _pair<default_constr, default_constr> p; (void)p;
        }

        // pair with non default-constructible elements
        {
            struct no_default {
                no_default() = delete;
                constexpr no_default(int) { }
            };
            constexpr _pair<no_default, no_default> p{1, 1}; (void)p;
        }

        // pair with non-constexpr default-constructible elements
        {
            struct non_constexpr { non_constexpr() { } };
            _pair<non_constexpr, non_constexpr> p; (void)p;
        }

        // Make sure we do not instantiate wrong constructors when copying
        {
            _pair<test::trap_construct, int> expr{};
            auto implicit_copy = expr;          (void)implicit_copy;
            decltype(expr) explicit_copy(expr); (void)explicit_copy;
        }
    }

    auto eq_elems = make<Tuple>(ct_eq<3>{}, ct_eq<4>{});

    auto eqs = make<Tuple>(
          make_pair(ct_eq<3>{}, ct_eq<3>{})
        , make_pair(ct_eq<3>{}, ct_eq<4>{})
        , make_pair(ct_eq<4>{}, ct_eq<3>{})
        , make_pair(ct_eq<4>{}, ct_eq<4>{})
    );

    auto ords = make<Tuple>(
          make_pair(ct_ord<3>{}, ct_ord<3>{})
        , make_pair(ct_ord<3>{}, ct_ord<4>{})
        , make_pair(ct_ord<4>{}, ct_ord<3>{})
        , make_pair(ct_ord<4>{}, ct_ord<4>{})
    );

    //////////////////////////////////////////////////////////////////////////
    // make_pair
    //////////////////////////////////////////////////////////////////////////
    BOOST_HANA_CONSTANT_CHECK(equal(
        make<Pair>(ct_eq<1>{}, ct_eq<2>{}),
        make_pair(ct_eq<1>{}, ct_eq<2>{})
    ));

    //////////////////////////////////////////////////////////////////////////
    // Comparable, Orderable, Foldable
    //////////////////////////////////////////////////////////////////////////
    test::TestComparable<Pair>{eqs};
    test::TestOrderable<Pair>{ords};
    test::TestFoldable<Pair>{eqs};

    //////////////////////////////////////////////////////////////////////////
    // Product
    //////////////////////////////////////////////////////////////////////////
    {
        // first
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                first(make_pair(ct_eq<1>{}, ct_eq<2>{})),
                ct_eq<1>{}
            ));
        }

        // second
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                second(make_pair(ct_eq<1>{}, ct_eq<2>{})),
                ct_eq<2>{}
            ));
        }

        // make
        {
            constexpr _pair<int, char> p = make<Pair>(1, 'x');
            static_assert(first(p) == 1, "");
            static_assert(second(p) == 'x', "");
        }

        // laws
        test::TestProduct<Pair>{eq_elems};
    }
}
