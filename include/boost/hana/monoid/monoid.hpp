/*!
@file
Forward declares `boost::hana::Monoid`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_MONOID_HPP
#define BOOST_HANA_MONOID_MONOID_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
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
    //!     plus(zero<M>, x) == x                      // left zero
    //!     plus(x, zero<M>) == x                      // right zero
    //!     plus(x, plus(y, z)) == plus(plus(x, y), z) // associativity
    //! @endcode
    struct Monoid {
        BOOST_HANA_BINARY_TYPECLASS(Monoid);
        struct mcd;
    };

    //! Associative operation on a `Monoid`.
    //! @relates Monoid
    //!
    //! ### Example
    //! @snippet example/monoid.cpp plus
    BOOST_HANA_CONSTEXPR_LAMBDA auto plus = [](auto&& x, auto&& y) -> decltype(auto) {
        return Monoid::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::plus_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
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
    //! @snippet example/monoid.cpp zero
    template <typename M>
    BOOST_HANA_CONSTEXPR_LAMBDA auto zero = Monoid::instance<M, M>::zero_impl();
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_MONOID_HPP
