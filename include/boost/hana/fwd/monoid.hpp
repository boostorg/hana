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
#include <boost/hana/core/method.hpp>
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
    //!
    //! Laws
    //! ----
    //! For all objects `x`, `y` and `z` whose data type `M` is a `Monoid`,
    //! the following laws must be satisfied:
    //! @code
    //!     plus(zero<M>(), x) == x                    // left zero
    //!     plus(x, zero<M>()) == x                    // right zero
    //!     plus(x, plus(y, z)) == plus(plus(x, y), z) // associativity
    //! @endcode
    //!
    //!
    //! Minimal complete definitions
    //! ----------------------------
    //! 1. `plus` and `zero`
    //! @todo
    //!
    //! 2. `operator+` and `static_cast`ability to 0
    //! Any two objects whose data types that can be added with the usual
    //! `operator+` have a definition of `plus` using this addition. Any
    //! data type `T` with a valid conversion (using `static_cast`) from
    //! `int` has a definition of `zero` equivalent to `static_cast<T>(0)`.
    //!
    //! @todo
    //! Use `to` instead of `static_cast` to provide the `zero` method.
    struct Monoid { };

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
    BOOST_HANA_BINARY_METHOD(plus_impl);

    struct _plus {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return dispatch<plus_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >>::apply(
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
    BOOST_HANA_METHOD(zero_impl);

    template <typename M>
    struct _zero {
        constexpr decltype(auto) operator()() const
        { return dispatch<zero_impl<M>>::apply(); }
    };

    template <typename M>
    constexpr _zero<M> zero{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_MONOID_HPP
