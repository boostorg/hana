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

#include <cassert>
#include <sstream>
#include <string>
using namespace boost::hana;


int main() {
    //! [fusion]
    auto make_string = [](auto x, std::string str) {
        return str + (std::ostringstream{} << x).str();
    };

    assert(foldr(make_string, "4", list(1, "2", '3')) == "4321");
    //! [fusion]

    //! [mpl]
    BOOST_HANA_CONSTEXPR_LAMBDA auto numbers = list_c<int, 5, -1, 0, -7, -2, 0, -5, 4>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto negatives = list_c<int, -1, -7, -2, -5>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto keep_negatives = [](auto n, auto acc) {
        return if_(n < int_<0>, cons(n, acc), acc);
    };

    BOOST_HANA_STATIC_ASSERT(foldr(keep_negatives, list_c<int>, numbers) == negatives);
    //! [mpl]
}
