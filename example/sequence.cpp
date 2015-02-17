/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <sstream>
#include <string>
#include <type_traits>
using namespace boost::hana;


int main() {

{

//! [comparable]
BOOST_HANA_CONSTEXPR_CHECK(tuple(1, 2, 3) == tuple(1, 2, 3));
BOOST_HANA_CONSTEXPR_CHECK(tuple(1, 2, 3) != tuple(1, 2, 3, 4));
//! [comparable]

}{

//! [orderable]
BOOST_HANA_CONSTEXPR_CHECK(tuple(1, 2, 3) < tuple(2, 3, 4));
BOOST_HANA_CONSTEXPR_CHECK(tuple(1, 2, 3) < tuple(1, 2, 3, 4));
//! [orderable]

}{

//! [foldable]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

auto show = [=](auto x, auto y) {
    return "(" + to_string(x) + " + " + to_string(y) + ")";
};

BOOST_HANA_RUNTIME_CHECK(foldl(tuple(2, "3", '4'), "1", show) == "(((1 + 2) + 3) + 4)");
//! [foldable]

}{

//! [iterable]
BOOST_HANA_CONSTEXPR_CHECK(head(tuple(1, '2', 3.3)) == 1);
BOOST_HANA_CONSTEXPR_CHECK(tail(tuple(1, '2', 3.3)) == tuple('2', 3.3));
BOOST_HANA_CONSTANT_CHECK(!is_empty(tuple(1, '2', 3.3)));
BOOST_HANA_CONSTANT_CHECK(is_empty(tuple()));
//! [iterable]

}{

//! [functor]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

BOOST_HANA_RUNTIME_CHECK(
    fmap(tuple(1, '2', "345", std::string{"67"}), to_string) ==
    tuple("1", "2", "345", "67")
);
//! [functor]

}{

//! [applicative]
BOOST_HANA_CONSTEXPR_CHECK(lift<Tuple>('x') == tuple('x'));
BOOST_HANA_CONSTEXPR_CHECK(equal(lift<ext::std::Tuple>('x'), std::make_tuple('x')));

BOOST_HANA_CONSTEXPR_LAMBDA auto f = pair;
BOOST_HANA_CONSTEXPR_LAMBDA auto g = flip(pair);
BOOST_HANA_CONSTEXPR_CHECK(
    ap(tuple(f, g), tuple(1, 2, 3), tuple('a', 'b'))
        ==
    tuple(
        f(1, 'a'), f(1, 'b'), f(2, 'a'), f(2, 'b'), f(3, 'a'), f(3, 'b'),
        g(1, 'a'), g(1, 'b'), g(2, 'a'), g(2, 'b'), g(3, 'a'), g(3, 'b')
    )
);
//! [applicative]

}{

//! [monad]
BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return tuple(x, -x);
};

BOOST_HANA_CONSTEXPR_CHECK((tuple(1, 2, 3) | f) == tuple(1, -1, 2, -2, 3, -3));

BOOST_HANA_CONSTEXPR_CHECK(
    flatten(tuple(tuple(1, 2), tuple(3, 4), tuple(tuple(5, 6))))
    ==
    tuple(1, 2, 3, 4, tuple(5, 6))
);
//! [monad]

}{

//! [traversable]
using namespace std::string_literals;

BOOST_HANA_RUNTIME_CHECK(
    sequence<Tuple>(
        tuple(tuple("a1"s, "a2"s), tuple("b1"s), tuple("c1"s, "c2"s, "c3"s))
    )
    ==
    tuple(
        tuple("a1"s, "b1"s, "c1"s),
        tuple("a1"s, "b1"s, "c2"s),
        tuple("a1"s, "b1"s, "c3"s),

        tuple("a2"s, "b1"s, "c1"s),
        tuple("a2"s, "b1"s, "c2"s),
        tuple("a2"s, "b1"s, "c3"s)
    )
);

BOOST_HANA_CONSTEXPR_LAMBDA auto half = [](auto x) {
    return if_(x % int_<2> == int_<0>,
        just(x / int_<2>),
        nothing
    );
};

BOOST_HANA_CONSTANT_CHECK(
    traverse<Maybe>(tuple(int_<2>, int_<4>, int_<6>), half)
    ==
    just(tuple(int_<1>, int_<2>, int_<3>))
);

BOOST_HANA_CONSTANT_CHECK(
    traverse<Maybe>(tuple(int_<2>, int_<3>, int_<6>), half)
    ==
    nothing
);
//! [traversable]

}{

//! [make]
BOOST_HANA_CONSTANT_CHECK(make<Tuple>() == tuple());
BOOST_HANA_CONSTEXPR_CHECK(make<Tuple>(1, '2', 3.3) == tuple(1, '2', 3.3));
//! [make]

}{

//! [init]
using namespace literals;
BOOST_HANA_CONSTANT_CHECK(init(tuple(1)) == tuple());
BOOST_HANA_CONSTEXPR_CHECK(init(tuple(1, '2', 3.3, 4_c)) == tuple(1, '2', 3.3));
//! [init]

}{

//! [intersperse]
BOOST_HANA_CONSTEXPR_CHECK(
    intersperse(tuple(1, '2', 3.3), 'x') == tuple(1, 'x', '2', 'x', 3.3)
);
BOOST_HANA_CONSTANT_CHECK(intersperse(tuple(), 'x') == tuple());
//! [intersperse]

}{

//! [remove_at]
BOOST_HANA_CONSTEXPR_CHECK(
    remove_at(int_<2>, tuple(0, '1', 2.2, 3u)) == tuple(0, '1', 3u)
);
//! [remove_at]

}{

//! [remove_at_c]
BOOST_HANA_CONSTEXPR_CHECK(
    remove_at_c<2>(tuple(0, '1', 2.2, 3u)) == tuple(0, '1', 3u)
);
//! [remove_at_c]

}{

//! [reverse]
BOOST_HANA_CONSTEXPR_CHECK(reverse(tuple(1, '2', 3.3)) == tuple(3.3, '2', 1));
//! [reverse]

}{

//! [group_by]
BOOST_HANA_CONSTEXPR_CHECK(
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

}{

//! [group]
BOOST_HANA_CONSTANT_CHECK(
    group(tuple(int_<1>, long_<1>, type<int>, char_<'x'>, char_<'x'>))
    ==
    tuple(
        tuple(int_<1>, long_<1>),
        tuple(type<int>),
        tuple(char_<'x'>, char_<'x'>)
    )
);
//! [group]

}{

//! [zip]
BOOST_HANA_CONSTEXPR_CHECK(
    zip(tuple(1, 'a'), tuple(2, 3.3))
    ==
    tuple(tuple(1, 2), tuple('a', 3.3))
);

BOOST_HANA_CONSTEXPR_CHECK(
    zip(tuple(1, 'a'), tuple(2, 3.3), tuple(3, 'c', "ignored"))
    ==
    tuple(tuple(1, 2, 3), tuple('a', 3.3, 'c'))
);
//! [zip]

}{

//! [zip_with]
BOOST_HANA_CONSTEXPR_CHECK(
    zip_with(_ * _, tuple(1, 2, 3, 4), tuple(5, 6, 7, 8, "ignored"))
    ==
    tuple(5, 12, 21, 32)
);
//! [zip_with]

}{

//! [unzip]
BOOST_HANA_CONSTEXPR_CHECK(
    unzip(tuple(tuple(1, '2', 3.3), tuple('4', 5.5, 6)))
    ==
    tuple(tuple(1, '4'), tuple('2', 5.5), tuple(3.3, 6))
);

BOOST_HANA_CONSTEXPR_CHECK(
    unzip(tuple(tuple(1, '2', 3.3), tuple('4', 5.5, 6, "ignored")))
    ==
    tuple(tuple(1, '4'), tuple('2', 5.5), tuple(3.3, 6))
);
//! [unzip]

}{

//! [take_while]
using namespace literals;
BOOST_HANA_CONSTANT_CHECK(
    take_while(tuple_c<int, 0, 1, 2, 3>, _ < 2_c)
    ==
    tuple_c<int, 0, 1>
);
//! [take_while]

}{

//! [take_until]
using namespace literals;
BOOST_HANA_CONSTANT_CHECK(
    take_until(tuple_c<int, 3, 2, 1, 0>, _ < 2_c)
    ==
    tuple_c<int, 3, 2>
);
//! [take_until]

}{

//! [take]
using namespace literals;
BOOST_HANA_CONSTANT_CHECK(take(0_c, tuple(1, '2', 3.3)) == tuple());
BOOST_HANA_CONSTEXPR_CHECK(take(1_c, tuple(1, '2', 3.3)) == tuple(1));
BOOST_HANA_CONSTEXPR_CHECK(take(2_c, tuple(1, '2', 3.3)) == tuple(1, '2'));
BOOST_HANA_CONSTEXPR_CHECK(take(3_c, tuple(1, '2', 3.3)) == tuple(1, '2', 3.3));
BOOST_HANA_CONSTEXPR_CHECK(take(4_c, tuple(1, '2', 3.3)) == tuple(1, '2', 3.3));
//! [take]

}{

//! [take_c]
BOOST_HANA_CONSTEXPR_CHECK(take_c<2>(tuple(1, '2', 3.3)) == tuple(1, '2'));
//! [take_c]

}{

//! [span]
BOOST_HANA_CONSTEXPR_LAMBDA auto xs = tuple(int_<1>, int_<2>, int_<3>, int_<4>);
BOOST_HANA_CONSTANT_CHECK(
    span(xs, _ < int_<3>)
    ==
    pair(tuple(int_<1>, int_<2>), tuple(int_<3>, int_<4>))
);

BOOST_HANA_CONSTANT_CHECK(
    span(xs, _ < int_<0>)
    ==
    pair(tuple(), xs)
);

BOOST_HANA_CONSTANT_CHECK(
    span(xs, _ < int_<5>)
    ==
    pair(xs, tuple())
);
//! [span]

}{

//! [sort]
using namespace literals;
BOOST_HANA_CONSTANT_CHECK(
    sort(tuple(1_c, -2_c, 3_c, 0_c)) == tuple(-2_c, 0_c, 1_c, 3_c)
);
//! [sort]

}{

//! [sort_by]
using namespace literals;
BOOST_HANA_CONSTANT_CHECK(
    sort_by(_>_, tuple(1_c, -2_c, 3_c, 0_c))
    ==
    tuple(3_c, 1_c, 0_c, -2_c)
);
//! [sort_by]

}{

//! [slice]
BOOST_HANA_CONSTEXPR_CHECK(
    slice(tuple(1, '2', 3.3, type<float>), int_<1>, int_<3>)
    ==
    tuple('2', 3.3)
);
//! [slice]

}{

//! [slice_c]
BOOST_HANA_CONSTEXPR_CHECK(
    slice_c<1, 3>(tuple(1, '2', 3.3, type<float>))
    ==
    tuple('2', 3.3)
);
//! [slice_c]

}{

//! [permutations]
BOOST_HANA_CONSTEXPR_LAMBDA auto is_permutation_of = curry<2>([](auto xs, auto perm) {
    return elem(permutations(xs), perm);
});

BOOST_HANA_CONSTEXPR_CHECK(
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

}{

//! [unfoldl]
BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return if_(x == int_<0>, nothing, just(pair(x - int_<1>, x)));
};

BOOST_HANA_CONSTANT_CHECK(
    unfoldl<Tuple>(f, int_<10>)
    ==
    tuple_c<int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10>
);
//! [unfoldl]

}{

//! [unfoldr]
BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return if_(x == int_<0>, nothing, just(pair(x, x - int_<1>)));
};

BOOST_HANA_CONSTANT_CHECK(
    unfoldr<Tuple>(f, int_<10>)
    ==
    tuple_c<int, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>
);
//! [unfoldr]

}{

//! [scanl]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

auto show = [=](auto x, auto y) {
    return "(" + to_string(x) + " + " + to_string(y) + ")";
};

BOOST_HANA_RUNTIME_CHECK(scanl(tuple(2, "3", '4'), 1, show) == tuple(
    1,
    "(1 + 2)",
    "((1 + 2) + 3)",
    "(((1 + 2) + 3) + 4)"
));
//! [scanl]

}{

//! [scanl1]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

auto show = [=](auto x, auto y) {
    return "(" + to_string(x) + " + " + to_string(y) + ")";
};

BOOST_HANA_RUNTIME_CHECK(scanl1(tuple(1, "2", '3'), show) == tuple(
    1,
    "(1 + 2)",
    "((1 + 2) + 3)"
));
//! [scanl1]

}{

//! [scanr]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

auto show = [=](auto x, auto y) {
    return "(" + to_string(x) + " + " + to_string(y) + ")";
};

BOOST_HANA_RUNTIME_CHECK(scanr(tuple(1, "2", '3'), 4, show) == tuple(
    "(1 + (2 + (3 + 4)))",
    "(2 + (3 + 4))",
    "(3 + 4)",
    4
));
//! [scanr]

}{

//! [scanr1]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

auto show = [=](auto x, auto y) {
    return "(" + to_string(x) + " + " + to_string(y) + ")";
};

BOOST_HANA_RUNTIME_CHECK(scanr1(tuple(1, "2", '3'), show) == tuple(
    "(1 + (2 + 3))",
    "(2 + 3)",
    '3'
));
//! [scanr1]

}{

//! [partition]
BOOST_HANA_CONSTANT_CHECK(
    partition(tuple_c<int, 1, 2, 3, 4, 5, 6, 7>, [](auto x) {
        return x % int_<2> != int_<0>;
    })
    ==
    pair(
        tuple_c<int, 1, 3, 5, 7>,
        tuple_c<int, 2, 4, 6>
    )
);

BOOST_HANA_CONSTANT_CHECK(
    partition(tuple_t<void, int, float, char, double>, trait<std::is_floating_point>)
    ==
    pair(
        tuple_t<float, double>,
        tuple_t<void, int, char>
    )
);
//! [partition]

}

}
