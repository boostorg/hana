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
    struct left_t : operators::adl, detail::closure<X> {
        left_t(left_t const&) = default;
        left_t(left_t&) = default;
        left_t(left_t&&) = default;
        using detail::closure<X>::closure;
        using hana = left_t;
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
                detail::get<0>(static_cast<left_t&&>(*this))
            );
        }
    };

    //! @cond
    template <typename T>
    constexpr auto make_left_t::operator()(T&& t) const {
        return left_t<typename std::decay<T>::type>{
            static_cast<T&&>(t)
        };
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // right
    //////////////////////////////////////////////////////////////////////////
    template <typename X>
    struct right_t : operators::adl, detail::closure<X> {
        using detail::closure<X>::closure;
        right_t(right_t const&) = default;
        right_t(right_t&) = default;
        right_t(right_t&&) = default;
        using hana = right_t;
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
                detail::get<0>(static_cast<right_t&&>(*this))
            );
        }
    };

    //! @cond
    template <typename T>
    constexpr auto make_right_t::operator()(T&& t) const {
        return right_t<typename std::decay<T>::type>{
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
        { return hana::false_; }
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
        static constexpr auto apply(left_t<T> const&, right_t<U> const&)
        { return hana::true_; }

        template <typename T, typename U>
        static constexpr auto apply(right_t<T> const&, left_t<U> const&)
        { return hana::false_; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct transform_impl<Either> {
        template <typename E, typename F>
        static constexpr decltype(auto) apply(E&& e, F&& f) {
            return hana::either(hana::left,
                hana::compose(hana::right, static_cast<F&&>(f)),
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
            return hana::either(hana::left,
                hana::partial(hana::transform, static_cast<X&&>(x)),
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
        { return hana::either(hana::left, hana::id, static_cast<E&&>(e)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<Either> {
        template <typename T, typename F>
        static constexpr decltype(auto) apply(left_t<T> const&, F&& f)
        { return static_cast<F&&>(f)(); }


        template <typename T, typename F>
        static constexpr decltype(auto) apply(right_t<T> const& x, F&& f)
        { return static_cast<F&&>(f)(detail::get<0>(x)); }

        template <typename T, typename F>
        static constexpr decltype(auto) apply(right_t<T>& x, F&& f)
        { return static_cast<F&&>(f)(detail::get<0>(x)); }

        template <typename T, typename F>
        static constexpr decltype(auto) apply(right_t<T>&& x, F&& f) {
            return static_cast<F&&>(f)(
                detail::get<0>(static_cast<right_t<T>&&>(x))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EITHER_HPP
