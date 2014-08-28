/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integer_list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>

#include <sstream>
#include <string>
using namespace boost::hana;


int main() {
    {
        //! [foldl1]
        auto to_string = [](auto x) {
            return (std::ostringstream{} << x).str();
        };

        auto show = [=](auto x, auto y) {
            return "(" + to_string(x) + " + " + to_string(y) + ")";
        };

        BOOST_HANA_RUNTIME_ASSERT(foldl1(tuple(1, "2", '3'), show) == "((1 + 2) + 3)");
        //! [foldl1]
    }

    {
        //! [foldl]
        auto to_string = [](auto x) {
            return (std::ostringstream{} << x).str();
        };

        auto show = [=](auto x, auto y) {
            return "(" + to_string(x) + " + " + to_string(y) + ")";
        };

        BOOST_HANA_RUNTIME_ASSERT(foldl(tuple(2, "3", '4'), "1", show) == "(((1 + 2) + 3) + 4)");
        //! [foldl]
    }


    {
        //! [foldr]
        auto to_string = [](auto x) {
            return (std::ostringstream{} << x).str();
        };

        auto show = [=](auto x, auto y) {
            return "(" + to_string(x) + " + " + to_string(y) + ")";
        };

        BOOST_HANA_RUNTIME_ASSERT(foldr(tuple(1, "2", '3'), "4", show) == "(1 + (2 + (3 + 4)))");
        //! [foldr]
    }

    {
        //! [foldr1]
        auto to_string = [](auto x) {
            return (std::ostringstream{} << x).str();
        };

        auto show = [=](auto x, auto y) {
            return "(" + to_string(x) + " + " + to_string(y) + ")";
        };

        BOOST_HANA_RUNTIME_ASSERT(foldr1(tuple(1, "2", '3'), show) == "(1 + (2 + 3))");
        //! [foldr1]
    }

    {
        //! [for_each]
        std::stringstream ss;
        for_each(tuple(0, '1', "234", 5.5), [&](auto x) {
            ss << x << ' ';
        });

        BOOST_HANA_RUNTIME_ASSERT(ss.str() == "0 1 234 5.5 ");
        //! [for_each]
    }

    {
        //! [length]
        BOOST_HANA_CONSTANT_ASSERT(length(tuple()) == int_<0>);
        BOOST_HANA_CONSTANT_ASSERT(length(tuple(1, '2', 3.0)) == int_<3>);

        BOOST_HANA_CONSTANT_ASSERT(length(nothing) == int_<0>);
        BOOST_HANA_CONSTANT_ASSERT(length(just('x')) == int_<1>);
        //! [length]
    }

    {
        //! [maximum]
        BOOST_HANA_CONSTANT_ASSERT(
            maximum(integer_list<int, -1, 0, 2, -4, 6, 9>) == int_<9>
        );
        //! [maximum]
    }

    {
        //! [minimum]
        BOOST_HANA_CONSTANT_ASSERT(
            minimum(integer_list<int, -1, 0, 2, -4, 6, 9>) == int_<-4>
        );
        //! [minimum]
    }

    {
        //! [product]
        BOOST_HANA_CONSTANT_ASSERT(
            product(range(int_<1>, int_<6>)) == int_<1 * 2 * 3 * 4 * 5>
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            product(tuple(1, int_<3>, long_<-5>, 9)) == 1 * 3 * -5 * 9
        );
        //! [product]
    }

    {
        //! [sum]
        BOOST_HANA_CONSTANT_ASSERT(
            sum(range(int_<1>, int_<6>)) == int_<1 + 2 + 3 + 4 + 5>
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            sum(tuple(1, int_<3>, long_<-5>, 9)) == 1 + 3 - 5 + 9
        );
        //! [sum]
    }
}
