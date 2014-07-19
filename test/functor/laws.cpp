/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functor/adjust_mcd.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/functor/laws.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/functor.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>

#include <tuple>
using namespace boost::hana;


template <int i>
constexpr auto x = int_<i>;

BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return std::make_tuple(1, x);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) {
    return std::make_tuple(2, x);
};

template <typename mcd>
void test() {
    constexpr auto functor = detail::minimal::functor<mcd>;

    BOOST_HANA_STATIC_ASSERT(Functor::laws::check(
        list(functor(x<0>)),
        list(f),
        list(g)
    ));
}

int main() {
    test<Functor::fmap_mcd>();
    test<Functor::adjust_mcd>();
}
