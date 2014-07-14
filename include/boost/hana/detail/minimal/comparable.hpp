/*!
@file
Defines `boost::hana::detail::minimal::Comparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_COMPARABLE_HPP
#define BOOST_HANA_DETAIL_MINIMAL_COMPARABLE_HPP

#include <boost/hana/comparable.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Comparable::equal_mcd>
    struct Comparable { };

    template <typename mcd, typename = operators::enable>
    struct comparable_impl {
        int value;
        using hana_datatype = Comparable<mcd>;
    };

    template <typename mcd>
    struct make_comparable_impl {
        constexpr auto operator()(int i) const
        { return comparable_impl<mcd>{i}; }
    };

    template <typename mcd = hana::Comparable::equal_mcd>
    constexpr make_comparable_impl<mcd> comparable{};
}} // end namespace detail::minimal

template <>
struct Comparable::instance<
    detail::minimal::Comparable<Comparable::equal_mcd>,
    detail::minimal::Comparable<Comparable::equal_mcd>
> : Comparable::equal_mcd
{
    static constexpr bool equal_impl(
        detail::minimal::comparable_impl<Comparable::equal_mcd> x,
        detail::minimal::comparable_impl<Comparable::equal_mcd> y
    )
    { return x.value == y.value; }
};

template <>
struct Comparable::instance<
    detail::minimal::Comparable<Comparable::not_equal_mcd>,
    detail::minimal::Comparable<Comparable::not_equal_mcd>
> : Comparable::not_equal_mcd
{
    static constexpr bool not_equal_impl(
        detail::minimal::comparable_impl<Comparable::not_equal_mcd> x,
        detail::minimal::comparable_impl<Comparable::not_equal_mcd> y
    )
    { return x.value != y.value; }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_COMPARABLE_HPP
