/*!
@file
Forward declares `boost::hana::Monoid`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_MONOID_HPP
#define BOOST_HANA_FWD_MONOID_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Monoid` type class is used for data types with an associative
    //! binary operation that has an identity.
    //!
    //! The method names refer to the monoid of numbers under addition, but
    //! there are many other instances such as sequences under concatenation.
    //! Some datatypes can be viewed as a monoid in more than one way, e.g.
    //! both addition and multiplication on numbers.
    //!
    //! ### Laws
    //! For all objects `x`, `y` and `z` whose data type `M` is a `Monoid`,
    //! the following laws must be satisfied:
    //! @code
    //!     plus(zero<M>(), x) == x                    // left zero
    //!     plus(x, zero<M>()) == x                    // right zero
    //!     plus(x, plus(y, z)) == plus(plus(x, y), z) // associativity
    //! @endcode
    struct Monoid {
        BOOST_HANA_BINARY_TYPECLASS(Monoid);
        struct mcd;
        template <typename I1, typename I2>
        struct integral_constant_mcd;
        template <typename T, typename U>
        struct default_instance;
    };

    //! Associative operation on a `Monoid`.
    //! @relates Monoid
    //!
    //! ### Example
    //! @snippet example/monoid.cpp plus
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto plus = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct plus_impl : plus_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct plus_impl<T, U, when<condition>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return Monoid::instance<T, U>::plus_impl(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    struct _plus {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return plus_impl<datatype_t<X>, datatype_t<Y>>::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _plus plus{};
#endif

    //! Identity of `plus`.
    //! @relates Monoid
    //!
    //! Since `Monoid` is a binary type class and `zero` is a nullary method,
    //! `zero<M>()` is dispatched to the type class instance for `M` and `M`,
    //! i.e. `Monoid::instance<M, M>`.
    //!
    //! @tparam M
    //! The data type (a `Monoid`) of the returned identity.
    //!
    //! ### Example
    //! @snippet example/monoid.cpp zero
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto zero = []() -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct zero_impl : zero_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct zero_impl<T, when<condition>> {
        static constexpr decltype(auto) apply() {
            return Monoid::instance<T, T>::zero_impl();
        }
    };

    template <typename M>
    struct _zero {
        constexpr decltype(auto) operator()() const {
            return zero_impl<M>::apply();
        }
    };

    template <typename M>
    constexpr _zero<M> zero{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MONOID_HPP
