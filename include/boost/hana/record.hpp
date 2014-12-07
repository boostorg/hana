/*!
@file
Defines `boost::hana::Record`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RECORD_HPP
#define BOOST_HANA_RECORD_HPP

#include <boost/hana/fwd/record.hpp>

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/product.hpp>
#include <boost/hana/searchable.hpp>

// provided instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `members`
    struct Record::mcd { };

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

    //! Two `Records` of the same data type `R` are equal if and only if
    //! all their members are equal. The members are compared in the
    //! same order as they appear in `members<R>`.
    template <typename R, typename _>
    struct equal_impl<R, R, _, when<is_a<Record, R>()>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const& x, Y const& y) {
            return all(members<R>, [&x, &y](auto&& member) -> decltype(auto) {
                auto accessor = second(detail::std::forward<decltype(member)>(member));
                return equal(accessor(x), accessor(y));
            });
        }
    };

    //! Minimal complete definition: `Record`
    template <typename R>
    struct Searchable::record_mcd : Searchable::mcd {
        template <typename X, typename Pred>
        static constexpr decltype(auto) find_impl(X&& x, Pred&& pred) {
            return fmap(
                find(members<R>, [&pred](auto&& member) -> decltype(auto) {
                    return pred(first(detail::std::forward<decltype(member)>(member)));
                }),
                [&x](auto&& member) -> decltype(auto) {
                    return second(detail::std::forward<decltype(member)>(member))(
                        detail::std::forward<X>(x)
                    );
                }
            );
        }

        template <typename X, typename Pred>
        static constexpr decltype(auto) any_impl(X const&, Pred&& pred) {
            return any(members<R>, [&pred](auto&& member) -> decltype(auto) {
                return pred(first(detail::std::forward<decltype(member)>(member)));
            });
        }
    };

    //! Searching a `Record` `r` is equivalent to searching `to<Map>(r)`.
    template <typename R>
    struct Searchable::instance<R, when<is_a<Record, R>()>>
        : Searchable::record_mcd<R>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RECORD_HPP
