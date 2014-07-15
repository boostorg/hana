/*!
@file
Defines `boost::hana::detail::minimal::Monad`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_MONAD_HPP
#define BOOST_HANA_DETAIL_MINIMAL_MONAD_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Monad::flatten_mcd>
    struct Monad { };

    template <typename T, typename mcd, typename = operators::enable>
    struct monad_type {
        T value;
        using hana_datatype = Monad<mcd>;
    };

    template <typename mcd>
    struct make_monad_impl {
        template <typename X>
        constexpr auto operator()(X x) const
        { return monad_type<X, mcd>{x}; }
    };

    template <typename mcd = hana::Monad::flatten_mcd>
    constexpr make_monad_impl<mcd> monad{};
}} // end namespace detail::minimal

// Provided for convenience only.
template <typename mcd>
struct Comparable::instance<
    detail::minimal::Monad<mcd>, detail::minimal::Monad<mcd>
>
    : Comparable::equal_mcd
{
    template <typename M1, typename M2>
    static constexpr auto equal_impl(M1 x, M2 y)
    { return equal(x.value, y.value); }
};

template <typename Mcd>
struct Functor::instance<detail::minimal::Monad<Mcd>> : Functor::fmap_mcd {
    template <typename F, typename M>
    static constexpr auto fmap_impl(F f, M m)
    { return detail::minimal::monad<Mcd>(f(m.value)); }
};

template <typename Mcd>
struct Applicative::instance<detail::minimal::Monad<Mcd>> : Applicative::mcd {
    template <typename X>
    static constexpr auto lift_impl(X x)
    { return detail::minimal::monad<Mcd>(x); }

    template <typename AF, typename AX>
    static constexpr auto ap_impl(AF af, AX ax)
    { return detail::minimal::monad<Mcd>(af.value(ax.value)); }
};

template <>
struct Monad::instance<detail::minimal::Monad<Monad::flatten_mcd>>
    : Monad::flatten_mcd
{
    template <typename M>
    static constexpr auto flatten_impl(M m)
    { return m.value; }
};

template <>
struct Monad::instance<detail::minimal::Monad<Monad::bind_mcd>>
    : Monad::bind_mcd
{
    template <typename M, typename F>
    static constexpr auto bind_impl(M m, F f)
    { return f(m.value); }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_MONAD_HPP
