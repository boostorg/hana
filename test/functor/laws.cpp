/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functor.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/laws.hpp>
#include <boost/hana/detail/minimal/functor.hpp>
#include <boost/hana/detail/static_assert.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) { return x + 1; };
BOOST_HANA_CONSTEXPR_LAMBDA auto g = [](auto x) { return x * 3; };
constexpr auto x = 1;

template <typename mcd>
void test() {
    constexpr auto functor = detail::minimal::functor<mcd>;
    BOOST_HANA_STATIC_ASSERT(detail::laws<Functor>(functor(x), f, g));
}

int main() {
    test<Functor::fmap_mcd>();
    test<Functor::adjust_mcd>();
}
