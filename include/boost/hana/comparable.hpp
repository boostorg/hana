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
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup typeclasses
    The `Comparable` type class defines equality and inequality.

    --------------------------------------------------------------------------

    ## Minimal complete definition
    `equal` or `not_equal`

    --------------------------------------------------------------------------

    ## Laws
    `equal` must define an equivalence relation. In other words, for all
    `a`, `b`, `c` of comparable data types,
    @f{align*}{
        a = a                                && \text{Reflexivity}      \\
        a = b \Rightarrow b = a              && \text{Symmetry}         \\
        a = b \wedge b = c \Rightarrow a = c && \text{Transitivity}
    @f}

    --------------------------------------------------------------------------

    @todo
    - Should all data types have a default implementation to allow arbitrary
      objects of different types to be compared? By default, two objects that
      don't share a common_type would be unequal, and otherwise we would use
      the common_type's comparison, if any.
    - Implement automatic checking of the laws for Comparable, if possible.
     */
    template <typename T, typename U, typename Enable = void>
    struct Comparable;

    /*!
    Returns a `Logical` representing whether `x` is equal to `y`.
    @method{Comparable}

    @internal
    ### Design choice: arity of `equal`
    It is a valid question whether `equal` should accept more than 2 arguments
    and have semantics matching those of Python's `==`. This is not supported
    right now for the following reasons:

    - It was not shown to be useful so far in the MPL11.
    - It does not make sense for `not_equal` to have an arity of more than 2,
      so only `equal` could maybe have those semantics.
    - Having a binary `equal` makes it possible to use currying.
    - `equal(x, y...)` can be implemented as `all(x == _, list(y...))`, which
      is pretty straightforward anyway.
     */
    BOOST_HANA_CONSTEXPR_LAMBDA auto equal = [](auto x, auto y) {
        return Comparable<datatype_t<decltype(x)>, datatype_t<decltype(y)>>::
               equal_impl(x, y);
    };

    //! Returns a `Logical` representing whether `x` is not equal to `y`.
    //! @method{Comparable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto not_equal = [](auto x, auto y) {
        return Comparable<datatype_t<decltype(x)>, datatype_t<decltype(y)>>::
               not_equal_impl(x, y);
    };

    template <>
    struct defaults<Comparable> {
        template <typename T, typename U, typename Enable = void>
        struct with : defaults<> {
            template <typename X, typename Y>
            static constexpr auto equal_impl(X x, Y y)
            { return !not_equal(x, y); }

            template <typename X, typename Y>
            static constexpr auto not_equal_impl(X x, Y y)
            { return !equal(x, y); }
        };
    };

    template <>
    struct instance<Comparable> {
        template <typename T, typename U, typename Enable = void>
        struct with : defaults<Comparable>::template with<T, U> {
            template <typename X, typename Y>
            static constexpr auto equal_impl(X x, Y y)
            { return x == y; }

            template <typename X, typename Y>
            static constexpr auto not_equal_impl(X x, Y y)
            { return x != y; }
        };
    };

    template <typename T, typename U, typename Enable>
    struct Comparable : instance<Comparable>::template with<T, U> { };

    namespace operators {
        //! Equivalent to `equal`.
        //! @method{boost::hana::Comparable}
        template <typename T, typename U>
        constexpr auto operator==(T t, U u)
        { return equal(t, u); }

        //! Equivalent to `not_equal`.
        //! @method{boost::hana::Comparable}
        template <typename T, typename U>
        constexpr auto operator!=(T t, U u)
        { return not_equal(t, u); }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_HPP
