/*!
@file
Defines `boost::hana::Comparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_HPP
#define BOOST_HANA_COMPARABLE_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    /*!
    @typeclass{Comparable}
    @{
    The `Comparable` type class defines equality and inequality.

    @mcd
    `equal` or `not_equal`

    @laws
    `equal` must define an equivalence relation. In other words, for all
    `a`, `b`, `c` of comparable data types,
    @f{align*}{
        a = a                                && \text{Reflexivity}      \\
        a = b \Rightarrow b = a              && \text{Symmetry}         \\
        a = b \wedge b = c \Rightarrow a = c && \text{Transitivity}
    @f}

    @todo
    - Should all data types have a default implementation to allow arbitrary
    objects of different types to be compared? See Boost.Fusion for an
    example of this.
    - Document `==` and `!=` operators.
    - Should `equal` and `not_equal` allow more than 2 arguments as in the
    MPL11?
    - Implement automatic checking of the laws for Comparable, if possible.
     */
    template <typename T, typename U>
    struct Comparable {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return x == y; }

        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y)
        { return x != y; }
    };

    //! Returns whether `x` is equal to `y`.
    //! @method{Comparable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto equal = [](auto x, auto y) {
        return Comparable<datatype_t<decltype(x)>, datatype_t<decltype(y)>>::
               equal_impl(x, y);
    };

    //! Returns whether `x` is not equal to `y`.
    //! @method{Comparable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto not_equal = [](auto x, auto y) {
        return Comparable<datatype_t<decltype(x)>, datatype_t<decltype(y)>>::
               not_equal_impl(x, y);
    };

    //! @}

    namespace operators {
        template <typename T, typename U>
        constexpr auto operator==(T t, U u)
        { return equal(t, u); }

        template <typename T, typename U>
        constexpr auto operator!=(T t, U u)
        { return not_equal(t, u); }
    }

    template <>
    struct defaults<Comparable> {
        template <typename T, typename U>
        static constexpr auto equal_impl(T t, U u)
        { return !not_equal(t, u); }

        template <typename T, typename U>
        static constexpr auto not_equal_impl(T t, U u)
        { return !equal(t, u); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_HPP
