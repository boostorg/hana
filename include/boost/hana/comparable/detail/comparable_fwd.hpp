/*!
@file
Forward declaration header to break cyclic dependencies.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_DETAIL_COMPARABLE_FWD_HPP
#define BOOST_HANA_COMPARABLE_DETAIL_COMPARABLE_FWD_HPP

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    namespace comparable_detail { namespace operators { struct enable { }; }}

    //! @ingroup group-typeclasses
    //! The `Comparable` type class defines equality and inequality.
    //!
    //! @anchor equivalence_relation
    //! ### Laws
    //! `equal` must define an equivalence relation. In other words, for all
    //! `a`, `b`, `c` of comparable data types,
    //! @code
    //!     a == a                          // Reflexivity
    //!     if a == b then b == a           // Symmetry
    //!     if a == b && b == c then a == c // Transitivity
    //! @endcode
    struct Comparable {
        BOOST_HANA_BINARY_TYPECLASS(Comparable);
        struct equal_mcd;
        struct not_equal_mcd;
        struct laws;

        using operators = comparable_detail::operators::enable;

        template <typename T, typename U>
        struct default_instance;
    };

    //! Returns a `Logical` representing whether `x` is equal to `y`.
    //! @relates Comparable
    //!
    //! @param x, y
    //! Two objects to compare for equality.
    //!
    //! ### Example
    //! @snippet example/comparable.cpp equal
    //!
    //! @internal
    //! ### Rationale for the arity of `equal`
    //! It is a valid question whether `equal` should accept more than 2
    //! arguments and have semantics matching those of Python's `==`. This
    //! is not supported right now for the following reasons:
    //!
    //! - It was not shown to be useful so far in the MPL11.
    //! - It does not make sense for `not_equal` to have an arity of more
    //!   than 2, so only `equal` could maybe have those semantics.
    //! - Having a binary `equal` makes it possible to use currying.
    //! - `equal(x, y...)` can be implemented as `all(list(y...), x == _)`,
    //!   which is pretty straightforward anyway.
    //! @endinternal
    BOOST_HANA_CONSTEXPR_LAMBDA auto equal = [](auto x, auto y) {
        return Comparable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::equal_impl(x, y);
    };

    //! Returns a `Logical` representing whether `x` is not equal to `y`.
    //! @relates Comparable
    //!
    //! @param x, y
    //! Two objects to compare for inequality.
    //!
    //! ### Example
    //! @snippet example/comparable.cpp not_equal
    BOOST_HANA_CONSTEXPR_LAMBDA auto not_equal = [](auto x, auto y) {
        return Comparable::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::not_equal_impl(x, y);
    };

    //! Returns a function performing `equal` after applying a transformation
    //! to both arguments.
    //! @relates Comparable
    //!
    //! This is not a method of the `Comparable` type class, but just a
    //! convenience function provided with it. Also note that
    //! @code
    //!     comparing(f) == equal ^on^ f
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/comparable.cpp comparing
    BOOST_HANA_CONSTEXPR_LAMBDA auto comparing = [](auto f) {
        return [=](auto x, auto y) {
            return equal(f(x), f(y));
        };
    };

    namespace comparable_detail {
        namespace operators {
            //! Equivalent to `equal`.
            //! @relates boost::hana::Comparable
            template <typename T, typename U>
            constexpr auto operator==(T t, U u)
            { return equal(t, u); }

            //! Equivalent to `not_equal`.
            //! @relates boost::hana::Comparable
            template <typename T, typename U>
            constexpr auto operator!=(T t, U u)
            { return not_equal(t, u); }
        }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_DETAIL_COMPARABLE_FWD_HPP
