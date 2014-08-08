/*!
@file
Defines `boost::hana::detail::minimal::Group`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_GROUP_HPP
#define BOOST_HANA_DETAIL_MINIMAL_GROUP_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/group/minus_mcd.hpp>
#include <boost/hana/group/negate_mcd.hpp>
#include <boost/hana/monoid/mcd.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <template <typename ...> class mcd = hana::Group::minus_mcd>
    struct Group { };

    template <template <typename ...> class mcd,
        typename = operators<hana::Group, hana::Monoid, hana::Comparable>>
    struct group_impl {
        int value;
        using hana_datatype = Group<mcd>;
    };

    template <template <typename ...> class mcd>
    struct make_group_impl {
        constexpr auto operator()(int i) const
        { return group_impl<mcd>{i}; }
    };

    template <template <typename ...> class mcd = hana::Group::minus_mcd>
    constexpr make_group_impl<mcd> group{};
}} // end namespace detail::minimal

// Provided for convenience in the unit tests.
template <template <typename ...> class mcd>
struct Comparable::instance<
    detail::minimal::Group<mcd>,
    detail::minimal::Group<mcd>
> : Comparable::equal_mcd
{
    static constexpr bool equal_impl(
        detail::minimal::group_impl<mcd> x,
        detail::minimal::group_impl<mcd> y
    )
    { return x.value == y.value; }
};

template <template <typename ...> class Mcd>
struct Monoid::instance<
    detail::minimal::Group<Mcd>,
    detail::minimal::Group<Mcd>
> : Monoid::mcd
{
    static constexpr auto plus_impl(
        detail::minimal::group_impl<Mcd> x,
        detail::minimal::group_impl<Mcd> y
    )
    { return detail::minimal::group<Mcd>(x.value + y.value); }

    static constexpr auto zero_impl()
    { return detail::minimal::group<Mcd>(0); }
};

template <>
struct Group::instance<
    detail::minimal::Group<Group::minus_mcd>,
    detail::minimal::Group<Group::minus_mcd>
> : Group::minus_mcd<
        detail::minimal::Group<Group::minus_mcd>,
        detail::minimal::Group<Group::minus_mcd>
    >
{
    static constexpr auto minus_impl(
        detail::minimal::group_impl<Group::minus_mcd> x,
        detail::minimal::group_impl<Group::minus_mcd> y
    )
    { return detail::minimal::group<Group::minus_mcd>(x.value - y.value); }
};

template <>
struct Group::instance<
    detail::minimal::Group<Group::negate_mcd>,
    detail::minimal::Group<Group::negate_mcd>
> : Group::negate_mcd<
    detail::minimal::Group<Group::negate_mcd>,
    detail::minimal::Group<Group::negate_mcd>
>
{
    static constexpr auto negate_impl(
        detail::minimal::group_impl<Group::negate_mcd> x
    )
    { return detail::minimal::group<Group::negate_mcd>(-x.value); }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_GROUP_HPP
