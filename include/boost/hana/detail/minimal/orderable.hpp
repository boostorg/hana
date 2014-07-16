/*!
@file
Defines `boost::hana::detail::minimal::Orderable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_ORDERABLE_HPP
#define BOOST_HANA_DETAIL_MINIMAL_ORDERABLE_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/orderable/less_mcd.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Orderable::less_mcd>
    struct Orderable { };

    template <typename mcd, typename = operators::enable>
    struct orderable_impl {
        int value;
        using hana_datatype = Orderable<mcd>;
    };

    template <typename mcd>
    struct make_orderable_impl {
        constexpr auto operator()(int i) const
        { return orderable_impl<mcd>{i}; }
    };

    template <typename mcd = hana::Orderable::less_mcd>
    constexpr make_orderable_impl<mcd> orderable{};
}} // end namespace detail::minimal

// Provided for convenience in the unit tests.
template <typename mcd>
struct Comparable::instance<
    detail::minimal::Orderable<mcd>, detail::minimal::Orderable<mcd>
> : Comparable::equal_mcd
{
    static constexpr bool equal_impl(
        detail::minimal::orderable_impl<mcd> x,
        detail::minimal::orderable_impl<mcd> y
    )
    { return x.value == y.value; }
};

template <>
struct Orderable::instance<
    detail::minimal::Orderable<Orderable::less_mcd>,
    detail::minimal::Orderable<Orderable::less_mcd>
> : Orderable::less_mcd
{
    static constexpr bool less_impl(
        detail::minimal::orderable_impl<Orderable::less_mcd> x,
        detail::minimal::orderable_impl<Orderable::less_mcd> y
    )
    { return x.value < y.value; }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_ORDERABLE_HPP
