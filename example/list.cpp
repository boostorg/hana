/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integer_list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#include <type_traits>
#include <sstream>
#include <string>
using namespace boost::hana;


int main() {
    {
        //! [cons]
        BOOST_HANA_CONSTEXPR_ASSERT(cons(1, tuple()) == tuple(1));
        BOOST_HANA_CONSTEXPR_ASSERT(cons(1, tuple('2', 3.3)) == tuple(1, '2', 3.3));
        BOOST_HANA_CONSTEXPR_ASSERT(cons(1, cons('2', cons(3.3, tuple()))) == tuple(1, '2', 3.3));
        //! [cons]
    }

    {
        //! [filter]
        BOOST_HANA_CONSTEXPR_ASSERT(
            filter(tuple(1, 2.0, 3, 4.0), trait_<std::is_integral>)
            ==
            tuple(1, 3)
        );
        //! [filter]
    }

    {
        //! [make]
        BOOST_HANA_CONSTANT_ASSERT(make<Tuple>() == tuple());
        BOOST_HANA_CONSTEXPR_ASSERT(make<Tuple>(1, '2', 3.3) == tuple(1, '2', 3.3));
        //! [make]
    }

    {
        //! [init]
        using namespace literals;
        BOOST_HANA_CONSTANT_ASSERT(init(tuple(1)) == tuple());
        BOOST_HANA_CONSTEXPR_ASSERT(init(tuple(1, '2', 3.3, 4_c)) == tuple(1, '2', 3.3));
        //! [init]
    }

    {
        //! [reverse]
        BOOST_HANA_CONSTEXPR_ASSERT(reverse(tuple(1, '2', 3.3)) == tuple(3.3, '2', 1));
        //! [reverse]
    }

    {
        //! [concat]
        using namespace literals;
        BOOST_HANA_CONSTEXPR_ASSERT(
            concat(tuple(1, '2'), tuple(3.3, 4_c)) == tuple(1, '2', 3.3, 4_c)
        );
        //! [concat]
    }

    {
        //! [group_by]
        BOOST_HANA_CONSTEXPR_ASSERT(
            group_by(equal ^on^ decltype_,
                tuple(1, 2, 3, 'x', 'y', 4.4, 5.5)
            )
            == tuple(
                tuple(1, 2, 3),
                tuple('x', 'y'),
                tuple(4.4, 5.5)
            )
        );
        //! [group_by]
    }

    {
        //! [group]
        BOOST_HANA_CONSTANT_ASSERT(
            group(tuple(int_<1>, long_<1>, type<int>, char_<'x'>, char_<'x'>))
            ==
            tuple(
                tuple(int_<1>, long_<1>),
                tuple(type<int>),
                tuple(char_<'x'>, char_<'x'>)
            )
        );
        //! [group]
    }

    {
        //! [zip]
        BOOST_HANA_CONSTEXPR_ASSERT(
            zip(tuple(1, 'a'), tuple(2, 3.3))
            ==
            tuple(tuple(1, 2), tuple('a', 3.3))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            zip(tuple(1, 'a'), tuple(2, 3.3), tuple(3, 'c', "ignored"))
            ==
            tuple(tuple(1, 2, 3), tuple('a', 3.3, 'c'))
        );
        //! [zip]
    }

    {
        //! [zip_with]
        BOOST_HANA_CONSTEXPR_ASSERT(
            zip_with(_ * _, tuple(1, 2, 3, 4), tuple(5, 6, 7, 8, "ignored"))
            ==
            tuple(5, 12, 21, 32)
        );
        //! [zip_with]
    }

    {
        //! [unzip]
        BOOST_HANA_CONSTEXPR_ASSERT(
            unzip(tuple(tuple(1, '2', 3.3), tuple('4', 5.5, 6)))
            ==
            tuple(tuple(1, '4'), tuple('2', 5.5), tuple(3.3, 6))
        );

        BOOST_HANA_CONSTEXPR_ASSERT(
            unzip(tuple(tuple(1, '2', 3.3), tuple('4', 5.5, 6, "ignored")))
            ==
            tuple(tuple(1, '4'), tuple('2', 5.5), tuple(3.3, 6))
        );
        //! [unzip]
    }

    {
        //! [take_while]
        using namespace literals;
        BOOST_HANA_CONSTANT_ASSERT(
            take_while(_ < 2_c, integer_list<int, 0, 1, 2, 3>)
            ==
            integer_list<int, 0, 1>
        );
        //! [take_while]
    }

    {
        //! [take_until]
        using namespace literals;
        BOOST_HANA_CONSTANT_ASSERT(
            take_until(_ < 2_c, integer_list<int, 3, 2, 1, 0>)
            ==
            integer_list<int, 3, 2>
        );
        //! [take_until]
    }

    {
        //! [take]
        using namespace literals;
        BOOST_HANA_CONSTANT_ASSERT(take(0_c, tuple(1, '2', 3.3)) == tuple());
        BOOST_HANA_CONSTEXPR_ASSERT(take(1_c, tuple(1, '2', 3.3)) == tuple(1));
        BOOST_HANA_CONSTEXPR_ASSERT(take(2_c, tuple(1, '2', 3.3)) == tuple(1, '2'));
        BOOST_HANA_CONSTEXPR_ASSERT(take(3_c, tuple(1, '2', 3.3)) == tuple(1, '2', 3.3));
        BOOST_HANA_CONSTEXPR_ASSERT(take(4_c, tuple(1, '2', 3.3)) == tuple(1, '2', 3.3));
        //! [take]
    }

    {
        //! [take_c]
        BOOST_HANA_CONSTEXPR_ASSERT(take_c<2>(tuple(1, '2', 3.3)) == tuple(1, '2'));
        //! [take_c]
    }

    {
        //! [snoc]
        BOOST_HANA_CONSTEXPR_ASSERT(snoc(tuple(), 1) == tuple(1));
        BOOST_HANA_CONSTEXPR_ASSERT(snoc(tuple(1, '2'), 3.3) == tuple(1, '2', 3.3));
        BOOST_HANA_CONSTEXPR_ASSERT(snoc(snoc(snoc(tuple(), 1), '2'), 3.3) == tuple(1, '2', 3.3));
        //! [snoc]
    }

    {
        //! [span]
        BOOST_HANA_CONSTEXPR_LAMBDA auto xs = tuple(int_<1>, int_<2>, int_<3>, int_<4>);
        BOOST_HANA_CONSTANT_ASSERT(
            span(xs, _ < int_<3>)
            ==
            pair(tuple(int_<1>, int_<2>), tuple(int_<3>, int_<4>))
        );

        BOOST_HANA_CONSTANT_ASSERT(
            span(xs, _ < int_<0>)
            ==
            pair(tuple(), xs)
        );

        BOOST_HANA_CONSTANT_ASSERT(
            span(xs, _ < int_<5>)
            ==
            pair(xs, tuple())
        );
        //! [span]
    }

    {
        //! [sort]
        using namespace literals;
        BOOST_HANA_CONSTANT_ASSERT(
            sort(tuple(1_c, -2_c, 3_c, 0_c)) == tuple(-2_c, 0_c, 1_c, 3_c)
        );
        //! [sort]
    }

    {
        //! [sort_by]
        using namespace literals;
        BOOST_HANA_CONSTANT_ASSERT(
            sort_by(_>_, tuple(1_c, -2_c, 3_c, 0_c))
            ==
            tuple(3_c, 1_c, 0_c, -2_c)
        );
        //! [sort_by]
    }

    {
        //! [nil]
        BOOST_HANA_CONSTANT_ASSERT(nil<Tuple> == tuple());
        //! [nil]
    }

    {
        //! [slice]
        BOOST_HANA_CONSTEXPR_ASSERT(
            slice(tuple(1, '2', 3.3, type<float>), int_<1>, int_<3>)
            ==
            tuple('2', 3.3)
        );
        //! [slice]
    }

    {
        //! [slice_c]
        BOOST_HANA_CONSTEXPR_ASSERT(
            slice_c<1, 3>(tuple(1, '2', 3.3, type<float>))
            ==
            tuple('2', 3.3)
        );
        //! [slice_c]
    }

    {
        //! [permutations]
        BOOST_HANA_CONSTEXPR_LAMBDA auto is_permutation_of = curry<2>([](auto xs, auto perm) {
            return elem(permutations(xs), perm);
        });

        BOOST_HANA_CONSTEXPR_ASSERT(
            all(
                tuple(
                    tuple('1', 2, 3.0),
                    tuple('1', 3.0, 2),
                    tuple(2, '1', 3.0),
                    tuple(2, 3.0, '1'),
                    tuple(3.0, '1', 2),
                    tuple(3.0, 2, '1')
                ),
                is_permutation_of(tuple('1', 2, 3.0))
            )
        );
        //! [permutations]
    }


    {
        //! [unfoldl]
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
            return if_(x == int_<0>, nothing, just(pair(x - int_<1>, x)));
        };

        BOOST_HANA_CONSTANT_ASSERT(
            unfoldl<IntegerList>(f, int_<10>)
            ==
            integer_list<int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10>
        );
        //! [unfoldl]
    }

    {
        //! [unfoldr]
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
            return if_(x == int_<0>, nothing, just(pair(x, x - int_<1>)));
        };

        BOOST_HANA_CONSTANT_ASSERT(
            unfoldr<IntegerList>(f, int_<10>)
            ==
            integer_list<int, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>
        );
        //! [unfoldr]
    }

    {
        //! [scanl]
        auto to_string = [](auto x) {
            return (std::ostringstream{} << x).str();
        };

        auto show = [=](auto x, auto y) {
            return "(" + to_string(x) + " + " + to_string(y) + ")";
        };

        BOOST_HANA_RUNTIME_ASSERT(scanl(tuple(2, "3", '4'), 1, show) == tuple(
            1,
            "(1 + 2)",
            "((1 + 2) + 3)",
            "(((1 + 2) + 3) + 4)"
        ));
        //! [scanl]
    }

    {
        //! [scanl1]
        auto to_string = [](auto x) {
            return (std::ostringstream{} << x).str();
        };

        auto show = [=](auto x, auto y) {
            return "(" + to_string(x) + " + " + to_string(y) + ")";
        };

        BOOST_HANA_RUNTIME_ASSERT(scanl1(tuple(1, "2", '3'), show) == tuple(
            1,
            "(1 + 2)",
            "((1 + 2) + 3)"
        ));
        //! [scanl1]
    }

    {
        //! [scanr]
        auto to_string = [](auto x) {
            return (std::ostringstream{} << x).str();
        };

        auto show = [=](auto x, auto y) {
            return "(" + to_string(x) + " + " + to_string(y) + ")";
        };

        BOOST_HANA_RUNTIME_ASSERT(scanr(tuple(1, "2", '3'), 4, show) == tuple(
            "(1 + (2 + (3 + 4)))",
            "(2 + (3 + 4))",
            "(3 + 4)",
            4
        ));
        //! [scanr]
    }

    {
        //! [scanr1]
        auto to_string = [](auto x) {
            return (std::ostringstream{} << x).str();
        };

        auto show = [=](auto x, auto y) {
            return "(" + to_string(x) + " + " + to_string(y) + ")";
        };

        BOOST_HANA_RUNTIME_ASSERT(scanr1(tuple(1, "2", '3'), show) == tuple(
            "(1 + (2 + 3))",
            "(2 + 3)",
            '3'
        ));
        //! [scanr1]
    }

    {
        //! [partition]
        BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
            return x % int_<2> != int_<0>;
        };

        BOOST_HANA_CONSTANT_ASSERT(
            partition(integer_list<int, 1, 2, 3, 4, 5, 6, 7>, odd)
            ==
            pair(
                integer_list<int, 1, 3, 5, 7>,
                integer_list<int, 2, 4, 6>
            )
        );

        BOOST_HANA_CONSTANT_ASSERT(
            partition(type_list<void, int, float, char, double>, trait<std::is_floating_point>)
            ==
            pair(
                type_list<float, double>,
                type_list<void, int, char>
            )
        );
        //! [partition]
    }
}
