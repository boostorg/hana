/*!
@file
Defines `boost::hana::Foldable::unpack_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_UNPACK_MCD_HPP
#define BOOST_HANA_FOLDABLE_UNPACK_MCD_HPP

#include <boost/hana/foldable/foldable.hpp>

#include <boost/hana/detail/variadic/foldl.hpp>
#include <boost/hana/detail/variadic/foldl1.hpp>
#include <boost/hana/detail/variadic/foldr.hpp>
#include <boost/hana/detail/variadic/foldr1.hpp>
#include <boost/hana/detail/variadic/for_each.hpp>
#include <boost/hana/foldable/folds_mcd.hpp>
#include <boost/hana/integral.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `unpack`
    struct Foldable::unpack_mcd : Foldable::folds_mcd {
        template <typename Xs, typename S, typename F>
        static constexpr auto foldl_impl(Xs xs, S s, F f) {
            return unpack(xs, [=](auto ...xs) {
                return detail::variadic::foldl(f, s, xs...);
            });
        }

        template <typename Xs, typename F>
        static constexpr auto foldl1_impl(Xs xs, F f) {
            return unpack(xs, [=](auto ...xs) {
                return detail::variadic::foldl1(f, xs...);
            });
        }

        template <typename Xs, typename S, typename F>
        static constexpr auto foldr_impl(Xs xs, S s, F f) {
            return unpack(xs, [=](auto ...xs) {
                return detail::variadic::foldr(f, s, xs...);
            });
        }

        template <typename Xs, typename F>
        static constexpr auto foldr1_impl(Xs xs, F f) {
            return unpack(xs, [=](auto ...xs) {
                return detail::variadic::foldr1(f, xs...);
            });
        }

        template <typename Xs, typename F>
        static constexpr auto for_each_impl(Xs xs, F f) {
            return unpack(xs, [=](auto ...xs) {
                return detail::variadic::for_each(f, xs...);
            });
        }

        template <typename Xs>
        static constexpr auto length_impl(Xs xs) {
            return unpack(xs, [](auto const& ...xs) {
                return size_t<sizeof...(xs)>;
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_UNPACK_MCD_HPP
