/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/sandbox/detail/is_valid.hpp>

#include <type_traits>
using namespace boost::hana;


struct x1; struct x2; struct x3;

template <typename f>
void test() {
    BOOST_HANA_STATIC_ASSERT(metafunction_class<f>() == type<typename f::template apply<>::type>);
    BOOST_HANA_STATIC_ASSERT(metafunction_class<f>(type<x1>) == type<typename f::template apply<x1>::type>);
    BOOST_HANA_STATIC_ASSERT(metafunction_class<f>(type<x1>, type<x2>) == type<typename f::template apply<x1, x2>::type>);
    BOOST_HANA_STATIC_ASSERT(metafunction_class<f>(type<x1>, type<x2>, type<x3>) == type<typename f::template apply<x1, x2, x3>::type>);

    using F = decltype(metafunction_class<f>);
    static_assert(std::is_same<typename F::template apply<>, typename f::template apply<>>::value, "");
    static_assert(std::is_same<typename F::template apply<x1>, typename f::template apply<x1>>::value, "");
    static_assert(std::is_same<typename F::template apply<x1, x2>, typename f::template apply<x1, x2>>::value, "");
    static_assert(std::is_same<typename F::template apply<x1, x2, x3>, typename f::template apply<x1, x2, x3>>::value, "");
}

struct f { template <typename ...> struct apply { struct type; }; };
struct invalid;

struct alias {
    template <typename x = void, typename y = void, typename z = void>
    using apply = f::apply<x, y, z>;
};

int main() {
    test<f>();
    test<alias>();

    // make sure it's SFINAE friendly
    BOOST_HANA_STATIC_ASSERT(!detail::is_valid(metafunction_class<invalid>)(type<x1>));
}
