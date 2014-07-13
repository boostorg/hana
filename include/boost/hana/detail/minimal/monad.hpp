/*!
@file
Defines `boost::hana::detail::minimal::Monad`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_MONAD_HPP
#define BOOST_HANA_DETAIL_MINIMAL_MONAD_HPP

#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd>
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

template <typename mcd>
struct Functor::instance<detail::minimal::Monad<mcd>> : Functor::fmap_mcd {
    template <typename F, typename M>
    static constexpr auto fmap_impl(F f, M m)
    { return ap(lift<detail::minimal::Monad<mcd>>(f), m); }
};

//! @todo
//! Inherit from Applicative::mcd once Clang is fixed or Core is changed.
template <typename mcd>
struct Applicative::instance<detail::minimal::Monad<mcd>> {
    template <typename T>
    static constexpr auto lift_impl(T t)
    { return detail::minimal::monad<mcd>(t); }

    //! @todo
    //! Use the bind-based implementation which causes infinite recursion
    //! right now.
    template <typename AF, typename AX>
    static constexpr auto ap_impl(AF af, AX ax) {
        return detail::minimal::monad<mcd>(af.value(ax.value));
        // return bind(af, [=](auto f) {
        //     return bind(ax, [=](auto x) {
        //         return lift<detail::minimal::Monad<mcd>>(f(x));
        //     });
        // });
    }
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
