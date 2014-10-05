/*!
@file
Defines the instance of `boost::hana::Foldable` for `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_FOLDABLE_HPP
#define BOOST_HANA_MAYBE_FOLDABLE_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable/folds_mcd.hpp>
#include <boost/hana/maybe/maybe.hpp>


namespace boost { namespace hana {
    //! Instance of `Foldable` for `Maybe`s.
    //!
    //! Folding a `Maybe` is equivalent to folding a `List` containing either
    //! no elements (for `nothing`) or `x` (for `just(x)`). The `Foldable`
    //! instance follows from that.
    //!
    //! ### Example
    //! @snippet example/maybe/foldable.cpp main
    template <>
    struct Foldable::instance<Maybe>
        : Foldable::folds_mcd
    {
        template <typename M, typename S, typename F>
        static constexpr decltype(auto) foldr_impl(M&& m, S&& s, F&& f) {
            // While it _seems_ like we're forwarding `s` twice, `s` may be
            // moved from in only the branch which is actually executed.
            return maybe(
                detail::std::forward<S>(s),
                [&f, &s](auto&& x) -> decltype(auto) {
                    return detail::std::forward<F>(f)(
                        detail::std::forward<decltype(x)>(x),
                        detail::std::forward<S>(s)
                    );
                },
                detail::std::forward<M>(m)
            );
        }

        template <typename M, typename S, typename F>
        static constexpr decltype(auto) foldl_impl(M&& m, S&& s, F&& f) {
            // The same comment as above applies for the forwarding of `s`.
            return maybe(
                detail::std::forward<S>(s),
                [&f, &s](auto&& x) -> decltype(auto) {
                    return detail::std::forward<F>(f)(
                        detail::std::forward<S>(s),
                        detail::std::forward<decltype(x)>(x)
                    );
                },
                detail::std::forward<M>(m)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_FOLDABLE_HPP
