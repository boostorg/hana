/*!
@file
Internal header to break cyclic dependencies.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_DETAIL_MONOID_FWD_HPP
#define BOOST_HANA_MONOID_DETAIL_MONOID_FWD_HPP

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    namespace monoid_detail { namespace operators { struct enable { }; }}

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

        using operators = monoid_detail::operators::enable;
    };

    //! Associative operation on a `Monoid`.
    //! @relates Monoid
    //!
    //! ### Example
    //! @snippet example/monoid.cpp plus
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
    //! @snippet example/monoid.cpp zero
    template <typename M>
    constexpr auto zero = Monoid::instance<M, M>::zero_impl();

    namespace monoid_detail { namespace operators {
        //! Equivalent to `plus`.
        //! @relates boost::hana::Monoid
        template <typename C1, typename C2>
        constexpr auto operator+(C1 c1, C2 c2)
        { return plus(c1, c2); }
    }}
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_DETAIL_MONOID_FWD_HPP
