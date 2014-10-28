/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/sandbox/detail/is_valid.hpp>

#include <type_traits>
using namespace boost::hana;


struct x1; struct x2; struct x3;

template <template <typename ...> class f>
void test() {
    BOOST_HANA_CONSTANT_ASSERT(metafunction<f>() == type<typename f<>::type>);
    BOOST_HANA_CONSTANT_ASSERT(metafunction<f>(type<x1>) == type<typename f<x1>::type>);
    BOOST_HANA_CONSTANT_ASSERT(metafunction<f>(type<x1>, type<x2>) == type<typename f<x1, x2>::type>);
    BOOST_HANA_CONSTANT_ASSERT(metafunction<f>(type<x1>, type<x2>, type<x3>) == type<typename f<x1, x2, x3>::type>);

    using F = decltype(metafunction<f>);
    static_assert(std::is_same<typename F::template apply<>, f<>>::value, "");
    static_assert(std::is_same<typename F::template apply<x1>, f<x1>>::value, "");
    static_assert(std::is_same<typename F::template apply<x1, x2>, f<x1, x2>>::value, "");
    static_assert(std::is_same<typename F::template apply<x1, x2, x3>, f<x1, x2, x3>>::value, "");
}

template <typename ...> struct f { struct type; };
template <typename ...> struct invalid;

template <typename x = void, typename y = void, typename z = void>
using alias = f<x, y, z>;

int main() {
    test<f>();

    // BOOST_HANA_CONSTANT_ASSERT(!detail::is_valid(metafunction<invalid>)(type<x1>));
    // test<alias>();
}
