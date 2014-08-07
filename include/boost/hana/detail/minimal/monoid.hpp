/*!
@file
Defines `boost::hana::detail::minimal::Monoid`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_MONOID_HPP
#define BOOST_HANA_DETAIL_MINIMAL_MONOID_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/monoid/mcd.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Monoid::mcd>
    struct Monoid { };

    template <typename mcd, typename = operators::enable>
    struct monoid_impl {
        int value;
        using hana_datatype = Monoid<mcd>;
    };

    template <typename mcd>
    struct make_monoid_impl {
        constexpr auto operator()(int i) const
        { return monoid_impl<mcd>{i}; }
    };

    template <typename mcd = hana::Monoid::mcd>
    constexpr make_monoid_impl<mcd> monoid{};
}} // end namespace detail::minimal

// Provided for convenience in the unit tests.
template <typename mcd>
struct Comparable::instance<
    detail::minimal::Monoid<mcd>, detail::minimal::Monoid<mcd>
> : Comparable::equal_mcd
{
    static constexpr bool equal_impl(
        detail::minimal::monoid_impl<mcd> x,
        detail::minimal::monoid_impl<mcd> y
    )
    { return x.value == y.value; }
};

template <>
struct Monoid::instance<
    detail::minimal::Monoid<Monoid::mcd>,
    detail::minimal::Monoid<Monoid::mcd>
> : Monoid::mcd
{
    static constexpr auto plus_impl(
        detail::minimal::monoid_impl<Monoid::mcd> x,
        detail::minimal::monoid_impl<Monoid::mcd> y
    )
    { return detail::minimal::monoid<Monoid::mcd>(x.value + y.value); }

    static constexpr auto zero_impl()
    { return detail::minimal::monoid<Monoid::mcd>(0); }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_MONOID_HPP
