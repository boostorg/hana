/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/trait.hpp>
#include <boost/hana/type.hpp>

#include <cassert>
#include <sstream>
#include <string>
using namespace boost::hana;


int main() {
    //! [fusion]
    auto to_string = [](auto x) { return (std::ostringstream{} << x).str(); };

    auto show = [=](auto x, auto y) {
        return "(" + to_string(x) + " + " + to_string(y) + ")";
    };

    assert(foldl(show, "1", list(2, "3", '4')) == "(((1 + 2) + 3) + 4)");
    //! [fusion]

    //! [mpl]
    BOOST_HANA_CONSTEXPR_LAMBDA auto types = list_t<long, float, short, double, float, long, long double>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto count_if_float = [](auto c, auto t) {
        return if_(trait::is_floating_point(t), c + int_<1>, c);
    };

    BOOST_HANA_STATIC_ASSERT(foldl(count_if_float, int_<0>, types) == int_<4>);
    //! [mpl]
}
