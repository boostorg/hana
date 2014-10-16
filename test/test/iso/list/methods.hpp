/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_LIST_METHODS_HPP
#define BOOST_HANA_TEST_TEST_ISO_LIST_METHODS_HPP

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/maybe.hpp>

#include <test/cnumeric/comparable.hpp>
#include <test/cnumeric/orderable.hpp>
#include <test/equivalence_class.hpp>
#include <test/injection.hpp>
#include <test/laws/list.hpp>
#include <test/minimal_product.hpp>


template <int i>
constexpr auto ord = test::cnumeric<int, i>;

template <typename L>
void List_methods() {
    // setup
    using namespace boost::hana;
    using test::x;

    constexpr struct { } undefined{};

    auto list = make<L>;

    auto prod = test::minimal_product<>;

    auto is = [](auto x) {
        return [=](auto y) { return equal(x, y); };
    };

    auto isnt = [](auto x) {
        return [=](auto y) { return not_equal(x, y); };
    };

    // make (that's tautological given our definition of `list`, but w/e)
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<L>(),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<L>(x<0>),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<L>(x<0>, x<1>),
            list(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<L>(x<0>, x<1>, x<2>),
            list(x<0>, x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            make<L>(x<0>, x<1>, x<2>, x<3>),
            list(x<0>, x<1>, x<2>, x<3>)
        ));
    }

    // cons
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(x<0>, list()),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(x<0>, list(x<1>)),
            list(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(x<0>, list(x<1>, x<2>)),
            list(x<0>, x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            cons(x<0>, list(x<1>, x<2>, x<3>)),
            list(x<0>, x<1>, x<2>, x<3>)
        ));
    }

    // nil
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            nil<L>, list()
        ));
    }

    // concat
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            concat(list(), list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            concat(list(), list(x<0>)),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            concat(list(), list(x<0>, x<1>)),
            list(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            concat(list(x<0>), list()),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            concat(list(x<0>), list(x<1>)),
            list(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            concat(list(x<0>), list(x<1>, x<2>)),
            list(x<0>, x<1>, x<2>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            concat(list(x<0>, x<1>), list()),
            list(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            concat(list(x<0>, x<1>), list(x<2>)),
            list(x<0>, x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            concat(list(x<0>, x<1>), list(x<2>, x<3>)),
            list(x<0>, x<1>, x<2>, x<3>)
        ));
    }

    // init
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            init(list(undefined)),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            init(list(x<0>, undefined)),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            init(list(x<0>, x<1>, undefined)),
            list(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            init(list(x<0>, x<1>, x<2>, undefined)),
            list(x<0>, x<1>, x<2>)
        ));
    }

    // intersperse
    {
        auto z = x<999>;
        BOOST_HANA_CONSTANT_ASSERT(equal(
            intersperse(list(), undefined),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            intersperse(list(x<0>), undefined),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            intersperse(list(x<0>, x<1>), z),
            list(x<0>, z, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            intersperse(list(x<0>, x<1>, x<2>), z),
            list(x<0>, z, x<1>, z, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            intersperse(list(x<0>, x<1>, x<2>, x<3>), z),
            list(x<0>, z, x<1>, z, x<2>, z, x<3>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            intersperse(list(x<0>, x<1>, x<2>, x<3>, x<4>), z),
            list(x<0>, z, x<1>, z, x<2>, z, x<3>, z, x<4>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            intersperse(list(x<0>, x<1>, x<2>, x<3>, x<4>, x<5>), z),
            list(x<0>, z, x<1>, z, x<2>, z, x<3>, z, x<4>, z, x<5>)
        ));
    }

    // slice
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            slice(list(), int_<0>, int_<0>),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            slice(list(undefined), int_<0>, int_<0>),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            slice(list(undefined, undefined), int_<0>, int_<0>),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            slice(list(x<0>), int_<0>, int_<1>),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            slice(list(x<0>, undefined), int_<0>, int_<1>),
            list(x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            slice(list(undefined, x<1>), int_<1>, int_<2>),
            list(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            slice(list(undefined, x<1>, undefined), int_<1>, int_<2>),
            list(x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            slice(list(x<0>, x<1>), int_<0>, int_<2>),
            list(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            slice(list(x<0>, x<1>, undefined), int_<0>, int_<2>),
            list(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            slice(list(undefined, x<1>, x<2>), int_<1>, int_<3>),
            list(x<1>, x<2>)
        ));
    }

    // snoc
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            snoc(list(), x<0>),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            snoc(list(x<0>), x<1>),
            list(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            snoc(list(x<0>, x<1>), x<2>),
            list(x<0>, x<1>, x<2>)
        ));
    }

    // take
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<0>, list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<1>, list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<2>, list()),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<0>, list(x<0>)),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<1>, list(x<0>)),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<2>, list(x<0>)),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<3>, list(x<0>)),
            list(x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<0>, list(x<0>, x<1>)),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<1>, list(x<0>, x<1>)),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<2>, list(x<0>, x<1>)),
            list(x<0>, x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take(int_<3>, list(x<0>, x<1>)),
            list(x<0>, x<1>)
        ));
    }

    // repeat
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            repeat<L>(int_<0>, x<0>),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            repeat<L>(int_<1>, x<0>),
            list(x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            repeat<L>(int_<2>, x<0>),
            list(x<0>, x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            repeat<L>(int_<3>, x<0>),
            list(x<0>, x<0>, x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            repeat<L>(int_<4>, x<0>),
            list(x<0>, x<0>, x<0>, x<0>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            repeat<L>(int_<5>, x<0>),
            list(x<0>, x<0>, x<0>, x<0>, x<0>)
        ));
    }

    // reverse
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            reverse(list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            reverse(list(x<0>)),
            list(x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            reverse(list(x<0>, x<1>)),
            list(x<1>, x<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            reverse(list(x<0>, x<1>, x<2>)),
            list(x<2>, x<1>, x<0>)
        ));
    }

    // sort
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort(list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort(list(ord<0>)),
            list(ord<0>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort(list(ord<0>, ord<1>)),
            list(ord<0>, ord<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort(list(ord<1>, ord<0>)),
            list(ord<0>, ord<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort(list(ord<1>, ord<0>, ord<4>, ord<2>)),
            list(ord<0>, ord<1>, ord<2>, ord<4>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort(list(ord<1>, ord<0>, ord<-4>, ord<2>)),
            list(ord<-4>, ord<0>, ord<1>, ord<2>)
        ));
    }

    // unzip
    {
        auto t = list; // tests are unreadable otherwise. mnemonic: tuple

        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t())),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(), t())),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(x<0>, x<2>), t(x<1>, x<3>, x<4>))),
            list(t(x<0>, x<1>), t(x<2>, x<3>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(),     t(),     t()))    ,
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(x<0>), t(),     t()))    ,
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(),     t(x<1>), t()))    ,
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(),     t(),     t(x<2>))),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(x<0>), t(x<1>), t()))    ,
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(),     t(x<1>), t(x<2>))),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(x<0>), t(),     t(x<2>))),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(x<0>), t(x<1>), t(x<2>))),
            list(t(x<0>, x<1>, x<2>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            unzip(list(t(x<0>, x<3>), t(x<1>, x<4>), t(x<2>, x<5>))),
            list(t(x<0>, x<1>, x<2>), t(x<3>, x<4>, x<5>))
        ));
    }

    // span
    {
        auto z = x<999>;

        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(), is(z)),
            prod(list(), list())
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(x<0>), is(z)),
            prod(list(), list(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(z), is(z)),
            prod(list(z), list())
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(x<0>, z), is(z)),
            prod(list(), list(x<0>, z))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(z, x<0>), is(z)),
            prod(list(z), list(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(x<0>, x<1>), is(z)),
            prod(list(), list(x<0>, x<1>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(x<0>, x<1>, x<2>), is(z)),
            prod(list(), list(x<0>, x<1>, x<2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(z, x<1>, x<2>), is(z)),
            prod(list(z), list(x<1>, x<2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(x<0>, z, x<2>), is(z)),
            prod(list(), list(x<0>, z, x<2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(z, z, x<2>), is(z)),
            prod(list(z, z), list(x<2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            span(list(z, z, z), is(z)),
            prod(list(z, z, z), list())
        ));
    }

    // take_while
    {
        auto z = x<999>;
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_while(list(), isnt(z)),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_while(list(x<1>), isnt(z)),
            list(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_while(list(z), isnt(z)),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_while(list(x<1>, x<2>), isnt(z)),
            list(x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_while(list(x<1>, z), isnt(z)),
            list(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_while(list(z, x<2>), isnt(z)),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_while(list(x<1>, x<2>, x<3>), isnt(z)),
            list(x<1>, x<2>, x<3>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_while(list(x<1>, x<2>, z), isnt(z)),
            list(x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_while(list(x<1>, z, x<3>), isnt(z)),
            list(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_while(list(z, x<2>, x<3>), isnt(z)),
            list()
        ));
    }

    // take_until
    {
        auto z = x<999>;

        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_until(list(), is(z)),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_until(list(x<1>), is(z)),
            list(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_until(list(z), is(z)),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_until(list(x<1>, x<2>), is(z)),
            list(x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_until(list(x<1>, z), is(z)),
            list(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_until(list(z, x<2>), is(z)),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_until(list(x<1>, x<2>, x<3>), is(z)),
            list(x<1>, x<2>, x<3>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_until(list(x<1>, x<2>, z), is(z)),
            list(x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_until(list(x<1>, z, x<3>), is(z)),
            list(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            take_until(list(z, x<2>, x<3>), is(z)),
            list()
        ));
    }

    // permutations
    {
        auto permute = [=](auto xs) {
            return [=](auto ...expected_) {
                auto actual = permutations(xs);
                auto expected = list(expected_...);

                BOOST_HANA_CONSTANT_ASSERT(and_(
                    equal(length(expected), length(actual)),
                    all(actual, [=](auto x) { return elem(expected, x); })
                ));
            };
        };

        BOOST_HANA_CONSTANT_ASSERT(equal(permutations(list()), list(list())));

        permute(list(x<0>))(list(x<0>));
        permute(list(x<0>, x<1>))(
            list(x<0>, x<1>),
            list(x<1>, x<0>)
        );
        permute(list(x<0>, x<1>, x<2>))(
            list(x<0>, x<1>, x<2>),
            list(x<0>, x<2>, x<1>),
            list(x<1>, x<0>, x<2>),
            list(x<1>, x<2>, x<0>),
            list(x<2>, x<0>, x<1>),
            list(x<2>, x<1>, x<0>)
        );
    }

    // filter
    {
        auto z = x<999>;

        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(), isnt(z)),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(z), isnt(z)),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(x<1>), isnt(z)),
            list(x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(x<1>, x<2>), isnt(z)),
            list(x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(z, x<2>), isnt(z)),
            list(x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(x<1>, z), isnt(z)),
            list(x<1>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(z, x<2>, x<3>), isnt(z)),
            list(x<2>, x<3>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(x<1>, z, x<3>), isnt(z)),
            list(x<1>, x<3>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(x<1>, x<2>, z), isnt(z)),
            list(x<1>, x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(x<1>, z, z), isnt(z)),
            list(x<1>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(z, x<2>, z), isnt(z)),
            list(x<2>)
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            filter(list(z, z, x<3>), isnt(z)),
            list(x<3>)
        ));
    }

    // group
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            group(list()),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            group(list(x<0>)),
            list(list(x<0>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            group(list(x<0>, x<0>)),
            list(list(x<0>, x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            group(list(x<0>, x<1>)),
            list(list(x<0>), list(x<1>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            group(list(x<0>, x<0>, x<0>)),
            list(list(x<0>, x<0>, x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            group(list(x<0>, x<0>, x<1>)),
            list(list(x<0>, x<0>), list(x<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            group(list(x<0>, x<1>, x<0>)),
            list(list(x<0>), list(x<1>), list(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            group(list(x<1>, x<0>, x<0>)),
            list(list(x<1>), list(x<0>, x<0>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            group(list(x<0>, x<0>, x<1>, x<1>)),
            list(list(x<0>, x<0>), list(x<1>, x<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            group(list(x<0>, x<0>, x<1>, x<1>, x<2>, x<2>, x<2>)),
            list(list(x<0>, x<0>), list(x<1>, x<1>), list(x<2>, x<2>, x<2>))
        ));
    }

    // partition
    {
        auto pred = [=](auto z) {
            return elem(list(x<-1>, x<-2>, x<-3>, x<-4>, x<-5>), z);
        };

        BOOST_HANA_CONSTANT_ASSERT(equal(
            partition(list(), pred),
            prod(list(), list())
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            partition(list(x<0>), pred),
            prod(list(), list(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            partition(list(x<0>, x<1>), pred),
            prod(list(), list(x<0>, x<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            partition(list(x<-1>), pred),
            prod(list(x<-1>), list())
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            partition(list(x<-1>, x<0>, x<2>), pred),
            prod(list(x<-1>), list(x<0>, x<2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            partition(list(x<0>, x<-3>, x<2>, x<-5>, x<6>), pred),
            prod(list(x<-3>, x<-5>), list(x<0>, x<2>, x<6>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            partition(list(x<-1>, x<2>, x<-3>, x<0>, x<-3>, x<4>), pred),
            prod(list(x<-1>, x<-3>, x<-3>), list(x<2>, x<0>, x<4>))
        ));
    }

    // scans
    {
        auto f = test::injection([]{});
        auto s = x<999>;

        // scanl
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(scanl(list(), s, f)                 , list(s)));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanl(list(x<0>), s, f)             , list(s, f(s, x<0>))));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanl(list(x<0>, x<1>), s, f)       , list(s, f(s, x<0>), f(f(s, x<0>), x<1>))));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanl(list(x<0>, x<1>, x<2>), s, f) , list(s, f(s, x<0>), f(f(s, x<0>), x<1>), f(f(f(s, x<0>), x<1>), x<2>))));
        }

        // scanr
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(scanr(list(), s, f)                 , list(                                                               s)));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanr(list(x<0>), s, f)             , list(                                                   f(x<0>, s), s)));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanr(list(x<0>, x<1>), s, f)       , list(                              f(x<0>, f(x<1>, s)), f(x<1>, s), s)));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanr(list(x<0>, x<1>, x<2>), s, f) , list(f(x<0>, f(x<1>, f(x<2>, s))), f(x<1>, f(x<2>, s)), f(x<2>, s), s)));
        }

        // scanl1
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(scanl1(list(), f)                       , list()));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanl1(list(x<0>), f)                   , list(x<0>)));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanl1(list(x<0>, x<1>), f)             , list(x<0>, f(x<0>, x<1>))));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanl1(list(x<0>, x<1>, x<2>), f)       , list(x<0>, f(x<0>, x<1>), f(f(x<0>, x<1>), x<2>))));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanl1(list(x<0>, x<1>, x<2>, x<3>), f) , list(x<0>, f(x<0>, x<1>), f(f(x<0>, x<1>), x<2>), f(f(f(x<0>, x<1>), x<2>), x<3>))));
        }

        // scanr1
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(scanr1(list(), f)                 , list(                                           )));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanr1(list(x<0>), f)             , list(                                       x<0>)));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanr1(list(x<0>, x<1>), f)       , list(                        f(x<0>, x<1>), x<1>)));
            BOOST_HANA_CONSTANT_ASSERT(equal(scanr1(list(x<0>, x<1>, x<2>), f) , list(f(x<0>, f(x<1>, x<2>)), f(x<1>, x<2>), x<2>)));
        }
    }

    // unfolds
    {
        // unfoldl
        {
            auto f = test::injection([]{});
            auto stop_at = [=](auto stop) {
                return [=](auto x) {
                    return if_(equal(stop, x),
                        nothing,
                        just(prod(x + int_<1>, f(x)))
                    );
                };
            };

            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldl<L>(stop_at(int_<0>), int_<0>),
                list()
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldl<L>(stop_at(int_<1>), int_<0>),
                list(f(int_<0>))
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldl<L>(stop_at(int_<2>), int_<0>),
                list(f(int_<1>), f(int_<0>))
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldl<L>(stop_at(int_<3>), int_<0>),
                list(f(int_<2>), f(int_<1>), f(int_<0>))
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldl<L>(stop_at(int_<4>), int_<0>),
                list(f(int_<3>), f(int_<2>), f(int_<1>), f(int_<0>))
            ));
        }

        // unfoldr
        {
            auto f = test::injection([]{});
            auto stop_at = [=](auto stop) {
                return [=](auto x) {
                    return if_(equal(stop, x),
                        nothing,
                        just(prod(f(x), x + int_<1>))
                    );
                };
            };

            BOOST_HANA_CONSTANT_ASSERT( equal(
                unfoldr<L>(stop_at(int_<0>), int_<0>),
                list()
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldr<L>(stop_at(int_<1>), int_<0>),
                list(f(int_<0>))
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldr<L>(stop_at(int_<2>), int_<0>),
                list(f(int_<0>), f(int_<1>))
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldr<L>(stop_at(int_<3>), int_<0>),
                list(f(int_<0>), f(int_<1>), f(int_<2>))
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unfoldr<L>(stop_at(int_<4>), int_<0>),
                list(f(int_<0>), f(int_<1>), f(int_<2>), f(int_<3>))
            ));
        }

        // Make sure unfold{r,l} can be reversed under certain conditions.
        {
            auto z = x<999>;
            auto f = prod;
            auto g = [=](auto k) { return if_(equal(k, z), nothing, just(k)); };

            // Make sure the special conditions are met
            BOOST_HANA_CONSTANT_ASSERT(equal(g(z), nothing));
            BOOST_HANA_CONSTANT_ASSERT(equal(g(f(x<0>, z)), just(prod(x<0>, z))));
            BOOST_HANA_CONSTANT_ASSERT(equal(g(f(z, x<0>)), just(prod(z, x<0>))));

            // Make sure the reversing works
            for_each(list(list(), list(x<0>), list(x<0>, x<1>), list(x<0>, x<1>, x<2>)),
                [=](auto xs) {
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

    // sort_by
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto pred = [](auto x, auto y) {
            return less(x.unwrap, y.unwrap);
        };
        auto a = [](auto z) { return test::tag(x<999>, z); };
        auto b = [](auto z) { return test::tag(x<888>, z); };

        BOOST_HANA_CONSTEXPR_LAMBDA auto check = [=](auto ...sorted) {
            auto perms = fmap(
                permutations(list(a(sorted)...)),
                partial(sort_by, pred)
            );
            BOOST_HANA_CONSTANT_ASSERT(all(perms, [=](auto xs) {
                return equal(xs, list(a(sorted)...));
            }));
        };

        check();
        check(ord<1>);
        check(ord<1>, ord<2>);
        check(ord<1>, ord<2>, ord<3>);

        // check stability
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort_by(pred, list(a(ord<1>), b(ord<1>))),
            list(a(ord<1>), b(ord<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort_by(pred, list(b(ord<1>), a(ord<1>))),
            list(b(ord<1>), a(ord<1>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort_by(pred, list(a(ord<1>), b(ord<1>), a(ord<2>), b(ord<2>))),
            list(a(ord<1>), b(ord<1>), a(ord<2>), b(ord<2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort_by(pred, list(a(ord<1>), a(ord<2>), b(ord<1>), b(ord<2>))),
            list(a(ord<1>), b(ord<1>), a(ord<2>), b(ord<2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort_by(pred, list(b(ord<1>), a(ord<1>), a(ord<2>), b(ord<2>))),
            list(b(ord<1>), a(ord<1>), a(ord<2>), b(ord<2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort_by(pred, list(a(ord<2>), b(ord<1>), b(ord<2>), a(ord<1>))),
            list(b(ord<1>), a(ord<1>), a(ord<2>), b(ord<2>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            sort_by(pred, list(a(ord<1>), a(ord<3>), b(ord<1>), a(ord<2>), b(ord<3>))),
            list(a(ord<1>), b(ord<1>), a(ord<2>), a(ord<3>), b(ord<3>))
        ));
    }

    // group_by
    {
        auto a = [](auto z) { return test::tag(x<999>, z); };
        auto b = [](auto z) { return test::tag(x<888>, z); };

        BOOST_HANA_CONSTEXPR_LAMBDA auto pred = [](auto x, auto y) {
            return equal(x.unwrap, y.unwrap);
        };

        BOOST_HANA_CONSTANT_ASSERT(equal(
            group_by(pred, list()),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            group_by(pred, list(a(x<0>))),
            list(list(a(x<0>)))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            group_by(pred, list(a(x<0>), b(x<0>))),
            list(list(a(x<0>), b(x<0>)))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            group_by(pred, list(a(x<0>), b(x<0>), a(x<1>))),
            list(list(a(x<0>), b(x<0>)), list(a(x<1>)))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            group_by(pred, list(a(x<0>), b(x<0>), a(x<1>), b(x<1>))),
            list(list(a(x<0>), b(x<0>)), list(a(x<1>), b(x<1>)))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            group_by(pred, list(a(x<0>), b(x<0>), a(x<1>), b(x<1>), b(x<0>))),
            list(list(a(x<0>), b(x<0>)), list(a(x<1>), b(x<1>)), list(b(x<0>)))
        ));
    }

    // zip
    {
        auto t = list; // tests are unreadable otherwise. mnemonic: tuple
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip(list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip(list(x<0>)),
            list(t(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip(list(x<0>, x<1>)),
            list(t(x<0>), t(x<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip(list(x<0>, x<1>, x<2>)),
            list(t(x<0>), t(x<1>), t(x<2>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip(list(), list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip(list(x<0>), list(x<1>)),
            list(t(x<0>, x<1>))
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip(list(x<0>), list(x<1>), list(x<2>)),
            list(t(x<0>, x<1>, x<2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip(list(x<0>, x<3>), list(x<1>, x<4>), list(x<2>, x<5>, x<8>)),
            list(t(x<0>, x<1>, x<2>), t(x<3>, x<4>, x<5>))
        ));
    }

    // zip_with
    {
        auto f = test::injection([]{});

        // zip 1
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(f, list(x<0>)),
            list(f(x<0>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(f, list(x<0>, x<1>)),
            list(f(x<0>), f(x<1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(f, list(x<0>, x<1>, x<2>)),
            list(f(x<0>), f(x<1>), f(x<2>))
        ));

        // zip 2
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list(), list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list(undefined), list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list(), list(undefined)),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(f, list(x<1>), list(x<-1>)),
            list(f(x<1>, x<-1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(f, list(x<1>, x<2>), list(x<-1>)),
            list(f(x<1>, x<-1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(f, list(x<1>), list(x<-1>, x<-2>)),
            list(f(x<1>, x<-1>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(f, list(x<1>, x<2>), list(x<-1>, x<-2>)),
            list(f(x<1>, x<-1>), f(x<2>, x<-2>))
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(f, list(x<1>, x<2>, x<3>, x<4>), list(x<-1>, x<-2>, x<-3>)),
            list(f(x<1>, x<-1>), f(x<2>, x<-2>), f(x<3>, x<-3>))
        ));

        // zip 3
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list(),          list(),          list()),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list(undefined), list(),          list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list(),          list(undefined), list()),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list(),          list(),          list(undefined)),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list(),          list(undefined), list(undefined)),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list(undefined), list(),          list(undefined)),
            list()
        ));
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(undefined, list(undefined), list(undefined), list()),
            list()
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(f, list(x<0>), list(x<1>), list(x<2>)),
            list(f(x<0>, x<1>, x<2>))
        ));

        // zip 4
        BOOST_HANA_CONSTANT_ASSERT(equal(
            zip_with(f,
                list(x<11>, x<12>, x<13>, x<14>),
                list(x<21>, x<22>, x<23>),
                list(x<31>, x<32>, x<33>, x<34>),
                list(x<41>, x<42>, x<43>, x<44>, x<45>)
            ),
            list(
                f(x<11>, x<21>, x<31>, x<41>),
                f(x<12>, x<22>, x<32>, x<42>),
                f(x<13>, x<23>, x<33>, x<43>)
            )
        ));
    }

    // laws
    {
        BOOST_HANA_CONSTANT_ASSERT(List_laws(
            list(),
            list(x<0>),
            list(x<0>, x<1>),
            list(x<0>, x<1>, x<2>),
            list(x<0>, x<1>, x<2>, x<3>)
        ));
    }
}

#endif //! BOOST_HANA_TEST_TEST_ISO_LIST_METHODS_HPP
