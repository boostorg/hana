/*!
@file
Defines `boost::hana::Foldable::record_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_RECORD_MCD_HPP
#define BOOST_HANA_FOLDABLE_RECORD_MCD_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable/folds_mcd.hpp>
#include <boost/hana/product/product.hpp>
#include <boost/hana/record/record.hpp>


namespace boost { namespace hana {
    template <typename R>
    struct Foldable::record_mcd : Foldable::folds_mcd {
        template <typename Udt, typename S, typename F>
        static constexpr decltype(auto) foldl_impl(Udt&& udt, S&& s, F&& f) {
            return foldl(members<R>, detail::std::forward<S>(s),
                [&udt, f(detail::std::forward<F>(f))]
                (auto&& s, auto&& member) -> decltype(auto) {
                    return f(
                        detail::std::forward<decltype(s)>(s),
                        second(detail::std::forward<decltype(member)>(member))(
                            detail::std::forward<Udt>(udt)
                        )
                    );
                }
            );
        }

        template <typename Udt, typename S, typename F>
        static constexpr decltype(auto) foldr_impl(Udt&& udt, S&& s, F&& f) {
            return foldr(members<R>, detail::std::forward<S>(s),
                [&udt, f(detail::std::forward<F>(f))]
                (auto&& member, auto&& s) -> decltype(auto) {
                    return f(
                        second(detail::std::forward<decltype(member)>(member))(
                            detail::std::forward<Udt>(udt)
                        ),
                        detail::std::forward<decltype(s)>(s)
                    );
                }
            );
        }
    };

    //! Folding a `Record` `R` is equivalent to folding a list of its members,
    //! in the same order as they appear in `members<R>`.
    template <typename R>
    struct Foldable::instance<R, when<is_a<Record, R>()>>
        : Foldable::record_mcd<R>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_RECORD_MCD_HPP
