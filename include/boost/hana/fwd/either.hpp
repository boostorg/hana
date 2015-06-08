/*!
@file
Forward declares `boost::hana::Either`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EITHER_HPP
#define BOOST_HANA_FWD_EITHER_HPP

namespace boost { namespace hana {
    //! @ingroup group-details
    //! Tag representing a value with two possibilities.
    //!
    //! An `Either` contains either a left value or a right value, both of
    //! which may have different data types. An `Either` containing a left
    //! value `a` is represented as `left(a)`, and an `Either` containing a
    //! right value `b` is represented as `right(b)`.
    //!
    //! `Either` can sometimes be used to represent a value which is either
    //! correct or an error; by convention, `left` is used to hold an error
    //! and `right` is used to hold a correct value. A mnemonic is that
    //! _right_ also means _correct_.
    //!
    //!
    //! Modeled concepts
    //! ----------------
    //! 1. `Comparable` (operators provided)\n
    //! Two `Either`s are equal if and only if they both contain left values
    //! or they both contain right values and those values are equal.
    //! @snippet example/either.cpp comparable
    //!
    //! 2. `Orderable` (operators provided)\n
    //! `Either`s are ordered by considering any `left` value as less than any
    //! `right` value, and ordering two `left`s or two `right`s by ordering
    //! their content. In other words,
    //! @code
    //!     left(anything) < right(anything)
    //!     left(x)  < left(y)  if and only if x < y
    //!     right(x) < right(y) if and only if x < y
    //! @endcode
    //! Example:
    //! @snippet example/either.cpp orderable
    //!
    //! 3. `Functor`\n
    //! Since `Either` can contain one of two possible values of different
    //! data types and `transform` accepts a single function, `Either`'s instance
    //! of `Functor` can only map the function over one arbitrarily-defined
    //! side of the `Either`. Hence, mapping a function over an `Either e`
    //! does nothing if `e` contains a left value, and it applies the function
    //! if `e` contains a right value. In other words:
    //! @code
    //!     transform(left(x), f) == left(x)
    //!     transform(right(x), f) == right(f(x))
    //! @endcode
    //! Example:
    //! @snippet example/either.cpp functor
    //!
    //! 4. `Applicative`\n
    //! The instance of `Applicative` for `Either` follows naturally from
    //! the instance of `Functor`. Specifically,
    //! @code
    //!     ap(left(x), anything) == left(x)
    //!     ap(right(x), left(anything)) == right(x)
    //!     ap(right(f), right(x)) == right(f(x))
    //!     lift<Either>(x) == right(x)
    //! @endcode
    //! Example:
    //! @snippet example/either.applicative.cpp main
    //!
    //! 5. `Monad` (operators provided)\n
    //! The instance of `Monad` for `Either` follows naturally from
    //! the instance of `Applicative`. Specifically,
    //! @code
    //!     flatten(right(right(x))) == right(x)
    //!     flatten(anything else) == anything else
    //! @endcode
    //! Example:
    //! @snippet example/either.cpp monad
    //!
    //! 6. `Foldable`\n
    //! For the purpose of being folded, an `Either` is considered empty if
    //! it is a `left`, and it is considered equivalent to a one element list
    //! if it is a `right`.
    //! @snippet example/either.cpp foldable
    //!
    //! 7. `Traversable`\n
    //! Traversing a `left` with an `Applicative A` just lifts the `left` into
    //! the `Applicative`. Traversing a `right` will apply the function to the
    //! value inside the `right`, which effectively lifts the value into the
    //! `Applicative`, and then put back the value inside an `right` with
    //! `transform`. In other words,
    //! @code
    //!     traverse<A>(left(x), f) == lift<A>(left(x))
    //!     traverse<A>(right(y), f) == transform(f(y), right)
    //! @endcode
    //! Example:
    //! @snippet example/either.cpp traversable
    struct Either { };

    //! Create an `Either` containing the given left value.
    //! @relates Either
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/either.cpp left
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto left = [](auto&& x) {
        return unspecified-type;
    };
#else
    template <typename X>
    struct _left;

    struct _make_left {
        template <typename T>
        constexpr auto operator()(T&&) const;
    };

    constexpr _make_left left{};
#endif

    //! Create an `Either` containing the given right value.
    //! @relates Either
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/either.cpp right
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto right = [](auto&& x) {
        return unspecified-type;
    };
#else
    template <typename X>
    struct _right;

    struct _make_right {
        template <typename T>
        constexpr auto operator()(T&&) const;
    };

    constexpr _make_right right{};
#endif

    //! Apply one of two functions to the value inside an `Either`.
    //! @relates Either
    //!
    //! Specifically, `either` is such that
    //! @code
    //!     either(f, g, left(x)) == f(x)
    //!     either(f, g, right(x)) == g(x)
    //! @endcode
    //!
    //!
    //! @param f
    //! The function applied to the left value if `e` contains a left value.
    //!
    //! @param g
    //! The function applied to the right value if `e` contains a right value.
    //!
    //! @param e
    //! The `Either` value to analyze.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/either.cpp either
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto either = [](auto&& f, auto&& g, auto&& e) -> decltype(auto) {
        if (e is a left(x))
            return forwarded(f)(forwarded(x));
        else if (e is a right(y))
            return forwarded(g)(forwarded(y));
    };
#else
    struct _either {
        template <typename F, typename G, typename E>
        constexpr decltype(auto) operator()(F&& f, G&& g, E&& e) const {
            return static_cast<E&&>(e).go(
                static_cast<F&&>(f),
                static_cast<G&&>(g)
            );
        }
    };

    constexpr _either either{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EITHER_HPP
