/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/constant.hpp>
#include <laws/enumerable.hpp>
#include <laws/group.hpp>
#include <laws/integral_domain.hpp>
#include <laws/logical.hpp>
#include <laws/monoid.hpp>
#include <laws/orderable.hpp>
#include <laws/ring.hpp>

#include <type_traits>
using namespace boost::hana;


struct inherit_simple : std::integral_constant<int, 3> { };
struct inherit_no_default : std::integral_constant<int, 3> {
    inherit_no_default() = delete;
};

struct incomplete;
struct empty_type { };
struct non_pod { virtual ~non_pod() { } };

int main() {
    auto ints = make<Tuple>(
        std::integral_constant<int, -10>{},
        std::integral_constant<int, -2>{},
        std::integral_constant<int, 0>{},
        std::integral_constant<int, 1>{},
        std::integral_constant<int, 3>{}
    );
    (void)ints;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Make sure the data type is detected properly
    //////////////////////////////////////////////////////////////////////////
    {
        static_assert(std::is_same<
            datatype_t<inherit_simple>,
            ext::std::IntegralConstant<int>
        >{}, "");
        static_assert(std::is_same<
            datatype_t<inherit_no_default>,
            ext::std::IntegralConstant<int>
        >{}, "");
        static_assert(std::is_same<
            datatype_t<std::is_pointer<int*>>,
            ext::std::IntegralConstant<bool>
        >{}, "");

        static_assert(!std::is_same<
            datatype_t<incomplete>,
            ext::std::IntegralConstant<int>
        >{}, "");
        static_assert(!std::is_same<
            datatype_t<empty_type>,
            ext::std::IntegralConstant<int>
        >{}, "");
        static_assert(!std::is_same<
            datatype_t<non_pod>,
            ext::std::IntegralConstant<int>
        >{}, "");
        static_assert(!std::is_same<
            datatype_t<void>,
            ext::std::IntegralConstant<int>
        >{}, "");
    }

    //////////////////////////////////////////////////////////////////////////
    // Interoperation with hana::integral_constant
    //////////////////////////////////////////////////////////////////////////
    {
        BOOST_HANA_CONSTANT_CHECK(std::integral_constant<int, 1>{} == int_<1>);
        BOOST_HANA_CONSTANT_CHECK(std::integral_constant<int, 1>{} == long_<1>);

        BOOST_HANA_CONSTANT_CHECK(std::integral_constant<int, 2>{} != int_<3>);
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Constant
    //////////////////////////////////////////////////////////////////////////
    {
        // value
        static_assert(value(std::integral_constant<int, 0>{}) == 0, "");
        static_assert(value(std::integral_constant<int, 1>{}) == 1, "");
        static_assert(value(std::integral_constant<int, 3>{}) == 3, "");

        // laws
        test::TestConstant<ext::std::IntegralConstant<int>>{ints, tuple_t<int, long, long long>};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Enumerable, Monoid, Group, Ring, IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestEnumerable<ext::std::IntegralConstant<int>>{ints};
        test::TestMonoid<ext::std::IntegralConstant<int>>{ints};
        test::TestGroup<ext::std::IntegralConstant<int>>{ints};
        test::TestRing<ext::std::IntegralConstant<int>>{ints};
        test::TestIntegralDomain<ext::std::IntegralConstant<int>>{ints};
    }

#elif BOOST_HANA_TEST_PART == 4
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

        auto ints = make<Tuple>(
            std::integral_constant<int, -2>{},
            std::integral_constant<int, 0>{},
            std::integral_constant<int, 1>{},
            std::integral_constant<int, 3>{}
        );

        auto bools = make<Tuple>(std::true_type{}, std::false_type{});

        // laws
        test::TestLogical<ext::std::IntegralConstant<int>>{ints};
        test::TestLogical<ext::std::IntegralConstant<bool>>{bools};
    }

#elif BOOST_HANA_TEST_PART == 5
    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    test::TestComparable<ext::std::IntegralConstant<int>>{ints};

#elif BOOST_HANA_TEST_PART == 6
    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    test::TestOrderable<ext::std::IntegralConstant<int>>{ints};
#endif
}
