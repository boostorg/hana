/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <type_traits>
using namespace boost::hana;


struct f { template <typename ...> struct apply { struct type; }; };
struct x1; struct x2; struct x3;

int main() {
    BOOST_HANA_STATIC_ASSERT(metafunction_class<f>() == type<f::apply<>::type>);
    BOOST_HANA_STATIC_ASSERT(metafunction_class<f>(type<x1>) == type<f::apply<x1>::type>);
    BOOST_HANA_STATIC_ASSERT(metafunction_class<f>(type<x1>, type<x2>) == type<f::apply<x1, x2>::type>);
    BOOST_HANA_STATIC_ASSERT(metafunction_class<f>(type<x1>, type<x2>, type<x3>) == type<f::apply<x1, x2, x3>::type>);

    static_assert(std::is_same<decltype(metafunction_class<f>)::apply<>, f::apply<>>::value, "");
    static_assert(std::is_same<decltype(metafunction_class<f>)::apply<x1>, f::apply<x1>>::value, "");
    static_assert(std::is_same<decltype(metafunction_class<f>)::apply<x1, x2>, f::apply<x1, x2>>::value, "");
    static_assert(std::is_same<decltype(metafunction_class<f>)::apply<x1, x2, x3>, f::apply<x1, x2, x3>>::value, "");
}
