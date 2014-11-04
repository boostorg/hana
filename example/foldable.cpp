/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integer_list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#include <sstream>
#include <string>
#include <type_traits>
using namespace boost::hana;


int main() {
    {
        //! [foldl1]
        auto to_string = [](auto x) {
            return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
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
            return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
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
            return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
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
            return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
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

    {
        //! [unpack]
        auto cheap_tie = [](auto& ...vars) {
            return partial(flip(unpack), [&vars...](auto ...values) {
                // Using an initializer list sequences the assignments.
                int dummy[] = {((vars = values), 0)...};
                (void)dummy;
            });
        };
        int a = 0;
        char b = '\0';
        double c = 0;

        cheap_tie(a, b, c)(tuple(1, '2', 3.3));
        BOOST_HANA_RUNTIME_ASSERT(a == 1 && b == '2' && c == 3.3);
        //! [unpack]
    }

    {
        //! [unpack_idiom]
        BOOST_HANA_CONSTEXPR_LAMBDA auto add = [](auto x, auto y) {
            return x + y;
        };

        // Would be `boost::fusion::make_fused(add)` in Boost.Fusion.
        BOOST_HANA_CONSTEXPR_LAMBDA auto add_seq = partial(flip(unpack), add);

        BOOST_HANA_CONSTEXPR_ASSERT(add_seq(tuple(1, 2)) == add(1, 2));
        //! [unpack_idiom]
    }

    {
        //! [count]
        using namespace literals;
        BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
            return x % 2_c != 0_c;
        };

        constexpr auto types = type_list<int, char, long, short, char, double>;
        constexpr auto ints = integer_list<int, 1, 2, 3>;

        BOOST_HANA_CONSTANT_ASSERT(count(ints, odd) == 2_c);

        BOOST_HANA_CONSTANT_ASSERT(count(types, trait<std::is_floating_point>) == 1_c);
        BOOST_HANA_CONSTANT_ASSERT(count(types, _ == type<char>) == 2_c);
        BOOST_HANA_CONSTANT_ASSERT(count(types, _ == type<void>) == 0_c);
        //! [count]
    }

    {
        //! [maximum_by]
        BOOST_HANA_CONSTEXPR_LAMBDA auto size = [](auto xs, auto ys) {
            return length(xs) < length(ys);
        };

        BOOST_HANA_CONSTEXPR_ASSERT(
            maximum_by(size, tuple(tuple(), tuple(1, '2'), tuple(3.3, nullptr, 4)))
            ==
            tuple(3.3, nullptr, 4)
        );
        //! [maximum_by]
    }

    {
        //! [minimum_by]
        BOOST_HANA_CONSTEXPR_LAMBDA auto size = [](auto xs, auto ys) {
            return length(xs) < length(ys);
        };

        BOOST_HANA_CONSTANT_ASSERT(
            minimum_by(size, tuple(tuple(), tuple(1, '2'), tuple(3.3, nullptr, 4)))
            ==
            tuple()
        );
        //! [minimum_by]
    }
}
