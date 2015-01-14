/*!
@file
Forward declares `boost::hana::Comparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_COMPARABLE_HPP
#define BOOST_HANA_FWD_COMPARABLE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Comparable` concept defines equality and inequality.
    //!
    //!
    //! Laws
    //! ----
    //! `equal` must define an [equivalence relation][1], and `not_equal` must
    //! be its negation. In other words, for all `a`, `b`, `c` of comparable
    //! data types,
    //! @code
    //!     a == a                          // Reflexivity
    //!     if a == b then b == a           // Symmetry
    //!     if a == b && b == c then a == c // Transitivity
    //!     a != b is equivalent to !(a == b)
    //! @endcode
    //!
    //!
    //! Minimal complete definitions
    //! ----------------------------
    //! 1. `equal` or `not_equal`
    //! If either `equal` or `not_equal` is defined for two data types `T`
    //! and `U`, then those data types together model `Comparable`.
    //! The default implementation for `equal` and `not_equal` are:
    //! @code
    //!     equal(x, y) == not_(not_equal(x, y))
    //!     not_equal(x, y) == not_(equal(x, y))
    //! @endcode
    //!
    //! 2. `operator==`
    //! Any two objects whose __data types__ can be compared using `operator==`
    //! are taken to model `Comparable` using that comparison as a basis for
    //! the `equal` method.
    //!
    //! 3. A common data type that is `Comparable`
    //! If `T` and `U` are two _distinct_ data types with a common data type
    //! `C` (as determined by the `common` metafunction) that is `Comparable`,
    //! then comparison is done by converting both objects to `C` and then
    //! performing the comparison on the new objects.
    //!
    //!
    //! Important note: special behavior of `equal`
    //! -------------------------------------------
    //! In the context of programming with heterogeneous values, it is useful
    //! to have unrelated objects compare `false` instead of triggering an
    //! error. For this reason, `equal` adopts a special behavior when no
    //! minimal complete definition is provided at all for two data types
    //! `T` and `U`: When `T` and `U` are unrelated (i.e. they do not share
    //! a common data type), comparing objects of `T` and `U` yields a
    //! compile-time false value. However, when `T` is the same as `U`, a
    //! compile-time error is triggered instead. This design choice aims to
    //! make `equal` more widely useful, while still rejecting usage patterns
    //! that are most likely programming errors.
    //!
    //!
    //! Operators
    //! ---------
    //! For convenience, the following operators are provided as an
    //! equivalent way of calling the corresponding method:
    //! @code
    //!     == -> equal
    //!     != -> not_equal
    //! @endcode
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Equivalence_relation#Definition
    struct Comparable { };

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
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto equal = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_BINARY_METHOD(equal_impl);

    struct _equal {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return dispatch<equal_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >>::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _equal equal{};
#endif

    //! Returns a `Logical` representing whether `x` is not equal to `y`.
    //! @relates Comparable
    //!
    //! @param x, y
    //! Two objects to compare for inequality.
    //!
    //! ### Example
    //! @snippet example/comparable.cpp not_equal
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto not_equal = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    BOOST_HANA_BINARY_METHOD(not_equal_impl);

    struct _not_equal {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            return dispatch<not_equal_impl<
                typename datatype<X>::type, typename datatype<Y>::type
            >>::apply(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    constexpr _not_equal not_equal{};
#endif

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
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto comparing = [](auto&& f) {
        return [perfect-capture](auto&& x, auto&& y) -> decltype(auto) {
            return equal(f(forwarded(x)), f(forwarded(y)));
        };
    };
#else
    template <typename F>
    struct _comparing {
        F f;

        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const& {
            return equal(
                f(detail::std::forward<X>(x)),
                f(detail::std::forward<Y>(y))
            );
        }

        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) & {
            return equal(
                f(detail::std::forward<X>(x)),
                f(detail::std::forward<Y>(y))
            );
        }
    };

    constexpr detail::create<_comparing> comparing{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_COMPARABLE_HPP
