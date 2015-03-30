/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
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
BOOST_HANA_CONSTEXPR_CHECK(make<Tuple>(1, 2, 3) == make<Tuple>(1, 2, 3));
BOOST_HANA_CONSTEXPR_CHECK(make<Tuple>(1, 2, 3) != make<Tuple>(1, 2, 3, 4));
//! [comparable]

}{

//! [orderable]
BOOST_HANA_CONSTEXPR_CHECK(make<Tuple>(1, 2, 3) < make<Tuple>(2, 3, 4));
BOOST_HANA_CONSTEXPR_CHECK(make<Tuple>(1, 2, 3) < make<Tuple>(1, 2, 3, 4));
//! [orderable]

}{

//! [foldable]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

auto show = [=](auto x, auto y) {
    return "(" + to_string(x) + " + " + to_string(y) + ")";
};

BOOST_HANA_RUNTIME_CHECK(fold.left(make<Tuple>(2, "3", '4'), "1", show) == "(((1 + 2) + 3) + 4)");
//! [foldable]

}{

//! [iterable]
BOOST_HANA_CONSTEXPR_CHECK(head(make<Tuple>(1, '2', 3.3)) == 1);
BOOST_HANA_CONSTEXPR_CHECK(tail(make<Tuple>(1, '2', 3.3)) == make<Tuple>('2', 3.3));
BOOST_HANA_CONSTANT_CHECK(!is_empty(make<Tuple>(1, '2', 3.3)));
BOOST_HANA_CONSTANT_CHECK(is_empty(make<Tuple>()));
//! [iterable]

}{

//! [searchable]
using namespace std::string_literals;

BOOST_HANA_RUNTIME_CHECK(
    find_if(make<Tuple>(1, '2', 3.3, "abc"s), is_a<std::string>) == just("abc"s)
);

BOOST_HANA_RUNTIME_CHECK(
    "abc"s ^in^ make<Tuple>(1, '2', 3.3, "abc"s)
);
//! [searchable]

}{

//! [functor]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

BOOST_HANA_RUNTIME_CHECK(
    transform(make<Tuple>(1, '2', "345", std::string{"67"}), to_string) ==
    make<Tuple>("1", "2", "345", "67")
);
//! [functor]

}{

//! [applicative]
BOOST_HANA_CONSTEXPR_CHECK(lift<Tuple>('x') == make<Tuple>('x'));
BOOST_HANA_CONSTEXPR_CHECK(equal(lift<ext::std::Tuple>('x'), std::make_tuple('x')));

BOOST_HANA_CONSTEXPR_LAMBDA auto f = pair;
BOOST_HANA_CONSTEXPR_LAMBDA auto g = flip(pair);
BOOST_HANA_CONSTEXPR_CHECK(
    ap(make<Tuple>(f, g), make<Tuple>(1, 2, 3), make<Tuple>('a', 'b'))
        ==
    make<Tuple>(
        f(1, 'a'), f(1, 'b'), f(2, 'a'), f(2, 'b'), f(3, 'a'), f(3, 'b'),
        g(1, 'a'), g(1, 'b'), g(2, 'a'), g(2, 'b'), g(3, 'a'), g(3, 'b')
    )
);
//! [applicative]

}{

//! [monad]
BOOST_HANA_CONSTEXPR_LAMBDA auto f = [](auto x) {
    return make<Tuple>(x, -x);
};

BOOST_HANA_CONSTEXPR_CHECK((make<Tuple>(1, 2, 3) | f) == make<Tuple>(1, -1, 2, -2, 3, -3));

BOOST_HANA_CONSTEXPR_CHECK(
    flatten(make<Tuple>(make<Tuple>(1, 2), make<Tuple>(3, 4), make<Tuple>(make<Tuple>(5, 6))))
    ==
    make<Tuple>(1, 2, 3, 4, make<Tuple>(5, 6))
);
//! [monad]

}{

//! [monad_plus]
using namespace std::string_literals;

BOOST_HANA_CONSTANT_CHECK(empty<Tuple>() == make<Tuple>());
BOOST_HANA_CONSTEXPR_CHECK(
    append(make<Tuple>(1, '2', 3.3), nullptr) == make<Tuple>(1, '2', 3.3, nullptr)
);

BOOST_HANA_CONSTEXPR_CHECK(
    concat(make<Tuple>(1, '2', 3.3), make<Tuple>("abcdef"s)) ==
    make<Tuple>(1, '2', 3.3, "abcdef"s)
);
//! [monad_plus]

}{

//! [traversable]
using namespace std::string_literals;

BOOST_HANA_RUNTIME_CHECK(
    sequence<Tuple>(
        make<Tuple>(make<Tuple>("a1"s, "a2"s),
                    make<Tuple>("b1"s),
                    make<Tuple>("c1"s, "c2"s, "c3"s))
    )
    ==
    make<Tuple>(
        make<Tuple>("a1"s, "b1"s, "c1"s),
        make<Tuple>("a1"s, "b1"s, "c2"s),
        make<Tuple>("a1"s, "b1"s, "c3"s),

        make<Tuple>("a2"s, "b1"s, "c1"s),
        make<Tuple>("a2"s, "b1"s, "c2"s),
        make<Tuple>("a2"s, "b1"s, "c3"s)
    )
);

BOOST_HANA_CONSTEXPR_LAMBDA auto half = [](auto x) {
    return if_(x % int_<2> == int_<0>,
        just(x / int_<2>),
        nothing
    );
};

BOOST_HANA_CONSTANT_CHECK(
    traverse<Maybe>(make<Tuple>(int_<2>, int_<4>, int_<6>), half)
    ==
    just(make<Tuple>(int_<1>, int_<2>, int_<3>))
);

BOOST_HANA_CONSTANT_CHECK(
    traverse<Maybe>(make<Tuple>(int_<2>, int_<3>, int_<6>), half)
    ==
    nothing
);
//! [traversable]

}{

//! [make]
BOOST_HANA_CONSTANT_CHECK(make<Tuple>() == make<Tuple>());
BOOST_HANA_CONSTEXPR_CHECK(make<Tuple>(1, '2', 3.3) == make<Tuple>(1, '2', 3.3));
//! [make]

}{

//! [init]
using namespace literals;
BOOST_HANA_CONSTANT_CHECK(init(make<Tuple>(1)) == make<Tuple>());
BOOST_HANA_CONSTEXPR_CHECK(init(make<Tuple>(1, '2', 3.3, 4_c)) == make<Tuple>(1, '2', 3.3));
//! [init]

}{

//! [intersperse]
BOOST_HANA_CONSTEXPR_CHECK(
    intersperse(make<Tuple>(1, '2', 3.3), 'x') == make<Tuple>(1, 'x', '2', 'x', 3.3)
);
BOOST_HANA_CONSTANT_CHECK(intersperse(make<Tuple>(), 'x') == make<Tuple>());
//! [intersperse]

}{

//! [remove_at]
BOOST_HANA_CONSTEXPR_CHECK(
    remove_at(int_<2>, make<Tuple>(0, '1', 2.2, 3u)) == make<Tuple>(0, '1', 3u)
);
//! [remove_at]

}{

//! [remove_at_c]
BOOST_HANA_CONSTEXPR_CHECK(
    remove_at_c<2>(make<Tuple>(0, '1', 2.2, 3u)) == make<Tuple>(0, '1', 3u)
);
//! [remove_at_c]

}{

//! [reverse]
BOOST_HANA_CONSTEXPR_CHECK(reverse(make<Tuple>(1, '2', 3.3)) == make<Tuple>(3.3, '2', 1));
//! [reverse]

}{

//! [group_by]
BOOST_HANA_CONSTEXPR_CHECK(
    group_by(equal ^on^ decltype_,
        make<Tuple>(1, 2, 3, 'x', 'y', 4.4, 5.5)
    )
    == make<Tuple>(
        make<Tuple>(1, 2, 3),
        make<Tuple>('x', 'y'),
        make<Tuple>(4.4, 5.5)
    )
);
//! [group_by]

}{

//! [group]
BOOST_HANA_CONSTANT_CHECK(
    group(make<Tuple>(int_<1>, long_<1>, type<int>, char_<'x'>, char_<'x'>))
    ==
    make<Tuple>(
        make<Tuple>(int_<1>, long_<1>),
        make<Tuple>(type<int>),
        make<Tuple>(char_<'x'>, char_<'x'>)
    )
);
//! [group]

}{

//! [zip]
BOOST_HANA_CONSTEXPR_CHECK(
    zip(make<Tuple>(1, 'a'), make<Tuple>(2, 3.3))
    ==
    make<Tuple>(make<Tuple>(1, 2), make<Tuple>('a', 3.3))
);

BOOST_HANA_CONSTEXPR_CHECK(
    zip(make<Tuple>(1, 'a'), make<Tuple>(2, 3.3), make<Tuple>(3, 'c', "ignored"))
    ==
    make<Tuple>(make<Tuple>(1, 2, 3), make<Tuple>('a', 3.3, 'c'))
);
//! [zip]

}{

//! [zip_with]
BOOST_HANA_CONSTEXPR_CHECK(
    zip.with(_ * _, make<Tuple>(1, 2, 3, 4), make<Tuple>(5, 6, 7, 8, "ignored"))
    ==
    make<Tuple>(5, 12, 21, 32)
);
//! [zip_with]

}{

//! [unzip]
BOOST_HANA_CONSTEXPR_CHECK(
    unzip(make<Tuple>(make<Tuple>(1, '2', 3.3), make<Tuple>('4', 5.5, 6)))
    ==
    make<Tuple>(make<Tuple>(1, '4'), make<Tuple>('2', 5.5), make<Tuple>(3.3, 6))
);

BOOST_HANA_CONSTEXPR_CHECK(
    unzip(make<Tuple>(make<Tuple>(1, '2', 3.3), make<Tuple>('4', 5.5, 6, "ignored")))
    ==
    make<Tuple>(make<Tuple>(1, '4'), make<Tuple>('2', 5.5), make<Tuple>(3.3, 6))
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
BOOST_HANA_CONSTANT_CHECK(take(0_c, make<Tuple>(1, '2', 3.3)) == make<Tuple>());
BOOST_HANA_CONSTEXPR_CHECK(take(1_c, make<Tuple>(1, '2', 3.3)) == make<Tuple>(1));
BOOST_HANA_CONSTEXPR_CHECK(take(2_c, make<Tuple>(1, '2', 3.3)) == make<Tuple>(1, '2'));
BOOST_HANA_CONSTEXPR_CHECK(take(3_c, make<Tuple>(1, '2', 3.3)) == make<Tuple>(1, '2', 3.3));
BOOST_HANA_CONSTEXPR_CHECK(take(4_c, make<Tuple>(1, '2', 3.3)) == make<Tuple>(1, '2', 3.3));
//! [take]

}{

//! [take_c]
BOOST_HANA_CONSTEXPR_CHECK(take_c<2>(make<Tuple>(1, '2', 3.3)) == make<Tuple>(1, '2'));
//! [take_c]

}{

//! [span]
BOOST_HANA_CONSTEXPR_LAMBDA auto xs = make<Tuple>(int_<1>, int_<2>, int_<3>, int_<4>);
BOOST_HANA_CONSTANT_CHECK(
    span(xs, _ < int_<3>)
    ==
    pair(make<Tuple>(int_<1>, int_<2>), make<Tuple>(int_<3>, int_<4>))
);

BOOST_HANA_CONSTANT_CHECK(
    span(xs, _ < int_<0>)
    ==
    pair(make<Tuple>(), xs)
);

BOOST_HANA_CONSTANT_CHECK(
    span(xs, _ < int_<5>)
    ==
    pair(xs, make<Tuple>())
);
//! [span]

}{

//! [sort]
using namespace literals;
BOOST_HANA_CONSTANT_CHECK(
    sort(make<Tuple>(1_c, -2_c, 3_c, 0_c)) == make<Tuple>(-2_c, 0_c, 1_c, 3_c)
);
//! [sort]

}{

//! [sort_by]
using namespace literals;
BOOST_HANA_CONSTANT_CHECK(
    sort_by(_>_, make<Tuple>(1_c, -2_c, 3_c, 0_c))
    ==
    make<Tuple>(3_c, 1_c, 0_c, -2_c)
);
//! [sort_by]

}{

//! [slice]
BOOST_HANA_CONSTEXPR_CHECK(
    slice(make<Tuple>(1, '2', 3.3, type<float>), int_<1>, int_<3>)
    ==
    make<Tuple>('2', 3.3)
);
//! [slice]

}{

//! [slice_c]
BOOST_HANA_CONSTEXPR_CHECK(
    slice_c<1, 3>(make<Tuple>(1, '2', 3.3, type<float>))
    ==
    make<Tuple>('2', 3.3)
);
//! [slice_c]

}{

//! [permutations]
BOOST_HANA_CONSTEXPR_LAMBDA auto is_permutation_of = curry<2>([](auto xs, auto perm) {
    return elem(permutations(xs), perm);
});

BOOST_HANA_CONSTEXPR_CHECK(
    all_of(
        make<Tuple>(
            make<Tuple>('1', 2, 3.0),
            make<Tuple>('1', 3.0, 2),
            make<Tuple>(2, '1', 3.0),
            make<Tuple>(2, 3.0, '1'),
            make<Tuple>(3.0, '1', 2),
            make<Tuple>(3.0, 2, '1')
        ),
        is_permutation_of(make<Tuple>('1', 2, 3.0))
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

BOOST_HANA_RUNTIME_CHECK(scanl(make<Tuple>(2, "3", '4'), 1, show) == make<Tuple>(
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

BOOST_HANA_RUNTIME_CHECK(scanl1(make<Tuple>(1, "2", '3'), show) == make<Tuple>(
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

BOOST_HANA_RUNTIME_CHECK(scanr(make<Tuple>(1, "2", '3'), 4, show) == make<Tuple>(
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

BOOST_HANA_RUNTIME_CHECK(scanr1(make<Tuple>(1, "2", '3'), show) == make<Tuple>(
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
