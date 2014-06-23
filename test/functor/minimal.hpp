/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_FUNCTOR_MINIMAL_HPP
#define BOOST_HANA_TEST_FUNCTOR_MINIMAL_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/laws.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>


struct MinimalFunctor;

template <typename Storage>
struct _functor {
    using hana_datatype = MinimalFunctor;
    Storage storage;

    // Required to check Functor laws.
    template <typename S1, typename S2>
    friend constexpr auto operator==(_functor<S1> a, _functor<S2> b)
    { return a.storage(boost::hana::list) == b.storage(boost::hana::list); }
};

BOOST_HANA_CONSTEXPR_LAMBDA auto functor = [](auto ...xs) {
    auto storage = [=](auto f) { return f(xs...); };
    return _functor<decltype(storage)>{storage};
};

auto FunctorTest = [](auto functor) {
    using namespace boost::hana;
    // laws
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return x + int_<1>; };
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) { return x * int_<3>; };
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(functor(), f, g));
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(functor(int_<1>), f, g));
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(functor(int_<1>, int_<2>), f, g));
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(functor(int_<1>, int_<2>, int_<3>), f, g));
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(functor(1), f, g));
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(functor(1, 2), f, g));
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(functor(1, 2, 3), f, g));


    BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
        return x % int_<2> != int_<0>;
    };
    BOOST_HANA_CONSTEXPR_LAMBDA auto negate = [](auto x) {
        return -x;
    };

    // adjust
    BOOST_HANA_STATIC_ASSERT(adjust(odd, negate, functor()) == functor());
    BOOST_HANA_STATIC_ASSERT(adjust(odd, negate, functor(int_<0>)) == functor(int_<0>));
    BOOST_HANA_STATIC_ASSERT(adjust(odd, negate, functor(int_<0>, int_<1>)) == functor(int_<0>, int_<-1>));
    BOOST_HANA_STATIC_ASSERT(adjust(odd, negate, functor(int_<0>, int_<1>, int_<2>)) == functor(int_<0>, int_<-1>, int_<2>));
    BOOST_HANA_STATIC_ASSERT(adjust(odd, negate, functor(0)) == functor(0));
    BOOST_HANA_STATIC_ASSERT(adjust(odd, negate, functor(0, 1)) == functor(0, -1));
    BOOST_HANA_STATIC_ASSERT(adjust(odd, negate, functor(0, 1, 2)) == functor(0, -1, 2));


    // replace
    BOOST_HANA_STATIC_ASSERT(replace(odd, int_<999>, functor()) == functor());
    BOOST_HANA_STATIC_ASSERT(replace(odd, int_<999>, functor(int_<0>)) == functor(int_<0>));
    BOOST_HANA_STATIC_ASSERT(replace(odd, int_<999>, functor(int_<0>, int_<1>)) == functor(int_<0>, int_<999>));
    BOOST_HANA_STATIC_ASSERT(replace(odd, int_<999>, functor(int_<0>, int_<1>, int_<2>)) == functor(int_<0>, int_<999>, int_<2>));
    BOOST_HANA_STATIC_ASSERT(replace(odd, 999, functor(0)) == functor(0));
    BOOST_HANA_STATIC_ASSERT(replace(odd, 999, functor(0, 1)) == functor(0, 999));
    BOOST_HANA_STATIC_ASSERT(replace(odd, 999, functor(0, 1, 2)) == functor(0, 999, 2));


    // fmap
    BOOST_HANA_STATIC_ASSERT(fmap(f, functor()) == functor());
    BOOST_HANA_STATIC_ASSERT(fmap(f, functor(int_<0>)) == functor(f(int_<0>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, functor(int_<0>, int_<1>)) == functor(f(int_<0>), f(int_<1>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, functor(int_<0>, int_<1>, int_<2>)) == functor(f(int_<0>), f(int_<1>), f(int_<2>)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, functor(0)) == functor(f(0)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, functor(0, 1)) == functor(f(0), f(1)));
    BOOST_HANA_STATIC_ASSERT(fmap(f, functor(0, 1, 2)) == functor(f(0), f(1), f(2)));
};

#endif // !BOOST_HANA_TEST_FUNCTOR_MINIMAL_HPP
