/*!
@file
Defines `boost::hana::Either`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EITHER_HPP
#define BOOST_HANA_EITHER_HPP

#include <boost/hana/fwd/either.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/traversable.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // left
    //////////////////////////////////////////////////////////////////////////
    template <typename X, typename>
    struct _left {
        X value;

        struct hana { using datatype = Either; };

        template <typename F, typename G>
        constexpr decltype(auto) go(F&& f, G const&) const&
        { return detail::std::forward<F>(f)(value); }

        template <typename F, typename G>
        constexpr decltype(auto) go(F f, G const&) &
        { return detail::std::forward<F>(f)(value); }

        template <typename F, typename G>
        constexpr decltype(auto) go(F&& f, G const&) &&
        { return detail::std::forward<F>(f)(detail::std::move(value)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // right
    //////////////////////////////////////////////////////////////////////////
    template <typename X, typename>
    struct _right {
        X value;

        struct hana { using datatype = Either; };

        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) const&
        { return detail::std::forward<G>(g)(value); }

        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) &
        { return detail::std::forward<G>(g)(value); }

        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) &&
        { return detail::std::forward<G>(g)(detail::std::move(value)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct enabled_operators<Either>
        : Comparable, Orderable, Monad
    { };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Comparable(Either)>
        : detail::std::true_type
    { };

    template <>
    struct equal_impl<Either, Either> {
        template <template <typename ...> class E, typename T, typename U>
        static constexpr decltype(auto) apply(E<T> const& x, E<U> const& y)
        { return hana::equal(x.value, y.value); }

        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Orderable(Either)>
        : detail::std::true_type
    { };

    template <>
    struct less_impl<Either, Either> {
        template <template <typename ...> class E, typename T, typename U>
        static constexpr decltype(auto) apply(E<T> const& x, E<U> const& y)
        { return hana::less(x.value, y.value); }

        template <typename T, typename U>
        static constexpr auto apply(_left<T> const&, _right<U> const&)
        { return true_; }

        template <typename T, typename U>
        static constexpr auto apply(_right<T> const&, _left<U> const&)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Functor(Either)>
        : detail::std::true_type
    { };

    template <>
    struct transform_impl<Either> {
        template <typename E, typename F>
        static constexpr decltype(auto) apply(E&& e, F&& f) {
            return hana::either(left,
                hana::compose(right, detail::std::forward<F>(f)),
                detail::std::forward<E>(e)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Applicative(Either)>
        : detail::std::true_type
    { };

    template <>
    struct lift_impl<Either> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return hana::right(detail::std::forward<X>(x)); }
    };

    template <>
    struct ap_impl<Either> {
        template <typename E, typename X>
        static constexpr decltype(auto) apply(E&& e, X&& x) {
            return hana::either(left,
                hana::partial(transform, detail::std::forward<X>(x)),
                detail::std::forward<E>(e)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Monad(Either)>
        : detail::std::true_type
    { };

    template <>
    struct flatten_impl<Either> {
        template <typename E>
        static constexpr decltype(auto) apply(E&& e)
        { return hana::either(left, id, detail::std::forward<E>(e)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Foldable(Either)>
        : detail::std::true_type
    { };

    template <>
    struct unpack_impl<Either> {
        template <typename T, typename F>
        static constexpr decltype(auto) apply(_left<T> const&, F&& f)
        { return detail::std::forward<F>(f)(); }


        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T> const& x, F&& f)
        { return detail::std::forward<F>(f)(x.value); }

        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>& x, F&& f)
        { return detail::std::forward<F>(f)(x.value); }

        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>&& x, F&& f)
        { return detail::std::forward<F>(f)(detail::std::move(x.value)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Traversable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct models<Traversable(Either)>
        : detail::std::true_type
    { };

    template <>
    struct traverse_impl<Either> {
        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_left<T> const& e, F&& f)
        { return lift<A>(e); }
        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_left<T>& e, F&& f)
        { return lift<A>(e); }
        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_left<T>&& e, F&& f)
        { return lift<A>(detail::std::move(e)); }

        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_right<T> const& e, F&& f) {
            return hana::transform(detail::std::forward<F>(f)(e.value), right);
        }
        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>& e, F&& f) {
            return hana::transform(detail::std::forward<F>(f)(e.value), right);
        }
        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>&& e, F&& f) {
            return hana::transform(detail::std::forward<F>(f)(
                                        detail::std::move(e.value)), right);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_HPP
