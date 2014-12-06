/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


int main() {
    {
        //! [comparable]
        BOOST_HANA_CONSTANT_CHECK(
            set(int_<0>, type<char>, int_<1>) == set(int_<1>, int_<0>, type<char>)
        );

        BOOST_HANA_CONSTEXPR_CHECK(set(1, '2', 3.3) == set('2', 1, 3.3));
        BOOST_HANA_CONSTANT_CHECK(set(1, '2', 3.3) != set('2', 1));
        //! [comparable]
    }

    {
        //! [searchable]
        BOOST_HANA_CONSTEXPR_LAMBDA auto xs = set(int_<0>, int_<1>, int_<2>);
        BOOST_HANA_CONSTANT_CHECK(lookup(xs, int_<0>) == just(int_<0>));
        BOOST_HANA_CONSTANT_CHECK(lookup(xs, int_<3>) == nothing);
        //! [searchable]
    }

    {
        //! [insert]
        BOOST_HANA_CONSTEXPR_LAMBDA auto xs = set(int_<0>, type<int>);
        BOOST_HANA_CONSTANT_CHECK(
            insert(xs, BOOST_HANA_STRING("abc")) ==
            set(int_<0>, type<int>, BOOST_HANA_STRING("abc"))
        );

        BOOST_HANA_CONSTANT_CHECK(
            insert(xs, int_<0>) == set(int_<0>, type<int>)
        );
        //! [insert]
    }
}
