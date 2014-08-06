/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    //! [main]
    BOOST_HANA_CONSTEXPR_LAMBDA auto xs = list(
        int_<1>, type<int>, int_<2>, type<float>, int_<3>, type<void>, type<char>
    );
    BOOST_HANA_CONSTANT_ASSERT(
        filter(xs, in ^ list(int_<3>, type<int>, type<void>))
        ==
        list(type<int>, int_<3>, type<void>)
    );
    //! [main]
}
