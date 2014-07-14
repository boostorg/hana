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
#include <boost/hana/foldable.hpp>


namespace boost { namespace hana {
namespace detail { namespace minimal {
    template <typename mcd = hana::Foldable::lazy_foldr_mcd>
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

    template <typename mcd = hana::Foldable::lazy_foldr_mcd>
    constexpr make_foldable_impl<mcd> foldable{};
}} // end namespace detail::minimal

template <>
struct Foldable::instance<detail::minimal::Foldable<Foldable::lazy_foldr_mcd>>
    : Foldable::lazy_foldr_mcd
{
    struct helper {
        template <typename F, typename S, typename X, typename ...Xs>
        constexpr auto operator()(F f, S s, X x, Xs ...xs) const {
            return f(
                [=] { return x; },
                [=](auto ...nothing) {
                    static_assert(sizeof...(nothing) == 0, "");
                    return lazy_foldr(f, s, detail::minimal::foldable<Foldable::lazy_foldr_mcd>(xs..., nothing...));
                }
            );
        }

        template <typename F, typename S>
        constexpr auto operator()(F f, S s) const
        { return s; }
    };

    template <typename F, typename S, typename Xs>
    static constexpr auto lazy_foldr_impl(F f, S s, Xs xs) {
        return detail::unwrap(xs)([=](auto ...xs) { return helper{}(f, s, xs...); });
    }
};
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MINIMAL_FOLDABLE_HPP
