/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/list/instance.hpp>

#include <type_traits>
using namespace boost::hana;


//! [main]
template <unsigned i>
struct storage {
    char c[i];
};

int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto size = [](auto x, auto y) {
        return bool_<(sizeof(x) < sizeof(y))>;
    };

    auto min = minimum_by(size, list(storage<1>{}, storage<3>{}, storage<2>{}));
    static_assert(std::is_same<decltype(min), storage<1>>::value, "");
}
//! [main]
