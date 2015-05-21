/*!
@file
Forward declares `boost::hana::Comparable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_COMPARABLE_HPP
#define BOOST_HANA_FWD_COMPARABLE_HPP

#include <boost/hana/detail/create.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/operators.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Comparable` concept defines equality and inequality.
    //!
    //! Intuitively, `Comparable` objects must define a binary predicate named
    //! `equal` that returns whether both objects represent the same abstract
    //! value. In other words, `equal` must check for deep equality. Since
    //! "representing the same abstract value" is difficult to express
    //! formally, the exact meaning of equality is partially left to
    //! interpretation by the programmer with the following guidelines:\n
    //! 1. Equality should be compatible with copy construction; copy
    //!    constructing a value yields an `equal` value.
    //! 2. Equality should be independent of representation; an object
    //!    representing a fraction as `4/8` should be `equal` to an object
    //!    representing a fraction as `2/4`, because they both represent
    //!    the mathematical object `1/2`.
    //!
    //! Moreover, `equal` must exhibit properties that make it intuitive to
    //! use for determining the equivalence of objects, which is formalized
    //! by the following laws.
    //!
    //!
    //! Laws
    //! ----
    //! `equal` must define an [equivalence relation][1], and `not_equal` must
    //! be its complement. In other words, for all objects `a`, `b`, `c` of a
    //! `Comparable` data type, the following must be true (where `x == y` and
    //! `x != y` denote `equal(x, y)` and `not_equal(x, y)`, respectively):
    //! @code
    //!     a == a                          // Reflexivity
    //!     if a == b then b == a           // Symmetry
    //!     if a == b && b == c then a == c // Transitivity
    //!     a != b is equivalent to !(a == b)
    //! @endcode
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! 1. `equal`\n
    //! When `equal` is defined, `not_equal` is implemented by default as its
    //! complement. For all objects `x`, `y` of a `Comparable` data type,
    //! @code
    //!     not_equal(x, y) == not_(equal(x, y))
    //! @endcode
    //!
    //!
    //! Free model for `EqualityComparable` data types
    //! ----------------------------------------------
    //! Two data types `T` and `U` that model the cross-type EqualityComparable
    //! concept presented in [N3351][2] automatically model the `Comparable`
    //! concept by setting
    //! @code
    //!     equal(x, y) = (x == y)
    //! @endcode
    //! Note that this also makes EqualityComparable types in the
    //! [usual sense][3] models of `Comparable` in the same way.
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Either`, `IntegralConstant`, `Map`, `Maybe`, `Pair`, `Range`,
    //! `Set`, `String`, `Tuple`, `Type`
    //!
    //!
    //! Operators
    //! ---------
    //! For convenience, the following operators are provided as an
    //! equivalent way of calling the corresponding method:
    //! @code
    //!     ==  ->  equal
    //!     !=  ->  not_equal
    //! @endcode
    //!
    //!
    //! Equality-preserving functions
    //! -----------------------------
    //! Let `A` and `B` be two `Comparable` data types. A function
    //! @f$ f : A \to B @f$ is said to be equality-preserving if it preserves
    //! the structure of the `Comparable` concept, which can be rigorously
    //! stated as follows. For all objects `x`, `y` of data type `A`,
    //! @code
    //!     if  equal(x, y)  then  equal(f(x), f(y))
    //! @endcode
    //! Equivalently, we simply require that `f` is a function in the usual
    //! mathematical sense. Another property is [injectivity][4], which can be
    //! viewed as being a "lossless" mapping. This property can be stated as
    //! @code
    //!     if  equal(f(x), f(y))  then  equal(x, y)
    //! @endcode
    //! This is equivalent to saying that `f` maps distinct elements to
    //! distinct elements, hence the "lossless" analogy. In other words, `f`
    //! will not collapse distinct elements from its domain into a single
    //! element in its image, thus losing information.
    //!
    //! These functions are very important, especially equality-preserving
    //! ones, because they allow us to reason simply about programs. Also
    //! note that the property of being equality-preserving is taken for
    //! granted in mathematics because it is part of the definition of a
    //! function. We feel it is important to make the distinction here
    //! because programming has evolved differently and as a result
    //! programmers are used to work with functions that do not preserve
    //! equality.
    //!
    //!
    //! Cross-type version of the methods
    //! ---------------------------------
    //! The `equal` and `not_equal` methods are "overloaded" to handle
    //! distinct data types with certain properties. Specifically, they
    //! are defined for _distinct_ data types `A` and `B` such that
    //! 1. `A` and `B` share a common data type `C`, as determined by the
    //!    `common` metafunction
    //! 2. `A`, `B` and `C` are all `Comparable` when taken individually
    //! 3. @f$ \mathrm{to<C>} : A \to C @f$ and @f$\mathrm{to<C>} : B \to C@f$
    //!    are both equality-preserving and injective (i.e. they are embeddings),
    //!    as determined by the `is_embedding` metafunction.
    //!
    //! The method definitions for data types satisfying the above
    //! properties are
    //! @code
    //!     equal(x, y)     = equal(to<C>(x), to<C>(y))
    //!     not_equal(x, y) = not_equal(to<C>(x), to<C>(y))
    //! @endcode
    //!
    //!
    //! Important note: special behavior of `equal`
    //! -------------------------------------------
    //! In the context of programming with heterogeneous values, it is useful
    //! to have unrelated objects compare `false` instead of triggering an
    //! error. For this reason, `equal` adopts a special behavior for
    //! unrelated objects of data types `T` and `U` that do not satisfy the
    //! above requirements for the cross-type overloads. Specifically, when
    //! `T` and `U` are unrelated (i.e. `T` can't be converted to `U` and
    //! vice-versa), comparing objects of those data types yields a
    //! compile-time false value. This has the effect that unrelated objects
    //! like `float` and `std::string` will compare false, while comparing
    //! related objects that can not be safely embedded into the same super
    //! structure (like `long long` and `float` because of the precision loss)
    //! will trigger a compile-time assertion. Also note that for any data type
    //! `T` for which the minimial complete definition of `Comparable` is not
    //! provided, a compile-time assertion will also be triggered because `T`
    //! and `T` trivially share the common data type `T`, which is the expected
    //! behavior. This design choice aims to provide more flexibility for
    //! comparing objects, while still rejecting usage patterns that are
    //! most likely programming errors.
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Equivalence_relation#Definition
    //! [2]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3351.pdf
    //! [3]: http://en.cppreference.com/w/cpp/concept/EqualityComparable
    //! [4]: http://en.wikipedia.org/wiki/Injective_function
    struct Comparable { };

    //! Returns a `Logical` representing whether `x` is equal to `y`.
    //! @relates Comparable
    //!
    //! The `equal` function can be called in two different ways. First, it
    //! can be called like a normal function:
    //! @code
    //!     equal(x, y)
    //! @endcode
    //!
    //! However, it may also be partially applied to an argument by using
    //! `equal.to`:
    //! @code
    //!     equal.to(x)(y) == equal(x, y)
    //! @endcode
    //!
    //! In other words, `equal.to(x)` is a function object that is equivalent
    //! to `partial(equal, x)`. This is provided to enhance the readability of
    //! some constructs, especially when using higher order algorithms.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Logical `Bool` and two Comparables `A` and `B` that
    //! share a common embedding, the signature is
    //! @f$ \mathrm{equal} : A \times B \to Bool @f$.
    //!
    //! @param x, y
    //! Two objects to compare for equality.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/comparable.cpp equal
    //!
    //!
    //! > #### Rationale for the arity of `equal`
    //! > It is a valid question whether `equal` should accept more than 2
    //! > arguments and have semantics matching those of Python's `==`. This
    //! > is not supported right now for the following reasons:
    //! > - It was implemented in the MPL11, but it was not shown to be useful
    //! >   so far.
    //! > - It does not make sense for `not_equal` to have an arity of more
    //! >   than 2, only `equal` could maybe have those semantics, which would
    //! >   break symmetry.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto equal = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct equal_impl;

    struct _equal_to {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const;
    };

    template <typename ...AvoidODRViolation>
    struct _equal {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using Equal = equal_impl<T, U>;
            return Equal::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }

        static constexpr _equal_to to{};
    };
    template <typename ...AvoidODRViolation>
    constexpr _equal_to _equal<AvoidODRViolation...>::to;

    constexpr _equal<> equal{};
#endif

    //! Returns a `Logical` representing whether `x` is not equal to `y`.
    //! @relates Comparable
    //!
    //! The `not_equal` function can be called in two different ways. First,
    //! it can be called like a normal function:
    //! @code
    //!     not_equal(x, y)
    //! @endcode
    //!
    //! However, it may also be partially applied to an argument by using
    //! `not_equal.to`:
    //! @code
    //!     not_equal.to(x)(y) == not_equal(x, y)
    //! @endcode
    //!
    //! In other words, `not_equal.to(x)` is a function object that is
    //! equivalent to `partial(not_equal, x)`. This is provided to enhance
    //! the readability of some constructs, especially when using higher
    //! order algorithms.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Logical `Bool` and two Comparables `A` and `B` that
    //! share a common embedding, the signature is
    //! @f$ \mathrm{not\_equal} : A \times B \to Bool @f$.
    //!
    //! @param x, y
    //! Two objects to compare for inequality.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/comparable.cpp not_equal
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto not_equal = [](auto&& x, auto&& y) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename T, typename U, typename = void>
    struct not_equal_impl;

    struct _not_equal_to {
        template <typename X>
        constexpr decltype(auto) operator()(X&& x) const;
    };

    template <typename ...AvoidODRViolation>
    struct _not_equal {
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const {
            using T = typename datatype<X>::type;
            using U = typename datatype<Y>::type;
            using NotEqual = not_equal_impl<T, U>;
            return NotEqual::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
        }

        static constexpr _not_equal_to to{};
    };
    template <typename ...AvoidODRViolation>
    constexpr _not_equal_to _not_equal<AvoidODRViolation...>::to;

    constexpr _not_equal<> not_equal{};
#endif

    //! Returns a function performing `equal` after applying a transformation
    //! to both arguments.
    //! @relates Comparable
    //!
    //! `comparing` creates an equivalence relation based on the result of
    //! applying a function to some objects, which is especially useful in
    //! conjunction with algorithms that accept a custom predicate that must
    //! represent an equivalence relation.
    //!
    //! Specifically, `comparing` is such that
    //! @code
    //!     comparing(f) == equal ^on^ f
    //! @endcode
    //! or, equivalently,
    //! @code
    //!     comparing(f)(x, y) == equal(f(x), f(y))
    //! @endcode
    //!
    //! @note
    //! This is not a tag-dispatched method (hence it can't be customized),
    //! but just a convenience function provided with the `Comparable` concept.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Logical `Bool` and a Comparable `B`, the signature is
    //! @f$ \mathrm{comparing} : (A \to B) \to (A \times A \to Bool) @f$.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/comparable.cpp comparing
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto comparing = [](auto&& f) {
        return [perfect-capture](auto&& x, auto&& y) -> decltype(auto) {
            return equal(f(forwarded(x)), f(forwarded(y)));
        };
    };
#else
    template <typename F>
    struct _comparing;

    constexpr detail::create<_comparing> comparing{};
#endif

    namespace operators {
        template <>
        struct of<Comparable>
            : decltype(equal), decltype(not_equal)
        { };
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_COMPARABLE_HPP
