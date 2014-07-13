/*!
@file
Defines `boost::hana::Comparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_HPP
#define BOOST_HANA_COMPARABLE_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>
#include <boost/hana/detail/logical_fwd.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! The `Comparable` type class defines equality and inequality.
    struct Comparable {
        BOOST_HANA_BINARY_TYPECLASS(Comparable);
        struct equal_mcd;
        struct not_equal_mcd;
        struct laws;
    };

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
        return Comparable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::equal_impl(x, y);
    };

    //! Returns a `Logical` representing whether `x` is not equal to `y`.
    //! @method{Comparable}
    BOOST_HANA_CONSTEXPR_LAMBDA auto not_equal = [](auto x, auto y) {
        return Comparable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::not_equal_impl(x, y);
    };

    //! Minimal complete definition : `equal`
    struct Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr auto not_equal_impl(X x, Y y)
        { return not_(equal(x, y)); }
    };

    //! Minimal complete definition : `not_equal`
    struct Comparable::not_equal_mcd {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return not_(not_equal(x, y)); }
    };

    //! @details
    //! `equal` must define an equivalence relation. In other words, for all
    //! `a`, `b`, `c` of comparable data types,
    //! @code
    //!     a == a                          // Reflexivity
    //!     if a == b then b == a           // Symmetry
    //!     if a == b && b == c then a == c // Transitivity
    //! @endcode
    struct Comparable::laws {
        template <typename A, typename B, typename C>
        static constexpr auto check(A a, B b, C c) {
            auto implies = [](auto p, auto q) { return or_(not_(p), q); };
            return and_(
                equal(a, a),
                implies(equal(a, b), equal(b, a)),
                implies(and_(equal(a, b), equal(b, c)), equal(a, c))
            );
        }
    };

    //! @details
    //! Objects whose data type is the same as their C++ type and who have a
    //! valid `operator==` are automatically instances of `Comparable` by
    //! using that comparison.
    template <typename X, typename Y>
    struct Comparable::instance<X, Y, decltype((void)(*(X*)0 == *(Y*)0))>
        : Comparable::equal_mcd
    {
        static constexpr auto equal_impl(X x, Y y)
        { return x == y; }
    };

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

#include <boost/hana/logical.hpp>
