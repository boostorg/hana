/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>

#include "minimal.hpp"
using namespace boost::hana;


struct EqClass;
template <int equivalence_class> struct X { using hana_datatype = EqClass; };
template <int equivalence_class> struct Y { using hana_datatype = EqClass; };
template <int equivalence_class> constexpr X<equivalence_class> x{};
template <int equivalence_class> constexpr Y<equivalence_class> y{};

namespace boost { namespace hana {
    template <>
    struct Comparable::instance<EqClass, EqClass> : Comparable::equal_mcd {
        template <typename T, typename U>
        static constexpr auto equal_impl(T, U)
        { return type<T> == type<U>; }
    };
}}

constexpr struct _predicate {
    template <int i, int j, template <int> class X, template <int> class Y>
    constexpr auto operator()(X<i>, Y<j>) const
    { return bool_<(i < j)>; }
} pred{};

BOOST_HANA_CONSTEXPR_LAMBDA auto check = [](auto sorted_list) {
    auto perms = fmap(partial(sort_by, pred), permutations(sorted_list));
    BOOST_HANA_STATIC_ASSERT(all(_ == sorted_list, perms));
};

int main() {
    check(minimal_list());
    check(minimal_list(x<1>));
    check(minimal_list(x<1>, x<2>));
    check(minimal_list(x<1>, x<2>, x<3>));

    // check stability
    BOOST_HANA_STATIC_ASSERT(sort_by(pred, minimal_list(x<1>, y<1>)) == minimal_list(x<1>, y<1>));
    BOOST_HANA_STATIC_ASSERT(sort_by(pred, minimal_list(y<1>, x<1>)) == minimal_list(y<1>, x<1>));

    BOOST_HANA_STATIC_ASSERT(sort_by(pred, minimal_list(x<1>, y<1>, x<2>, y<2>)) == minimal_list(x<1>, y<1>, x<2>, y<2>));
    BOOST_HANA_STATIC_ASSERT(sort_by(pred, minimal_list(x<1>, x<2>, y<1>, y<2>)) == minimal_list(x<1>, y<1>, x<2>, y<2>));
    BOOST_HANA_STATIC_ASSERT(sort_by(pred, minimal_list(y<1>, x<1>, x<2>, y<2>)) == minimal_list(y<1>, x<1>, x<2>, y<2>));
    BOOST_HANA_STATIC_ASSERT(sort_by(pred, minimal_list(x<2>, y<1>, y<2>, x<1>)) == minimal_list(y<1>, x<1>, x<2>, y<2>));

    BOOST_HANA_STATIC_ASSERT(sort_by(pred, minimal_list(x<1>, x<3>, y<1>, x<2>, y<3>)) == minimal_list(x<1>, y<1>, x<2>, x<3>, y<3>));
}
