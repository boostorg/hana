/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/assert.hpp>

#include <type_traits>
using namespace boost::hana;


struct x1; struct x2; struct x3;

// metafunction
namespace tc1 {
    template <typename ...> struct f { struct type; };
    using F = decltype(metafunction<f>);
    BOOST_HANA_CONSTANT_CHECK(metafunction<f>() == type<f<>::type>);
    BOOST_HANA_CONSTANT_CHECK(metafunction<f>(type<x1>) == type<f<x1>::type>);
    BOOST_HANA_CONSTANT_CHECK(metafunction<f>(type<x1>, type<x2>) == type<f<x1, x2>::type>);
    BOOST_HANA_CONSTANT_CHECK(metafunction<f>(type<x1>, type<x2>, type<x3>) == type<f<x1, x2, x3>::type>);
    static_assert(std::is_same<F::apply<>, f<>>::value, "");
    static_assert(std::is_same<F::apply<x1>, f<x1>>::value, "");
    static_assert(std::is_same<F::apply<x1, x2>, f<x1, x2>>::value, "");
    static_assert(std::is_same<F::apply<x1, x2, x3>, f<x1, x2, x3>>::value, "");
}

// metafunction_class
namespace tc2 {
    struct f { template <typename ...> struct apply { struct type; }; };
    using F = decltype(metafunction_class<f>);
    BOOST_HANA_CONSTANT_CHECK(metafunction_class<f>() == type<f::apply<>::type>);
    BOOST_HANA_CONSTANT_CHECK(metafunction_class<f>(type<x1>) == type<f::apply<x1>::type>);
    BOOST_HANA_CONSTANT_CHECK(metafunction_class<f>(type<x1>, type<x2>) == type<f::apply<x1, x2>::type>);
    BOOST_HANA_CONSTANT_CHECK(metafunction_class<f>(type<x1>, type<x2>, type<x3>) == type<f::apply<x1, x2, x3>::type>);
    static_assert(std::is_same<F::apply<>, f::apply<>>::value, "");
    static_assert(std::is_same<F::apply<x1>, f::apply<x1>>::value, "");
    static_assert(std::is_same<F::apply<x1, x2>, f::apply<x1, x2>>::value, "");
    static_assert(std::is_same<F::apply<x1, x2, x3>, f::apply<x1, x2, x3>>::value, "");
}

// template_
namespace tc3 {
    template <typename ...> struct f;
    using F = decltype(template_<f>);
    BOOST_HANA_CONSTANT_CHECK(template_<f>() == type<f<>>);
    BOOST_HANA_CONSTANT_CHECK(template_<f>(type<x1>) == type<f<x1>>);
    BOOST_HANA_CONSTANT_CHECK(template_<f>(type<x1>, type<x2>) == type<f<x1, x2>>);
    BOOST_HANA_CONSTANT_CHECK(template_<f>(type<x1>, type<x2>, type<x3>) == type<f<x1, x2, x3>>);
    static_assert(std::is_same<F::apply<>::type, f<>>::value, "");
    static_assert(std::is_same<F::apply<x1>::type, f<x1>>::value, "");
    static_assert(std::is_same<F::apply<x1, x2>::type, f<x1, x2>>::value, "");
    static_assert(std::is_same<F::apply<x1, x2, x3>::type, f<x1, x2, x3>>::value, "");
}

// trait
namespace tc4 {
    template <typename ...> struct f { };

    // make sure `trait<f>(...)` returns the right type
    static_assert(std::is_same<decltype(trait<f>()), f<>>{}, "");
    static_assert(std::is_same<decltype(trait<f>(type<x1>)), f<x1>>{}, "");
    static_assert(std::is_same<decltype(trait<f>(type<x1>, type<x2>)), f<x1, x2>>{}, "");
    static_assert(std::is_same<decltype(trait<f>(type<x1>, type<x2>, type<x3>)), f<x1, x2, x3>>{}, "");

    // make sure we can perform the call; we already made sure the return type was correct
    constexpr auto a = trait<f>();
    constexpr auto b = trait<f>(type<x1>);
    constexpr auto c = trait<f>(type<x1>, type<x2>);
    constexpr auto d = trait<f>(type<x1>, type<x2>, type<x3>);
}

// trait_
namespace tc5 {
    struct x1 { }; struct x2 { }; struct x3 { };
    template <typename ...> struct f { };

    // make sure `trait_<f>(...)` returns the right type
    static_assert(std::is_same<decltype(trait_<f>()), f<>>{}, "");
    static_assert(std::is_same<decltype(trait_<f>(x1{})), f<x1>>{}, "");
    static_assert(std::is_same<decltype(trait_<f>(x1{}, x2{})), f<x1, x2>>{}, "");
    static_assert(std::is_same<decltype(trait_<f>(x1{}, x2{}, x3{})), f<x1, x2, x3>>{}, "");

    // make sure we can perform the call; we already made sure the return type was correct
    constexpr auto a = trait_<f>();
    constexpr auto b = trait_<f>(x1{});
    constexpr auto c = trait_<f>(x1{}, x2{});
    constexpr auto d = trait_<f>(x1{}, x2{}, x3{});
}


int main() {

}
