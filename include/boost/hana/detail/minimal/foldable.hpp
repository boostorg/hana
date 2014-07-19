/*!
@file
Defines `boost::hana::detail::minimal::Foldable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MINIMAL_FOLDABLE_HPP
#define BOOST_HANA_DETAIL_MINIMAL_FOLDABLE_HPP

#include <boost/hana/detail/wrap.hpp>
#include <boost/hana/foldable/mcd.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Foldable::mcd>
    struct Foldable { };

    template <typename mcd>
    struct make_foldable_impl {
        template <typename ...Xs>
        constexpr auto operator()(Xs ...xs) const {
            return detail::wrap<Foldable<mcd>>([=](auto f) {
                return f(xs...);
            });
        }
    };

    template <typename mcd = hana::Foldable::mcd>
    constexpr make_foldable_impl<mcd> foldable{};
}} // end namespace detail::minimal

template <>
struct Foldable::instance<detail::minimal::Foldable<Foldable::mcd>>
    : Foldable::mcd
{
    template <typename F, typename S, typename X, typename ...Xs>
    static constexpr auto foldr_helper(F f, S s, X x, Xs ...xs)
    { return f(x, foldr_helper(f, s, xs...)); }

    template <typename F, typename S>
    static constexpr auto foldr_helper(F f, S s)
    { return s; }

    template <typename F, typename S, typename Xs>
    static constexpr auto foldr_impl(F f, S s, Xs xs) {
        return detail::unwrap(xs)([=](auto ...xs) {
            return foldr_helper(f, s, xs...);
        });
    }

    template <typename F, typename S, typename X, typename ...Xs>
    static constexpr auto foldl_helper(F f, S s, X x, Xs ...xs)
    { return foldl_helper(f, f(s, x), xs...); }

    template <typename F, typename S>
    static constexpr auto foldl_helper(F f, S s)
    { return s; }

    template <typename F, typename S, typename Xs>
    static constexpr auto foldl_impl(F f, S s, Xs xs) {
        return detail::unwrap(xs)([=](auto ...xs) {
            return foldl_helper(f, s, xs...);
        });
    }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_FOLDABLE_HPP
