/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>

#include "matrix/comparable.hpp"
using namespace boost::hana;
using namespace cppcon;


int main() {
    // transpose
    {
        auto m = matrix(
            row(1, 2.2, '3'),
            row(4, '5', 6)
        );

        BOOST_HANA_CONSTEXPR_CHECK(transpose(m) ==
            matrix(
                row(1, 4),
                row(2.2, '5'),
                row('3', 6)
            )
        );
    }

    // vector
    {
        auto v = vector(1, '2', int_<3>, 4.2f);
        BOOST_HANA_CONSTEXPR_CHECK(v.size() == 4ul);
        BOOST_HANA_CONSTEXPR_CHECK(v.nrows() == 4ul);
        BOOST_HANA_CONSTEXPR_CHECK(v.ncolumns() == 1ul);
    }

    // matrix.at
    {
        auto m = matrix(
            row(1, '2', 3),
            row('4', char_<'5'>, 6),
            row(int_<7>, '8', 9.3)
        );
        BOOST_HANA_CONSTEXPR_CHECK(m.at(int_<0>, int_<0>) == 1);
        BOOST_HANA_CONSTEXPR_CHECK(m.at(int_<0>, int_<1>) == '2');
        BOOST_HANA_CONSTEXPR_CHECK(m.at(int_<0>, int_<2>) == 3);

        BOOST_HANA_CONSTEXPR_CHECK(m.at(int_<1>, int_<0>) == '4');
        BOOST_HANA_CONSTANT_CHECK(m.at(int_<1>, int_<1>) == char_<'5'>);
        BOOST_HANA_CONSTEXPR_CHECK(m.at(int_<1>, int_<2>) == 6);

        BOOST_HANA_CONSTANT_CHECK(m.at(int_<2>, int_<0>) == int_<7>);
        BOOST_HANA_CONSTEXPR_CHECK(m.at(int_<2>, int_<1>) == '8');
        BOOST_HANA_CONSTEXPR_CHECK(m.at(int_<2>, int_<2>) == 9.3);
    }

    // size, ncolumns, nrows
    {
        auto m = matrix(
            row(1, '2', 3),
            row('4', char_<'5'>, 6)
        );
        BOOST_HANA_CONSTEXPR_CHECK(m.size() == 6ul);
        BOOST_HANA_CONSTEXPR_CHECK(m.ncolumns() == 3ul);
        BOOST_HANA_CONSTEXPR_CHECK(m.nrows() == 2ul);
    }
}
