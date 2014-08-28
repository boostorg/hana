/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>

#include <test/injection.hpp>
#include <test/laws/foldable.hpp>
#include <test/seq/foldable.hpp>

#include <vector>
using namespace boost::hana;


int main() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto foldable = test::seq;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
    using test::x;
    constexpr struct { } undefined{};

    // foldl
    {
        auto s = x<999>;
        BOOST_HANA_CONSTANT_ASSERT(equal(foldl(foldable(x<1>), s, f), f(s, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldl(foldable(x<1>, x<2>), s, f), f(f(s, x<1>), x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldl(foldable(x<1>, x<2>, x<3>), s, f), f(f(f(s, x<1>), x<2>), x<3>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldl(foldable(x<1>, x<2>, x<3>, x<4>), s, f), f(f(f(f(s, x<1>), x<2>), x<3>), x<4>)));
    }

    // foldl1
    {
        auto s = x<999>;
        BOOST_HANA_CONSTANT_ASSERT(equal(foldl1(foldable(s), f), s));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldl1(foldable(s, x<2>), f), f(s, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldl1(foldable(s, x<2>, x<3>), f), f(f(s, x<2>), x<3>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldl1(foldable(s, x<2>, x<3>, x<4>), f), f(f(f(s, x<2>), x<3>), x<4>)));
    }

    // foldr
    {
        auto s = x<999>;
        BOOST_HANA_CONSTANT_ASSERT(equal(foldr(foldable(x<0>), s, f), f(x<0>, s)));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldr(foldable(x<0>, x<1>), s, f), f(x<0>, f(x<1>, s))));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldr(foldable(x<0>, x<1>, x<2>), s, f), f(x<0>, f(x<1>, f(x<2>, s)))));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldr(foldable(x<0>, x<1>, x<2>, x<3>), s, f), f(x<0>, f(x<1>, f(x<2>, f(x<3>, s))))));
    }

    // foldr1
    {
        auto s = x<999>;
        BOOST_HANA_CONSTANT_ASSERT(equal(foldr1(foldable(s), f), s));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldr1(foldable(x<1>, s), f), f(x<1>, s)));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldr1(foldable(x<1>, x<2>, s), f), f(x<1>, f(x<2>, s))));
        BOOST_HANA_CONSTANT_ASSERT(equal(foldr1(foldable(x<1>, x<2>, x<3>, s), f), f(x<1>, f(x<2>, f(x<3>, s)))));
    }

    // for_each
    {
        auto check = [=](auto ...xs) {
            std::vector<int> seen{};
            for_each(foldable(xs...), [&](int x) {
                seen.push_back(x);
            });
            BOOST_HANA_RUNTIME_ASSERT(seen == std::vector<int>{xs...});
        };
        check();
        check(0);
        check(0, 1);
        check(0, 1, 2);
        check(0, 1, 2, 3);
        check(0, 1, 2, 3, 4);
    }

    // length
    {
        BOOST_HANA_CONSTANT_ASSERT(length(foldable()) == size_t<0>);
        BOOST_HANA_CONSTANT_ASSERT(length(foldable(undefined)) == size_t<1>);
        BOOST_HANA_CONSTANT_ASSERT(length(foldable(undefined, undefined)) == size_t<2>);
        BOOST_HANA_CONSTANT_ASSERT(length(foldable(undefined, undefined, undefined)) == size_t<3>);
    }

    // maximum
    {
        {
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<0>)) == int_<0>);

            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<0>, int_<1>)) == int_<1>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<1>, int_<0>)) == int_<1>);

            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<0>, int_<1>, int_<2>)) == int_<2>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<1>, int_<0>, int_<2>)) == int_<2>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<1>, int_<2>, int_<0>)) == int_<2>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<2>, int_<1>, int_<0>)) == int_<2>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<2>, int_<0>, int_<1>)) == int_<2>);

            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<0>, int_<1>, int_<2>, int_<3>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<1>, int_<0>, int_<2>, int_<3>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<1>, int_<2>, int_<0>, int_<3>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<1>, int_<2>, int_<3>, int_<0>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<2>, int_<1>, int_<3>, int_<0>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<2>, int_<3>, int_<1>, int_<0>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<2>, int_<3>, int_<0>, int_<1>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<3>, int_<2>, int_<0>, int_<1>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<3>, int_<0>, int_<2>, int_<1>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<3>, int_<0>, int_<1>, int_<2>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<0>, int_<2>, int_<3>, int_<1>)) == int_<3>);
            BOOST_HANA_CONSTANT_ASSERT(maximum(foldable(int_<0>, int_<3>, int_<1>, int_<2>)) == int_<3>);
        }

        {
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{0})) == int{0});

            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{0}, char{1})) == char{1});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{1}, char{0})) == int{1});

            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{0}, char{1}, long{2})) == long{2});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{1}, char{0}, long{2})) == long{2});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{1}, char{2}, long{0})) == char{2});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{2}, char{1}, long{0})) == int{2});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{2}, char{0}, long{1})) == int{2});

            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{0}, char{1}, long{2}, double{3})) == double{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{1}, char{0}, long{2}, double{3})) == double{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{1}, char{2}, long{0}, double{3})) == double{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{1}, char{2}, long{3}, double{0})) == long{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{2}, char{1}, long{3}, double{0})) == long{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{2}, char{3}, long{1}, double{0})) == char{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{2}, char{3}, long{0}, double{1})) == char{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{3}, char{2}, long{0}, double{1})) == int{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{3}, char{0}, long{2}, double{1})) == int{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{3}, char{0}, long{1}, double{2})) == int{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{0}, char{2}, long{3}, double{1})) == long{3});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{0}, char{3}, long{1}, double{2})) == char{3});
        }

        {
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{0}, char{1}, long_<2>)) == long{2});
            BOOST_HANA_CONSTEXPR_ASSERT(maximum(foldable(int{0}, long_<1>, char{2})) == char{2});
        }
    }

    // minimum
    {
        {
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<0>)) == int_<0>);

            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<0>, int_<1>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<1>, int_<0>)) == int_<0>);

            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<0>, int_<1>, int_<2>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<1>, int_<0>, int_<2>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<1>, int_<2>, int_<0>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<2>, int_<1>, int_<0>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<2>, int_<0>, int_<1>)) == int_<0>);

            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<0>, int_<1>, int_<2>, int_<3>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<1>, int_<0>, int_<2>, int_<3>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<1>, int_<2>, int_<0>, int_<3>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<1>, int_<2>, int_<3>, int_<0>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<2>, int_<1>, int_<3>, int_<0>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<2>, int_<3>, int_<1>, int_<0>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<2>, int_<3>, int_<0>, int_<1>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<3>, int_<2>, int_<0>, int_<1>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<3>, int_<0>, int_<2>, int_<1>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<3>, int_<0>, int_<1>, int_<2>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<0>, int_<2>, int_<3>, int_<1>)) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(minimum(foldable(int_<0>, int_<3>, int_<1>, int_<2>)) == int_<0>);
        }

        {
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{0})) == int{0});

            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{0}, char{1})) == int{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{1}, char{0})) == char{0});

            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{0}, char{1}, long{2})) == int{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{1}, char{0}, long{2})) == char{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{1}, char{2}, long{0})) == long{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{2}, char{1}, long{0})) == long{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{2}, char{0}, long{1})) == char{0});

            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{0}, char{1}, long{2}, double{3})) == int{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{1}, char{0}, long{2}, double{3})) == char{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{1}, char{2}, long{0}, double{3})) == long{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{1}, char{2}, long{3}, double{0})) == double{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{2}, char{1}, long{3}, double{0})) == double{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{2}, char{3}, long{1}, double{0})) == double{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{2}, char{3}, long{0}, double{1})) == long{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{3}, char{2}, long{0}, double{1})) == long{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{3}, char{0}, long{2}, double{1})) == char{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{3}, char{0}, long{1}, double{2})) == char{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{0}, char{2}, long{3}, double{1})) == int{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{0}, char{3}, long{1}, double{2})) == int{0});
        }

        {
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int{0}, char{1}, long_<2>)) == int{0});
            BOOST_HANA_CONSTEXPR_ASSERT(minimum(foldable(int_<0>, char{1}, long{2})) == int_<0>);
        }
    }

    // count
    {
        auto f = [](auto x) { return x; };

        BOOST_HANA_CONSTANT_ASSERT(count(foldable(), f) == size_t<0>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>), f) == size_t<1>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>), f) == size_t<0>);

        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<1>), f) == size_t<2>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<0>), f) == size_t<1>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<1>), f) == size_t<1>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<0>), f) == size_t<0>);

        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<1>, long_<1>), f) == size_t<3>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<1>, long_<0>), f) == size_t<2>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<0>, long_<1>), f) == size_t<2>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<1>, char_<0>, long_<0>), f) == size_t<1>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<1>, long_<1>), f) == size_t<2>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<1>, long_<0>), f) == size_t<1>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<0>, long_<1>), f) == size_t<1>);
        BOOST_HANA_CONSTANT_ASSERT(count(foldable(int_<0>, char_<0>, long_<0>), f) == size_t<0>);


        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}), f) == 1);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}), f) == 0);

        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{1}), f) == 2);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{0}), f) == 1);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{1}), f) == 1);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{0}), f) == 0);

        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{1}, double{1}), f) == 3);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{1}, double{0}), f) == 2);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{0}, double{1}), f) == 2);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{1}, char{0}, double{0}), f) == 1);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{1}, double{1}), f) == 2);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{1}, double{0}), f) == 1);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{0}, double{1}), f) == 1);
        BOOST_HANA_CONSTEXPR_ASSERT(count(foldable(int{0}, char{0}, double{0}), f) == 0);
    }

    // product
    {
        {
            BOOST_HANA_CONSTANT_ASSERT(product(foldable()) == int_<1>);
            BOOST_HANA_CONSTANT_ASSERT(product(foldable(int_<2>)) == int_<2>);
            BOOST_HANA_CONSTANT_ASSERT(product(foldable(int_<2>, int_<3>)) == int_<2 * 3>);
            BOOST_HANA_CONSTANT_ASSERT(product(foldable(int_<2>, int_<3>, int_<4>)) == int_<2 * 3 * 4>);
            BOOST_HANA_CONSTANT_ASSERT(product(foldable(int_<2>, int_<3>, int_<4>, int_<5>)) == int_<2 * 3 * 4 * 5>);
        }
        {
            BOOST_HANA_CONSTEXPR_ASSERT(product(foldable(2)) == 2);
            BOOST_HANA_CONSTEXPR_ASSERT(product(foldable(2, 3)) == 2 * 3);
            BOOST_HANA_CONSTEXPR_ASSERT(product(foldable(2, 3, 4)) == 2 * 3 * 4);
            BOOST_HANA_CONSTEXPR_ASSERT(product(foldable(2, 3, 4, 5)) == 2 * 3 * 4 * 5);
        }
    }

    // sum
    {
        {
            BOOST_HANA_CONSTANT_ASSERT(sum(foldable()) == int_<0>);
            BOOST_HANA_CONSTANT_ASSERT(sum(foldable(int_<1>)) == int_<1>);
            BOOST_HANA_CONSTANT_ASSERT(sum(foldable(int_<1>, int_<2>)) == int_<1 + 2>);
            BOOST_HANA_CONSTANT_ASSERT(sum(foldable(int_<1>, int_<2>, int_<3>)) == int_<1 + 2 + 3>);
            BOOST_HANA_CONSTANT_ASSERT(sum(foldable(int_<1>, int_<2>, int_<3>, int_<4>)) == int_<1 + 2 + 3 + 4>);
        }

        {
            BOOST_HANA_CONSTEXPR_ASSERT(sum(foldable(1)) == 1);
            BOOST_HANA_CONSTEXPR_ASSERT(sum(foldable(1, 2)) == 1 + 2);
            BOOST_HANA_CONSTEXPR_ASSERT(sum(foldable(1, 2, 3)) == 1 + 2 + 3);
            BOOST_HANA_CONSTEXPR_ASSERT(sum(foldable(1, 2, 3, 4)) == 1 + 2 + 3 + 4);
        }
    }

    // unpack
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(unpack(foldable(), f), f()));
        BOOST_HANA_CONSTANT_ASSERT(equal(unpack(foldable(x<0>), f), f(x<0>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(unpack(foldable(x<0>, x<1>), f), f(x<0>, x<1>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(unpack(foldable(x<0>, x<1>, x<2>), f), f(x<0>, x<1>, x<2>)));
        BOOST_HANA_CONSTANT_ASSERT(equal(unpack(foldable(x<0>, x<1>, x<2>, x<3>), f), f(x<0>, x<1>, x<2>, x<3>)));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(Foldable_laws(
            foldable(),
            foldable(x<0>),
            foldable(x<0>, x<1>),
            foldable(x<0>, x<1>, x<2>)
        ));
    }
}
