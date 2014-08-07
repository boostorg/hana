/*!
@file
Forward declares `boost::hana::Monoid`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_MONOID_HPP
#define BOOST_HANA_MONOID_MONOID_HPP

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>


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
    //!     plus(zero<M>, x) == x                      // left zero
    //!     plus(x, zero<M>) == x                      // right zero
    //!     plus(x, plus(y, z)) == plus(plus(x, y), z) // associativity
    //! @endcode
    struct Monoid {
        BOOST_HANA_BINARY_TYPECLASS(Monoid);

        //! Minimal complete definition : `zero` and `plus`
        struct mcd { };

        struct laws;
    };

    //! Associative operation on a `Monoid`.
    //! @relates Monoid
    //!
    //! ### Example
    //! @snippet example/monoid/plus.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto plus = [](auto x, auto y) {
        return Monoid::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::plus_impl(x, y);
    };

    //! Identity of `plus`.
    //! @relates Monoid
    //!
    //! Since `Monoid` is a binary type class and `zero` is a nullary method,
    //! `zero<M>` is dispatched to the type class instance for `M` and `M`,
    //! i.e. `Monoid::instance<M, M>`.
    //!
    //! @tparam M
    //! The data type (a `Monoid`) of the returned identity.
    //!
    //! ### Example
    //! @snippet example/monoid/zero.cpp main
    template <typename M>
    constexpr auto zero = Monoid::instance<M, M>::zero_impl();

    //! Instance of `Monoid` for objects with numeric data types.
    //!
    //! Any two objects whose data types can be added with the usual
    //! `operator+` and for which a valid conversion from `int` exists
    //! (for both data types) naturally form an additive `Monoid`, with
    //! `0` being the identity and the usual `operator+` being the
    //! associative operation.
    template <typename X, typename Y>
    struct Monoid::instance<X, Y, when_valid<decltype((void)(
        static_cast<X>(0),
        static_cast<Y>(0),
        *(X*)0 + *(Y*)0
    ))>> : Monoid::mcd {
        static constexpr auto plus_impl(X x, Y y)
        { return x + y; }

        // Will never be used with two different `X` and `Y` anyway.
        static constexpr auto zero_impl()
        { return static_cast<X>(0); }
    };

    namespace operators {
        //! Equivalent to `plus`.
        //! @relates boost::hana::Monoid
        template <typename C1, typename C2>
        constexpr auto operator+(C1 c1, C2 c2)
        { return plus(c1, c2); }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_MONOID_HPP
