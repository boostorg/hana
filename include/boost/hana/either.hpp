/*!
@file
Defines `boost::hana::Either`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EITHER_HPP
#define BOOST_HANA_EITHER_HPP

#include <boost/hana/fwd/either.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/applicative.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/concept/functor.hpp>
#include <boost/hana/concept/monad.hpp>
#include <boost/hana/concept/orderable.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/monad.hpp>
#include <boost/hana/detail/operators/orderable.hpp>
#include <boost/hana/detail/closure.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>

#include <type_traits>


namespace boost { namespace hana {
    namespace detail {
        template <>
        struct comparable_operators<Either>
        { static constexpr bool value = true; };
        template <>
        struct orderable_operators<Either>
        { static constexpr bool value = true; };
        template <>
        struct monad_operators<Either>
        { static constexpr bool value = true; };
    }

    //////////////////////////////////////////////////////////////////////////
    // left
    //////////////////////////////////////////////////////////////////////////
    template <typename X>
    struct _left : operators::adl, detail::closure<X> {
        _left(_left const&) = default;
        _left(_left&) = default;
        _left(_left&&) = default;
        using detail::closure<X>::closure;
        using hana = _left;
        using datatype = Either;

        template <typename F, typename G>
        constexpr decltype(auto) go(F&& f, G const&) const&
        { return static_cast<F&&>(f)(detail::get<0>(*this)); }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename F, typename G>
        constexpr decltype(auto) go(F&& f, G const&) &
        { return static_cast<F&&>(f)(detail::get<0>(*this)); }
#endif

        template <typename F, typename G>
        constexpr decltype(auto) go(F&& f, G const&) && {
            return static_cast<F&&>(f)(
                detail::get<0>(static_cast<_left&&>(*this))
            );
        }
    };

    //! @cond
    template <typename T>
    constexpr auto _make_left::operator()(T&& t) const {
        return _left<typename std::decay<T>::type>{
            static_cast<T&&>(t)
        };
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // right
    //////////////////////////////////////////////////////////////////////////
    template <typename X>
    struct _right : operators::adl, detail::closure<X> {
        using detail::closure<X>::closure;
        _right(_right const&) = default;
        _right(_right&) = default;
        _right(_right&&) = default;
        using hana = _right;
        using datatype = Either;

        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) const&
        { return static_cast<G&&>(g)(detail::get<0>(*this)); }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) &
        { return static_cast<G&&>(g)(detail::get<0>(*this)); }
#endif

        template <typename F, typename G>
        constexpr decltype(auto) go(F const&, G&& g) && {
            return static_cast<G&&>(g)(
                detail::get<0>(static_cast<_right&&>(*this))
            );
        }
    };

    //! @cond
    template <typename T>
    constexpr auto _make_right::operator()(T&& t) const {
        return _right<typename std::decay<T>::type>{
            static_cast<T&&>(t)
        };
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Either, Either> {
        template <template <typename ...> class E, typename T, typename U>
        static constexpr decltype(auto) apply(E<T> const& x, E<U> const& y)
        { return hana::equal(detail::get<0>(x), detail::get<0>(y)); }

        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&)
        { return false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<Either, Either> {
        template <template <typename ...> class E, typename T, typename U>
        static constexpr decltype(auto) apply(E<T> const& x, E<U> const& y)
        { return hana::less(detail::get<0>(x), detail::get<0>(y)); }

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
    struct transform_impl<Either> {
        template <typename E, typename F>
        static constexpr decltype(auto) apply(E&& e, F&& f) {
            return hana::either(left,
                hana::compose(right, static_cast<F&&>(f)),
                static_cast<E&&>(e)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct lift_impl<Either> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return hana::right(static_cast<X&&>(x)); }
    };

    template <>
    struct ap_impl<Either> {
        template <typename E, typename X>
        static constexpr decltype(auto) apply(E&& e, X&& x) {
            return hana::either(left,
                hana::partial(transform, static_cast<X&&>(x)),
                static_cast<E&&>(e)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct flatten_impl<Either> {
        template <typename E>
        static constexpr decltype(auto) apply(E&& e)
        { return hana::either(left, id, static_cast<E&&>(e)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<Either> {
        template <typename T, typename F>
        static constexpr decltype(auto) apply(_left<T> const&, F&& f)
        { return static_cast<F&&>(f)(); }


        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T> const& x, F&& f)
        { return static_cast<F&&>(f)(detail::get<0>(x)); }

        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>& x, F&& f)
        { return static_cast<F&&>(f)(detail::get<0>(x)); }

        template <typename T, typename F>
        static constexpr decltype(auto) apply(_right<T>&& x, F&& f) {
            return static_cast<F&&>(f)(
                detail::get<0>(static_cast<_right<T>&&>(x))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_HPP
