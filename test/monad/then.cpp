/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/monad/bind_mcd.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
#include <boost/hana/detail/minimal/monad.hpp>
using namespace boost::hana;


template <int i>
constexpr auto x = detail::minimal::comparable<>(i);
struct invalid { };

template <template <typename ...> class mcd>
void test() {
    constexpr auto monad = detail::minimal::monad<mcd>;

    BOOST_HANA_CONSTEXPR_ASSERT(then(monad(invalid{}), monad(x<1>)) == monad(x<1>));
}

int main() {
    test<Monad::flatten_mcd>();
    test<Monad::bind_mcd>();
}
