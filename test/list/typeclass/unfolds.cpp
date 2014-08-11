/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/injection.hpp>
#include <boost/hana/detail/minimal/list.hpp>
#include <boost/hana/detail/minimal/product.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


// stop_at must return a Maybe, so we need the comparison to be compile-time.
template <int i>
constexpr auto x = int_<i>;

template <typename mcd>
void test() {
    BOOST_HANA_CONSTEXPR_LAMBDA auto list = detail::minimal::list<mcd>;
    using L = detail::minimal::List<mcd>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = detail::injection([]{});
    constexpr auto prod = detail::minimal::product<>;

    // unfoldl
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto stop_at = [=](auto stop) {
            return [=](auto x) {
                return if_(equal(stop, x),
                    nothing,
                    just(prod(x + int_<1>, f(x)))
                );
            };
        };

        BOOST_HANA_CONSTANT_ASSERT(equal(
            unfoldl<L>(stop_at(x<0>), x<0>),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unfoldl<L>(stop_at(x<1>), x<0>),
            list(f(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unfoldl<L>(stop_at(x<2>), x<0>),
            list(f(x<1>), f(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unfoldl<L>(stop_at(x<3>), x<0>),
            list(f(x<2>), f(x<1>), f(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unfoldl<L>(stop_at(x<4>), x<0>),
            list(f(x<3>), f(x<2>), f(x<1>), f(x<0>))
        ));
    }

    // unfoldr
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto stop_at = [=](auto stop) {
            return [=](auto x) {
                return if_(equal(stop, x),
                    nothing,
                    just(prod(f(x), x + int_<1>))
                );
            };
        };

        BOOST_HANA_CONSTANT_ASSERT( equal(
            unfoldr<L>(stop_at(x<0>), x<0>),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unfoldr<L>(stop_at(x<1>), x<0>),
            list(f(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unfoldr<L>(stop_at(x<2>), x<0>),
            list(f(x<0>), f(x<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unfoldr<L>(stop_at(x<3>), x<0>),
            list(f(x<0>), f(x<1>), f(x<2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unfoldr<L>(stop_at(x<4>), x<0>),
            list(f(x<0>), f(x<1>), f(x<2>), f(x<3>))
        ));
    }

    // Make sure unfold{r,l} can be reversed under certain conditions.
    {
        constexpr auto z = x<999>;
        BOOST_HANA_CONSTEXPR_LAMBDA auto f = prod;
        BOOST_HANA_CONSTEXPR_LAMBDA auto g = [=](auto k) {
            return if_(equal(k, z), nothing, just(k));
        };

        // Make sure the special conditions are met
        BOOST_HANA_CONSTANT_ASSERT(g(z) == nothing);
        BOOST_HANA_CONSTANT_ASSERT(g(f(x<0>, z)) == just(prod(x<0>, z)));
        BOOST_HANA_CONSTANT_ASSERT(g(f(z, x<0>)) == just(prod(z, x<0>)));

        // Make sure the reversing works
        for_each(list(list(), list(x<0>), list(x<0>, x<1>), list(x<0>, x<1>, x<2>)), [=](auto xs) {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldl<L>(g, foldl(xs, z, f)),
                xs
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldr<L>(g, foldr(xs, z, f)),
                xs
            ));
        });
    }
}

int main() {
    test<List::mcd<void>>();
}
