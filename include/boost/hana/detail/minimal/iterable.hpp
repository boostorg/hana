/*!
@file
Defines `boost::hana::detail::minimal::Iterable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_ITERABLE_HPP
#define BOOST_HANA_DETAIL_MINIMAL_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/list/instance.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Iterable::mcd>
    struct Iterable { };

    template <typename Storage, typename mcd,
        typename = operators<hana::Comparable>
    >
    struct iterable_type {
        Storage storage;
        using hana_datatype = Iterable<mcd>;
    };

    template <typename mcd>
    struct make_iterable_impl {
        template <typename ...Xs>
        constexpr auto operator()(Xs ...xs) const {
            auto storage = [=](auto f) { return f(xs...); };
            return iterable_type<decltype(storage), mcd>{storage};
        }
    };

    template <typename mcd = hana::Iterable::mcd>
    constexpr make_iterable_impl<mcd> iterable{};
}} // end namespace detail::minimal

// Since some Iterable methods return an Iterable, we define the obvious
// Comparable instance so we can perform comparisons in the unit tests.
template <typename mcd>
struct Comparable::instance<
    detail::minimal::Iterable<mcd>, detail::minimal::Iterable<mcd>
> : Comparable::equal_mcd {
    template <typename Xs, typename Ys>
    static constexpr auto equal_impl(Xs xs, Ys ys)
    { return equal(to<List>(xs), to<List>(ys)); }
};

template <>
struct Iterable::instance<detail::minimal::Iterable<Iterable::mcd>>
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
            return detail::minimal::iterable<Iterable::mcd>(xs...);
        });
    }

    template <typename Xs>
    static constexpr auto is_empty_impl(Xs xs) {
        return xs.storage([](auto ...xs) {
            return bool_<sizeof...(xs) == 0>;
        });
    }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_ITERABLE_HPP
