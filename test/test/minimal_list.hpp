/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_MINIMAL_LIST_HPP
#define BOOST_HANA_TEST_TEST_MINIMAL_LIST_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/list/mcd.hpp>
#include <boost/hana/monad/monad.hpp>

// Provided instances
#include <boost/hana/applicative/list_mcd.hpp>
#include <boost/hana/comparable/list_mcd.hpp>
#include <boost/hana/foldable/iterable_mcd.hpp>
#include <boost/hana/functor/list_mcd.hpp>
#include <boost/hana/monad/list_mcd.hpp>
#include <boost/hana/searchable/list_mcd.hpp>
#include <boost/hana/traversable/list_mcd.hpp>

// Conversions
#include <boost/hana/list/free/convert.hpp>


namespace test {
    template <typename mcd = boost::hana::List::mcd<void>>
    struct List { };

    template <typename Storage, typename mcd>
    struct list_type {
        Storage storage;
        using hana_datatype = List<mcd>;
    };

    template <typename mcd>
    struct make_list_impl {
        template <typename ...Xs>
        constexpr auto operator()(Xs ...xs) const {
            auto storage = [=](auto f) { return f(xs...); };
            return list_type<decltype(storage), mcd>{storage};
        }
    };

    template <typename Mcd = boost::hana::List::mcd<void>>
    constexpr make_list_impl<Mcd> list{};

    template <typename Mcd = boost::hana::List::mcd<void>>
    constexpr auto minimal_list = list<Mcd>;
}

namespace boost { namespace hana {
    template <typename Mcd>
    struct Iterable::instance<test::List<Mcd>>
        : Iterable::mcd
    {
        template <typename Xs>
        static constexpr auto head_impl(Xs xs) {
            return xs.storage([](auto x, auto ...xs) {
                return x;
            });
        }

        template <typename Xs>
        static constexpr auto tail_impl(Xs xs) {
            return xs.storage([](auto x, auto ...xs) {
                return test::list<Mcd>(xs...);
            });
        }

        template <typename Xs>
        static constexpr auto is_empty_impl(Xs xs) {
            return xs.storage([](auto ...xs) {
                return bool_<sizeof...(xs) == 0>;
            });
        }
    };

    template <>
    struct List::instance<test::List<List::mcd<void>>>
        : List::mcd<test::List<List::mcd<void>>>
    {
        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return test::list<List::mcd<void>>(x, xs...);
            });
        }

        static constexpr auto nil_impl() {
            return test::list<List::mcd<void>>();
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_MINIMAL_LIST_HPP
