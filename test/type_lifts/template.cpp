/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/sandbox/detail/is_valid.hpp>

#include <type_traits>
using namespace boost::hana;


struct x1; struct x2; struct x3;

template <template <typename ...> class f>
void test() {
    BOOST_HANA_CONSTANT_CHECK(template_<f>() == type<f<>>);
    BOOST_HANA_CONSTANT_CHECK(template_<f>(type<x1>) == type<f<x1>>);
    BOOST_HANA_CONSTANT_CHECK(template_<f>(type<x1>, type<x2>) == type<f<x1, x2>>);
    BOOST_HANA_CONSTANT_CHECK(template_<f>(type<x1>, type<x2>, type<x3>) == type<f<x1, x2, x3>>);

    using F = decltype(template_<f>);
    static_assert(std::is_same<typename F::template apply<>::type, f<>>::value, "");
    static_assert(std::is_same<typename F::template apply<x1>::type, f<x1>>::value, "");
    static_assert(std::is_same<typename F::template apply<x1, x2>::type, f<x1, x2>>::value, "");
    static_assert(std::is_same<typename F::template apply<x1, x2, x3>::type, f<x1, x2, x3>>::value, "");
}

template <typename ...> struct f;

template <typename ...x>
using invalid = typename f<x...>::this_must_not_be_instantiated;

template <typename x = void, typename y = void, typename z = void>
using alias = f<x, y, z>;

int main() {
    test<f>();

    // BOOST_HANA_CONSTANT_CHECK(!detail::is_valid(template_<invalid>)(type<x1>));
    // test<alias>();
}
