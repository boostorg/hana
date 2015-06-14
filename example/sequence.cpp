/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/ext/std/type_traits.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
using namespace boost::hana;


int main() {

//////////////////////////////////////////////////////////////////////////////
// Models
//////////////////////////////////////////////////////////////////////////////
{

//! [Applicative]
static_assert(lift<Tuple>('x') == make_tuple('x'), "");
static_assert(equal(lift<ext::std::Tuple>('x'), std::make_tuple('x')), "");

constexpr auto f = make_pair;
constexpr auto g = flip(make_pair);
static_assert(
    ap(make_tuple(f, g), make_tuple(1, 2, 3), make_tuple('a', 'b'))
        ==
    make_tuple(
        f(1, 'a'), f(1, 'b'), f(2, 'a'), f(2, 'b'), f(3, 'a'), f(3, 'b'),
        g(1, 'a'), g(1, 'b'), g(2, 'a'), g(2, 'b'), g(3, 'a'), g(3, 'b')
    )
, "");
//! [Applicative]

}{

//! [Comparable]
static_assert(make_tuple(1, 2, 3) == make_tuple(1, 2, 3), "");
BOOST_HANA_CONSTANT_CHECK(make_tuple(1, 2, 3) != make_tuple(1, 2, 3, 4));
//! [Comparable]

}{

//! [Orderable]
static_assert(make_tuple(1, 2, 3) < make_tuple(2, 3, 4), "");
static_assert(make_tuple(1, 2, 3) < make_tuple(1, 2, 3, 4), "");
//! [Orderable]

}{

//! [Foldable]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

auto show = [=](auto x, auto y) {
    return "(" + to_string(x) + " + " + to_string(y) + ")";
};

BOOST_HANA_RUNTIME_CHECK(
    fold_left(make_tuple(2, "3", '4'), "1", show) == "(((1 + 2) + 3) + 4)"
);
//! [Foldable]

}{

//! [Functor]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

BOOST_HANA_RUNTIME_CHECK(
    transform(make_tuple(1, '2', "345", std::string{"67"}), to_string) ==
    make_tuple("1", "2", "345", "67")
);
//! [Functor]

}{

//! [Iterable]
BOOST_HANA_CONSTEXPR_CHECK(head(make_tuple(1, '2', 3.3)) == 1);
BOOST_HANA_CONSTEXPR_CHECK(tail(make_tuple(1, '2', 3.3)) == make_tuple('2', 3.3));
BOOST_HANA_CONSTANT_CHECK(!is_empty(make_tuple(1, '2', 3.3)));
BOOST_HANA_CONSTANT_CHECK(is_empty(make_tuple()));
//! [Iterable]

}{

//! [Monad.ints]
BOOST_HANA_CONSTEXPR_CHECK(
    flatten(make_tuple(
        make_tuple(1, 2),
        make_tuple(3, 4),
        make_tuple(make_tuple(5, 6))
    ))
    == make_tuple(1, 2, 3, 4, make_tuple(5, 6))
);
//! [Monad.ints]

}{

//! [Monad.types]
// Using the Tuple Monad, we generate all the possible combinations of
// cv-qualifiers and reference qualifiers. Then, we use the Optional Monad
// to make sure that our generic function can be called with arguments
// of any of those types.

// cv_qualifiers : Type -> Tuple(Type)
auto cv_qualifiers = [](auto t) {
    return make_tuple(
        t,
        traits::add_const(t),
        traits::add_volatile(t),
        traits::add_volatile(traits::add_const(t))
    );
};

// ref_qualifiers : Type -> Tuple(Type)
auto ref_qualifiers = [](auto t) {
    return make_tuple(
        traits::add_lvalue_reference(t),
        traits::add_rvalue_reference(t)
    );
};

auto possible_args = cv_qualifiers(type<int>) | ref_qualifiers;

BOOST_HANA_CONSTANT_CHECK(
    possible_args == make_tuple(
                        type<int&>,
                        type<int&&>,
                        type<int const&>,
                        type<int const&&>,
                        type<int volatile&>,
                        type<int volatile&&>,
                        type<int const volatile&>,
                        type<int const volatile&&>
                    )
);

auto my_generic_function = [](auto&&) { return 1; };

for_each(possible_args, [=](auto t) {
    using T = typename decltype(t)::type;
    static_assert(decltype(is_just(sfinae(my_generic_function)(std::declval<T>()))){},
    "my_generic_function should be callable with any type of argument");
});
//! [Monad.types]

}{

//! [MonadPlus]
using namespace std::string_literals;

BOOST_HANA_CONSTANT_CHECK(empty<Tuple>() == make_tuple());

static_assert(append(make_tuple(1, '2', 3.3), nullptr)
                        == make_tuple(1, '2', 3.3, nullptr), "");

BOOST_HANA_RUNTIME_CHECK(
    concat(make_tuple(1, '2', 3.3), make_tuple("abcdef"s)) ==
    make_tuple(1, '2', 3.3, "abcdef"s)
);
//! [MonadPlus]

}{

//! [Searchable]
using namespace std::string_literals;

BOOST_HANA_RUNTIME_CHECK(
    find_if(make_tuple(1, '2', 3.3, "abc"s), is_a<std::string>) == just("abc"s)
);

BOOST_HANA_RUNTIME_CHECK(
    "abc"s ^in^ make_tuple(1, '2', 3.3, "abc"s)
);
//! [Searchable]

}

//////////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////////
{

//! [cartesian_product]
static_assert(
    cartesian_product(
        make_tuple(
            make_tuple(1, 2, 3),
            make_tuple('a', 'b'),
            make_tuple(type<int>, type<char>)
        )
    ) ==
    make_tuple(
        make_tuple(1, 'a', type<int>),
        make_tuple(1, 'a', type<char>),
        make_tuple(1, 'b', type<int>),
        make_tuple(1, 'b', type<char>),

        make_tuple(2, 'a', type<int>),
        make_tuple(2, 'a', type<char>),
        make_tuple(2, 'b', type<int>),
        make_tuple(2, 'b', type<char>),

        make_tuple(3, 'a', type<int>),
        make_tuple(3, 'a', type<char>),
        make_tuple(3, 'b', type<int>),
        make_tuple(3, 'b', type<char>)
    )
, "");
//! [cartesian_product]

}{

//! [group]
// without a predicate
BOOST_HANA_CONSTANT_CHECK(
    group(make_tuple(int_<1>, long_<1>, type<int>, char_<'x'>, char_<'x'>))
        == make_tuple(
            make_tuple(int_<1>, long_<1>),
            make_tuple(type<int>),
            make_tuple(char_<'x'>, char_<'x'>)
        )
);

// with a predicate
auto tuples = make_tuple(
    range_c<int, 0, 1>,
    range_c<int, 0, 2>,
    range_c<int, 1, 3>,
    range_c<int, 2, 6>
);

BOOST_HANA_CONSTANT_CHECK(
    group(tuples, comparing(length))
        == make_tuple(
            make_tuple(
                range_c<int, 0, 1>
            ),
            make_tuple(
                range_c<int, 0, 2>,
                range_c<int, 1, 3>
            ),
            make_tuple(
                range_c<int, 2, 6>
            )
        )
);
//! [group]

}{

//! [group.by]
BOOST_HANA_CONSTEXPR_CHECK(
    group.by(comparing(decltype_), make_tuple(1, 2, 3, 'x', 'y', 4.4, 5.5))
        == make_tuple(
            make_tuple(1, 2, 3),
            make_tuple('x', 'y'),
            make_tuple(4.4, 5.5)
        )
);
//! [group.by]

}{

//! [init]
using namespace boost::hana::literals;

BOOST_HANA_CONSTANT_CHECK(init(make_tuple(1)) == make_tuple());
BOOST_HANA_CONSTEXPR_CHECK(init(make_tuple(1, '2', 3.3, 4_c)) == make_tuple(1, '2', 3.3));
//! [init]

}{

//! [insert]
using namespace boost::hana::literals;
using namespace std::literals;

auto xs = make_tuple("Hello"s, "world!"s);
BOOST_HANA_RUNTIME_CHECK(insert(xs, 1_c, " "s) == make_tuple("Hello"s, " "s, "world!"s));
//! [insert]

}{

//! [insert_range]
using namespace boost::hana::literals;
using namespace std::literals;

auto xs = make_tuple("Hello"s, "world!"s);
BOOST_HANA_RUNTIME_CHECK(
    insert_range(xs, 1_c, make_tuple(1, 2, 3)) == make_tuple("Hello"s, 1, 2, 3, "world!"s)
);
//! [insert_range]

}{

//! [intersperse]
BOOST_HANA_CONSTEXPR_CHECK(
    intersperse(make_tuple(1, '2', 3.3), 'x') == make_tuple(1, 'x', '2', 'x', 3.3)
);
BOOST_HANA_CONSTANT_CHECK(intersperse(make_tuple(), 'x') == make_tuple());
//! [intersperse]

}{

//! [make]
BOOST_HANA_CONSTANT_CHECK(make<Tuple>() == make_tuple());
BOOST_HANA_CONSTEXPR_CHECK(make<Tuple>(1, '2', 3.3) == make_tuple(1, '2', 3.3));
//! [make]

}{

//! [permutations]
BOOST_HANA_CONSTEXPR_LAMBDA auto is_permutation_of = curry<2>([](auto xs, auto perm) {
    return contains(permutations(xs), perm);
});

BOOST_HANA_CONSTEXPR_CHECK(
    all_of(
        make_tuple(
            make_tuple('1', 2, 3.0),
            make_tuple('1', 3.0, 2),
            make_tuple(2, '1', 3.0),
            make_tuple(2, 3.0, '1'),
            make_tuple(3.0, '1', 2),
            make_tuple(3.0, 2, '1')
        ),
        is_permutation_of(make_tuple('1', 2, 3.0))
    )
);
//! [permutations]

}{

//! [partition]
BOOST_HANA_CONSTANT_CHECK(
    partition(tuple_c<int, 1, 2, 3, 4, 5, 6, 7>, [](auto x) {
        return x % int_<2> != int_<0>;
    })
    ==
    make_pair(
        tuple_c<int, 1, 3, 5, 7>,
        tuple_c<int, 2, 4, 6>
    )
);

BOOST_HANA_CONSTANT_CHECK(
    partition(tuple_t<void, int, float, char, double>, trait<std::is_floating_point>)
    ==
    make_pair(
        tuple_t<float, double>,
        tuple_t<void, int, char>
    )
);
//! [partition]

}{

//! [partition.by]
BOOST_HANA_CONSTANT_CHECK(
    partition.by(traits::is_floating_point, tuple_t<void, int, float, char, double>)
    ==
    make_pair(
        tuple_t<float, double>,
        tuple_t<void, int, char>
    )
);
//! [partition.by]

}{

//! [remove_at]
BOOST_HANA_CONSTEXPR_CHECK(
    remove_at(make_tuple(0, '1', 2.2, 3u), int_<2>) == make_tuple(0, '1', 3u)
);
//! [remove_at]

}{

//! [remove_at_c]
BOOST_HANA_CONSTEXPR_CHECK(
    remove_at_c<2>(make_tuple(0, '1', 2.2, 3u)) == make_tuple(0, '1', 3u)
);
//! [remove_at_c]

}{

//! [reverse]
BOOST_HANA_CONSTEXPR_CHECK(reverse(make_tuple(1, '2', 3.3)) == make_tuple(3.3, '2', 1));
//! [reverse]

}{

//! [scan.left]
auto to_string = [](auto x) {
    std::ostringstream ss;
    ss << x;
    return ss.str();
};

auto f = [=](auto state, auto element) {
    return "f(" + to_string(state) + ", " + to_string(element) + ")";
};

// with initial state
BOOST_HANA_RUNTIME_CHECK(scan.left(make_tuple(2, "3", '4'), 1, f) == make_tuple(
    1,
    "f(1, 2)",
    "f(f(1, 2), 3)",
    "f(f(f(1, 2), 3), 4)"
));

// without initial state
BOOST_HANA_RUNTIME_CHECK(scan.left(make_tuple(1, "2", '3'), f) == make_tuple(
    1,
    "f(1, 2)",
    "f(f(1, 2), 3)"
));
//! [scan.left]

}{

//! [scan.right]
auto to_string = [](auto x) {
    std::ostringstream ss;
    ss << x;
    return ss.str();
};

auto f = [=](auto element, auto state) {
    return "f(" + to_string(element) + ", " + to_string(state) + ")";
};

// with initial state
BOOST_HANA_RUNTIME_CHECK(scan.right(make_tuple(1, "2", '3'), 4, f) == make_tuple(
    "f(1, f(2, f(3, 4)))",
    "f(2, f(3, 4))",
    "f(3, 4)",
    4
));

// without initial state
BOOST_HANA_RUNTIME_CHECK(scan.right(make_tuple(1, "2", '3'), f) == make_tuple(
    "f(1, f(2, 3))",
    "f(2, 3)",
    '3'
));
//! [scan.right]

}{

//! [slice]
BOOST_HANA_CONSTEXPR_CHECK(
    slice(make_tuple(1, '2', 3.3, type<float>), int_<1>, int_<3>)
    ==
    make_tuple('2', 3.3)
);
//! [slice]

}{

//! [slice_c]
BOOST_HANA_CONSTEXPR_CHECK(
    slice_c<1, 3>(make_tuple(1, '2', 3.3, type<float>))
    ==
    make_tuple('2', 3.3)
);
//! [slice_c]

}{

//! [sort]
using namespace boost::hana::literals;

// without a predicate
BOOST_HANA_CONSTANT_CHECK(
    sort(make_tuple(1_c, -2_c, 3_c, 0_c)) ==
         make_tuple(-2_c, 0_c, 1_c, 3_c)
);

// with a predicate
BOOST_HANA_CONSTANT_CHECK(
    sort(make_tuple(1_c, -2_c, 3_c, 0_c), greater) ==
         make_tuple(3_c, 1_c, 0_c, -2_c)
);
//! [sort]

}{

//! [sort.by]
using namespace std::literals;
using namespace boost::hana::literals;

auto tuples = make_tuple(
    make_tuple(2_c, 'x', nullptr),
    make_tuple(1_c, "foobar"s, int_<4>)
);
BOOST_HANA_RUNTIME_CHECK(
    sort.by(ordering(head), tuples)
        == make_tuple(
            make_tuple(1_c, "foobar"s, int_<4>),
            make_tuple(2_c, 'x', nullptr)
        )
);
//! [sort.by]

}{

//! [span]
constexpr auto xs = make_tuple(int_<1>, int_<2>, int_<3>, int_<4>);

BOOST_HANA_CONSTANT_CHECK(
    span(xs, _ < int_<3>)
    ==
    make_pair(make_tuple(int_<1>, int_<2>), make_tuple(int_<3>, int_<4>))
);

BOOST_HANA_CONSTANT_CHECK(
    span(xs, _ < int_<0>)
    ==
    make_pair(make_tuple(), xs)
);

BOOST_HANA_CONSTANT_CHECK(
    span(xs, _ < int_<5>)
    ==
    make_pair(xs, make_tuple())
);
//! [span]

}{

//! [span.by]
constexpr auto xs = make_tuple(int_<1>, int_<2>, int_<3>, int_<4>);

BOOST_HANA_CONSTANT_CHECK(
    span.by(_ < int_<3>, xs)
    ==
    make_pair(make_tuple(int_<1>, int_<2>), make_tuple(int_<3>, int_<4>))
);
//! [span.by]

}{

//! [subsequence]
constexpr auto letters = to<Tuple>(range_c<char, 'a', 'z'>);
constexpr auto indices = to<Tuple>(make_range(int_<0>, length(letters)));

BOOST_HANA_CONSTEXPR_LAMBDA auto even_indices = filter(indices, [](auto n) {
    return n % uint<2> == uint<0>;
});

BOOST_HANA_CONSTANT_CHECK(
    subsequence(letters, even_indices) == tuple_c<char,
        'a', 'c', 'e', 'g', 'i', 'k', 'm', 'o', 'q', 's', 'u', 'w', 'y'
    >
);
//! [subsequence]

}{

//! [take]
using namespace boost::hana::literals;

BOOST_HANA_CONSTANT_CHECK( take(make_tuple(1, '2', 3.3), 0_c) == make_tuple());
BOOST_HANA_CONSTEXPR_CHECK(take(make_tuple(1, '2', 3.3), 1_c) == make_tuple(1));
BOOST_HANA_CONSTEXPR_CHECK(take(make_tuple(1, '2', 3.3), 2_c) == make_tuple(1, '2'));
BOOST_HANA_CONSTEXPR_CHECK(take(make_tuple(1, '2', 3.3), 3_c) == make_tuple(1, '2', 3.3));
BOOST_HANA_CONSTEXPR_CHECK(take(make_tuple(1, '2', 3.3), 4_c) == make_tuple(1, '2', 3.3));
//! [take]

}{

//! [take_c]
BOOST_HANA_CONSTEXPR_CHECK(take_c<2>(make_tuple(1, '2', 3.3)) == make_tuple(1, '2'));
//! [take_c]

}{

//! [take_until]
using namespace boost::hana::literals;
BOOST_HANA_CONSTANT_CHECK(
    take_until(tuple_c<int, 3, 2, 1, 0>, _ < 2_c)
    ==
    tuple_c<int, 3, 2>
);
//! [take_until]

}{

//! [take_while]
using namespace boost::hana::literals;
BOOST_HANA_CONSTANT_CHECK(
    take_while(tuple_c<int, 0, 1, 2, 3>, _ < 2_c)
    ==
    tuple_c<int, 0, 1>
);
//! [take_while]

}{

//! [unfold]
BOOST_HANA_CONSTANT_CHECK(
    unfold<Tuple>.left(int_<10>, [](auto x) {
        return if_(x == int_<0>,
            nothing,
            just(make_pair(x - int_<1>, x))
        );
    })
    ==
    tuple_c<int, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10>
);

BOOST_HANA_CONSTANT_CHECK(
    unfold<Tuple>.right(int_<10>, [](auto x) {
        return if_(x == int_<0>,
            nothing,
            just(make_pair(x, x - int_<1>))
        );
    })
    ==
    tuple_c<int, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>
);
//! [unfold]

}{

//! [unique]
using namespace std::literals;

// without a predicate
constexpr auto types = tuple_t<int, float, float, char, int, int, int, double>;
BOOST_HANA_CONSTANT_CHECK(
    unique(types) == tuple_t<int, float, char, int, double>
);

// with a predicate
auto objects = make_tuple(1, 2, "abc"s, 'd', "efg"s, "hij"s, 3.4f);
BOOST_HANA_RUNTIME_CHECK(
    unique(objects, [](auto const& t, auto const& u)
                    { return decltype_(t) == decltype_(u); })
    ==
    make_tuple(1, "abc"s, 'd', "efg"s, 3.4f)
);
//! [unique]

}{

//! [unique.by]
using namespace std::literals;

auto objects = make_tuple(1, 2, "abc"s, 'd', "efg"s, "hij"s, 3.4f);
BOOST_HANA_RUNTIME_CHECK(
    unique.by(comparing(decltype_), objects) ==
        make_tuple(1, "abc"s, 'd', "efg"s, 3.4f)
);
//! [unique.by]

}{

//! [unzip]
BOOST_HANA_CONSTEXPR_CHECK(
    unzip(make_tuple(make_tuple(1, '2', 3.3), make_tuple('4', 5.5, 6)))
    ==
    make_tuple(make_tuple(1, '4'), make_tuple('2', 5.5), make_tuple(3.3, 6))
);

BOOST_HANA_CONSTEXPR_CHECK(
    unzip(make_tuple(make_tuple(1, '2', 3.3), make_tuple('4', 5.5, 6, "ignored")))
    ==
    make_tuple(make_tuple(1, '4'), make_tuple('2', 5.5), make_tuple(3.3, 6))
);
//! [unzip]

}{

//! [zip]
BOOST_HANA_CONSTEXPR_CHECK(
    zip(make_tuple(1, 'a'), make_tuple(2, 3.3))
    ==
    make_tuple(make_tuple(1, 2), make_tuple('a', 3.3))
);

BOOST_HANA_CONSTEXPR_CHECK(
    zip(make_tuple(1, 'a'), make_tuple(2, 3.3), make_tuple(3, 'c', "ignored"))
    ==
    make_tuple(make_tuple(1, 2, 3), make_tuple('a', 3.3, 'c'))
);
//! [zip]

}{

//! [zip_with]
BOOST_HANA_CONSTEXPR_CHECK(
    zip.with(_ * _, make_tuple(1, 2, 3, 4), make_tuple(5, 6, 7, 8, "ignored"))
    ==
    make_tuple(5, 12, 21, 32)
);
//! [zip_with]

}

}
