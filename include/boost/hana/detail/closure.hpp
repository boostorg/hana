/*!
@file
Defines `boost::hana::detail::closure`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CLOSURE_HPP
#define BOOST_HANA_DETAIL_CLOSURE_HPP

#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana { namespace detail {
    template <detail::std::size_t Index, typename X>
    struct closure_element { X x; };

    template <typename Indices, typename ...Xn>
    struct closure_impl;

    void swallow(...);

    template <detail::std::size_t ...Indices, typename ...Xn>
    struct closure_impl<detail::std::index_sequence<Indices...>, Xn...>
        : closure_element<Indices, Xn>...
    {
        closure_impl() = default;

        // Make sure the constructor is SFINAE-friendly.
        template <typename ...Yn, typename = decltype(swallow(
            (closure_element<Indices, Xn>{detail::std::declval<Yn>()}, 0)...
        ))>
        explicit constexpr closure_impl(Yn&& ...yn)
            : closure_element<Indices, Xn>{detail::std::forward<Yn>(yn)}...
        { }

        template <typename F>
        constexpr decltype(auto) operator()(F&& f) const& {
            return detail::std::forward<F>(f)(
                static_cast<closure_element<Indices, Xn> const&>(*this).x...
            );
        }

        template <typename F>
        constexpr decltype(auto) operator()(F&& f) & {
            return detail::std::forward<F>(f)(
                static_cast<closure_element<Indices, Xn>&>(*this).x...
            );
        }

        template <typename F>
        constexpr decltype(auto) operator()(F&& f) && {
            return detail::std::forward<F>(f)(
                static_cast<closure_element<Indices, Xn>&&>(*this).x...
            );
        }
    };

    template <typename ...Xn>
    using closure = closure_impl<
        detail::std::make_index_sequence<sizeof...(Xn)>, Xn...
    >;

    template <detail::std::size_t n, typename Xn>
    constexpr decltype(auto) get(closure_element<n, Xn> const& elt)
    { return elt.x; }

    template <detail::std::size_t n, typename Xn>
    constexpr decltype(auto) get(closure_element<n, Xn>& elt)
    { return elt.x; }

    template <detail::std::size_t n, typename Xn>
    constexpr decltype(auto) get(closure_element<n, Xn>&& elt)
    { return detail::std::move(elt.x); }
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_CLOSURE_HPP
