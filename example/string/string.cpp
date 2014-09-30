/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/string.hpp>
using namespace boost::hana;


int main() {
    {
        //! [macro]
        BOOST_HANA_CONSTEXPR_LAMBDA auto s = BOOST_HANA_STRING("abcdef");
        //! [macro]
        (void)s;
    }
}
