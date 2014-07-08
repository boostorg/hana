/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename ...> struct f;
struct x1; struct x2; struct x3;

int main() {
    BOOST_HANA_STATIC_ASSERT(template_<f>() == type<f<>>);
    BOOST_HANA_STATIC_ASSERT(template_<f>(type<x1>) == type<f<x1>>);
    BOOST_HANA_STATIC_ASSERT(template_<f>(type<x1>, type<x2>) == type<f<x1, x2>>);
    BOOST_HANA_STATIC_ASSERT(template_<f>(type<x1>, type<x2>, type<x3>) == type<f<x1, x2, x3>>);

    static_assert(std::is_same<decltype(template_<f>)::apply<>::type, f<>>::value, "");
    static_assert(std::is_same<decltype(template_<f>)::apply<x1>::type, f<x1>>::value, "");
    static_assert(std::is_same<decltype(template_<f>)::apply<x1, x2>::type, f<x1, x2>>::value, "");
    static_assert(std::is_same<decltype(template_<f>)::apply<x1, x2, x3>::type, f<x1, x2, x3>>::value, "");
}
