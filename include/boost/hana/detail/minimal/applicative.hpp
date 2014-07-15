/*!
@file
Defines `boost::hana::detail::minimal::Applicative`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_APPLICATIVE_HPP
#define BOOST_HANA_DETAIL_MINIMAL_APPLICATIVE_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Applicative::mcd>
    struct Applicative { };

    template <typename T, typename mcd, typename = operators::enable>
    struct applicative_type {
        T value;
        using hana_datatype = Applicative<mcd>;
    };

    template <typename mcd>
    struct make_applicative_impl {
        template <typename X>
        constexpr auto operator()(X x) const
        { return applicative_type<X, mcd>{x}; }
    };

    template <typename mcd = hana::Applicative::mcd>
    constexpr make_applicative_impl<mcd> applicative{};
}} // end namespace detail::minimal

// Provided for convenience only.
template <typename mcd>
struct Comparable::instance<
    detail::minimal::Applicative<mcd>, detail::minimal::Applicative<mcd>
>
    : Comparable::equal_mcd
{
    template <typename A1, typename A2>
    static constexpr auto equal_impl(A1 x, A2 y)
    { return equal(x.value, y.value); }
};

template <typename mcd>
struct Functor::instance<detail::minimal::Applicative<mcd>>
    : Functor::fmap_mcd
{
    template <typename F, typename A>
    static constexpr auto fmap_impl(F f, A a)
    { return ap(lift<detail::minimal::Applicative<mcd>>(f), a); }
};

template <>
struct Applicative::instance<detail::minimal::Applicative<Applicative::mcd>>
    : Applicative::mcd
{
    template <typename T>
    static constexpr auto lift_impl(T t)
    { return detail::minimal::applicative<Applicative::mcd>(t); }

    template <typename F, typename X>
    static constexpr auto ap_impl(F f, X x)
    { return detail::minimal::applicative<Applicative::mcd>(f.value(x.value)); }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_APPLICATIVE_HPP
