/*!
@file
Defines `boost::hana::detail::minimal::List`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_LIST_HPP
#define BOOST_HANA_DETAIL_MINIMAL_LIST_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/iterable/foldable_instance.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/list/mcd.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::List::mcd<void>>
    struct List { };

    template <typename Storage, typename mcd, typename = operators::enable>
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

    template <typename mcd = hana::List::mcd<void>>
    constexpr make_list_impl<mcd> list{};
}} // end namespace detail::minimal

template <typename mcd>
struct Foldable::instance<detail::minimal::List<mcd>>
    : Iterable::FoldableInstance
{ };

template <typename Mcd>
struct Iterable::instance<detail::minimal::List<Mcd>>
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
            return detail::minimal::list<Mcd>(xs...);
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
struct List::instance<detail::minimal::List<List::mcd<void>>>
    : List::mcd<detail::minimal::List<List::mcd<void>>>
{
    template <typename X, typename Xs>
    static constexpr auto cons_impl(X x, Xs xs) {
        return xs.storage([=](auto ...xs) {
            return detail::minimal::list<List::mcd<void>>(x, xs...);
        });
    }

    static constexpr auto nil_impl() {
        return detail::minimal::list<List::mcd<void>>();
    }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_LIST_HPP
