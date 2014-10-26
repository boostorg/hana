/*!
@file
Defines `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_HPP
#define BOOST_HANA_MAYBE_HPP

#include <boost/hana/fwd/maybe.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/logical.hpp>

// instances
#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/traversable.hpp>


namespace boost { namespace hana {
    //! Instance of `Comparable` for `Maybe`s.
    //!
    //! Two `Maybe`s are equal if and only if they are both empty or they
    //! both contain a value and those values are equal.
    //!
    //! ### Example
    //! @snippet example/maybe/comparable.cpp main
    template <>
    struct Comparable::instance<Maybe, Maybe> : Comparable::equal_mcd {
        template <typename T, typename U>
        static constexpr decltype(auto)
        equal_impl(_just<T> const& t, _just<U> const& u)
        { return equal(t.val, u.val); }

        static constexpr auto equal_impl(_nothing const&, _nothing const&)
        { return true_; }

        template <typename T, typename U>
        static constexpr auto equal_impl(T const&, U const&)
        { return false_; }
    };

    //! Instance of `Functor` for `Maybe`s.
    //!
    //! A `Maybe` can be seen as a `List` containing either one element
    //! (`just(x)`) or no elements at all (`nothing`). As such, `fmap` for
    //! `Maybe`s returns `nothing` when applied to `nothing` and `just(f(x))`
    //! when applied to `just(x)`.
    //!
    //! ### Example
    //! @snippet example/maybe/functor.cpp main
    template <>
    struct Functor::instance<Maybe> : Functor::fmap_mcd {
        template <typename M, typename F>
        static constexpr decltype(auto) fmap_impl(M&& m, F&& f) {
            return maybe(
                nothing,
                compose(just, detail::std::forward<F>(f)),
                detail::std::forward<M>(m)
            );
        }
    };

    //! Instance of `Applicative` for `Maybe`s.
    //!
    //! First, a value can be made optional with `lift<Maybe>`, which is
    //! equivalent to `just`. Second, one can feed an optional value to an
    //! optional function with `ap`, which will return `just(f(x))` if there
    //! is both a function _and_ a value, and `nothing` otherwise.
    //!
    //! ### Example
    //! @snippet example/maybe/applicative_simple.cpp main
    //!
    //! ### Example
    //! @include example/maybe/applicative.cpp
    template <>
    struct Applicative::instance<Maybe> : Applicative::mcd {
        template <typename X>
        static constexpr decltype(auto) lift_impl(X&& x)
        { return just(detail::std::forward<X>(x)); }

        template <typename Mf, typename Mx>
        static constexpr decltype(auto) ap_impl(Mf&& mf, Mx&& mx) {
            return maybe(
                nothing,
                [&mx](auto&& f) -> decltype(auto) {
                    return fmap(
                        detail::std::forward<Mx>(mx),
                        detail::std::forward<decltype(f)>(f)
                    );
                },
                detail::std::forward<Mf>(mf)
            );
        }
    };

    //! Instance of `Monad` for `Maybe`s.
    //!
    //! The `Maybe` `Monad` makes it easy to compose actions that might fail.
    //! One can feed an optional value if there is one into a function with
    //! `bind`, which will return `nothing` if there is no value. Finally,
    //! optional-optional values can have their redundant level of
    //! `Maybe`ness removed with `flatten`.
    //!
    //! ### Example
    //! @include example/maybe/monad.cpp
    template <>
    struct Monad::instance<Maybe> : Monad::flatten_mcd<Maybe> {
        template <typename MMX>
        static constexpr decltype(auto) flatten_impl(MMX&& mmx) {
            return maybe(nothing, id, detail::std::forward<MMX>(mmx));
        }
    };

    //! Instance of `Traversable` for `Maybe`s.
    //!
    //! Traversing `nothing` yields `nothing` in the new applicative, and
    //! traversing `just(x)` applies the function and maps `just` inside
    //! the resulting applicative.
    //!
    //! ### Example
    //! @snippet example/maybe/traversable.cpp main
    template <>
    struct Traversable::instance<Maybe> : Traversable::traverse_mcd {
        template <typename A, typename Mx, typename F>
        static constexpr decltype(auto) traverse_impl(Mx&& mx, F&& f) {
            return maybe(
                lift<A>(nothing),
                [&f](auto&& x) -> decltype(auto) {
                    return fmap(
                        detail::std::forward<F>(f)(
                            detail::std::forward<decltype(x)>(x)
                        ),
                        just
                    );
                },
                detail::std::forward<Mx>(mx)
            );
        }
    };

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

    //! Instance of `Searchable` for `Maybe`s.
    //!
    //! Searching a `Maybe` is equivalent to searching a list containing
    //! `x` for `just(x)` and an empty list for `nothing`.
    //!
    //! ### Example
    //! @snippet example/maybe/searchable.cpp main
    template <>
    struct Searchable::instance<Maybe> : Searchable::mcd {
        template <typename M, typename Pred>
        static constexpr decltype(auto) find_impl(M&& m, Pred&& p) {
            return maybe(nothing,
                [&p](auto&& x) -> decltype(auto) {
                    return eval_if(detail::std::forward<Pred>(p)(x),
                        [&x](auto _) -> decltype(auto) {
                            return just(detail::std::forward<decltype(x)>(x));
                        },
                        [](auto) { return nothing; }
                    );
                },
                detail::std::forward<M>(m)
            );
        }

        template <typename M, typename Pred>
        static constexpr decltype(auto) any_impl(M&& m, Pred&& p) {
            return maybe(false_,
                detail::std::forward<Pred>(p),
                detail::std::forward<M>(m)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_HPP
