/*!
@file
Defines `boost::hana::detail::minimal::Logical`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_LOGICAL_HPP
#define BOOST_HANA_DETAIL_MINIMAL_LOGICAL_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/logical/mcd.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Logical::mcd>
    struct Logical { };

    template <typename mcd, typename = operators<hana::Logical, hana::Comparable>>
    struct logical_impl {
        bool value;
        using hana_datatype = Logical<mcd>;
    };

    template <typename mcd>
    struct make_logical_impl {
        constexpr auto operator()(bool b) const
        { return logical_impl<mcd>{b}; }
    };

    template <typename mcd = hana::Logical::mcd>
    constexpr make_logical_impl<mcd> logical{};
}} // end namespace detail::minimal

// Provided for convenience in unit tests.
template <typename mcd>
struct Comparable::instance<
    detail::minimal::Logical<mcd>, detail::minimal::Logical<mcd>
> : Comparable::equal_mcd
{
    static constexpr bool equal_impl(
        detail::minimal::logical_impl<mcd> x,
        detail::minimal::logical_impl<mcd> y
    )
    { return x.value == y.value; }
};

template <>
struct Logical::instance<detail::minimal::Logical<Logical::mcd>>
    : Logical::mcd
{
    template <typename Log, typename Then, typename Else>
    static constexpr auto eval_if_impl(Log cond, Then t, Else e) {
        return cond.value ? t([](auto x) { return x; })
                          : e([](auto x) { return x; });
    }

    template <typename Log>
    static constexpr auto not_impl(Log cond)
    { return detail::minimal::logical<Logical::mcd>(!cond.value); }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_LOGICAL_HPP
