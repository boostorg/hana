/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/monad/bind_mcd.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/monad.hpp>

#include <tuple>
using namespace boost::hana;


template <template <typename ...> class mcd>
void test() {
    constexpr auto monad = detail::minimal::monad<mcd>;
    constexpr auto comparable = detail::minimal::comparable<>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = [=](auto x) {
        return monad(std::make_tuple(x));
    };

    BOOST_HANA_CONSTEXPR_ASSERT(bind(monad(comparable(1)), f) == f(comparable(1)));
    BOOST_HANA_CONSTEXPR_ASSERT((monad(comparable(1)) | f) == f(comparable(1)));

    // check associativity
    BOOST_HANA_CONSTEXPR_ASSERT((monad(comparable(1)) | f | f) == f(std::make_tuple(comparable(1))));
}

int main() {
    test<Monad::flatten_mcd>();
    test<Monad::bind_mcd>();
}
