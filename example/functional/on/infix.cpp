/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
//! [main]
constexpr auto sorted = sort.by(less ^on^ first, make<Tuple>(
    pair(int_<3>, 'x'),
    pair(int_<1>, type<void>),
    pair(int_<2>, 9876)
));

static_assert(sorted == make<Tuple>(
    pair(int_<1>, type<void>),
    pair(int_<2>, 9876),
    pair(int_<3>, 'x')
), "");
//! [main]
}
