/*!
@file
Forward declares `boost::hana::Constant`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CONSTANT_HPP
#define BOOST_HANA_FWD_CONSTANT_HPP

#include <boost/hana/fwd/core/datatype.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Constant` concept represents data that can be manipulated at
    //! compile-time.
    //!
    //! At its core, `Constant` is simply a generalization of the principle
    //! behind `std::integral_constant` to all types that can be constructed
    //! at compile-time, i.e. to all types with a `constexpr` constructor
    //! (also called [Literal types][1]). More specifically, a `Constant` is
    //! an object from which a `constexpr` value may be obtained (through the
    //! `value` method) regardless of the `constexpr`ness of the object itself.
    //!
    //! All `Constant`s must be somewhat equivalent, in the following sense.
    //! Let `C(T)` and `D(U)` denote the data types of `Constant`s holding
    //! objects of type `T` and `U`, respectively. Then, an object of data
    //! type `D(U)` must be convertible to an object of type `C(T)` whenever
    //! `U` is convertible to `T`, has determined by `is_convertible`. The
    //! interpretation here is that a `Constant` is just a box holding an
    //! object of some type, and it should be possible to swap between boxes
    //! whenever the objects inside the boxes can be swapped.
    //!
    //! Because of this last requirement, one could be tempted to think that
    //! specialized "boxes" like `std::integral_constant` are prevented from
    //! being `Constant`s because they are not able to hold objects of any
    //! type `T` (`std::integral_constant` may only hold integral types).
    //! This is false; the requirement should be interpreted as saying that
    //! whenever `C(T)` is _meaningful_ (e.g. only when `T` is integral for
    //! `std::integral_constant`) _and_ there exists a conversion from `U`
    //! to `T`, then a conversion from `D(U)` to `C(T)` should also exist.
    //! The precise requirements for being a `Constant` are embodied in the
    //! following laws.
    //!
    //!
    //! Laws
    //! ----
    //! Let `c` be an object of a data type `C`, which represents a `Constant`
    //! holding an object of data type `T`. The first law ensures that the
    //! value of the wrapped object is always a constant expression by
    //! requiring the following to be well-formed:
    //! @code
    //!     constexpr auto x = hana::value<decltype(x)>();
    //! @endcode
    //!
    //! This means that the `value` function must return an object that can
    //! be constructed at compile-time. It is important to note how `value`
    //! only receives the type of the object and not the object itself.
    //! This is the core of the `Constant` concept; it means that the only
    //! information required to implement `value` must be stored in the _type_
    //! of its argument, and hence be available statically.
    //!
    //! The second law that must be satisfied ensures that `Constant`s are
    //! basically dumb boxes, which makes it possible to provide models for
    //! many concepts without much work from the user. The law simply asks
    //! for the following expression to be valid:
    //! @code
    //!     to<C>(i)
    //! @endcode
    //! where, `i` is an _arbitrary_ `Constant` holding an internal value
    //! of a data type which can be converted to `T`, as determined by the
    //! `is_convertible` metafunction. In other words, whenever `U` is
    //! convertible to `T`, a `Constant` holding a `U` is convertible to
    //! a `Constant` holding a `T`, if such a `Constant` can be created.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `value`, satisfying the laws above.
    //!
    //!
    //! Provided conversions
    //! --------------------
    //! 1. To the data type of the underlying value\n
    //! Any `Constant` `c` holding an underlying value of data type `T` is
    //! convertible to any data type `U` such that `T` is convertible to `U`.
    //! Specifically, the conversion is equivalent to
    //! @code
    //!     to<U>(c) == to<U>(value<decltype(c)>())
    //! @endcode
    //!
    //!
    //! Provided common data type
    //! -------------------------
    //! Because of the requirement that `Constant`s be interchangeable when
    //! their contents are compatible, two `Constant`s `A` and `B` will have
    //! a common data type whenever `A::value_type` and `B::value_type` have
    //! one. Their common data type is an unspecified `Constant` `C` such
    //! that `C::value_type` is exactly `common_t<A::value_type, B::value_type>`.
    //! A specialization of the `common` metafunction is provided for
    //! `Constant`s to reflect this.
    //!
    //!
    //! @todo
    //! - Document the nested value_type.
    //! - Document the provided models, but that should be done in each concept.
    //! - The fact that `common_t<IntegralConstant<int>, IntegralConstant<long>>`
    //! is `CanonicalConstant<long>` is unsatisfactory. We use a super ugly
    //! hack right now to avoid it; get rid of it.
    //! -  Are the conversions required to be embeddings? It seems like this
    //! should always be the case?
    //! - Right now, the provided conversion to non-constants T is marked as an
    //! embedding whenever conversion from the underlying type to T is an
    //! embedding, regardless of the fact that we actually lose the Constant's
    //! structure. This is a real bummer, but without this expressions like
    //! `plus(int_<1>, 1)` would fail. What should be done? In the same vein,
    //! we currently provide a common type with non-constants for which our
    //! underlying type has a common type. Does this respect the requirements
    //! of a common type? Add tests for that in the Constant automatic tests
    //! once this is settled.
    //!
    //!
    //! [1]: http://en.cppreference.com/w/cpp/concept/LiteralType
    struct Constant { };

    //! Return the compile-time value associated to a constant.
    //! @relates Constant
    //!
    //! This function returns the value associated to a `Constant`. That
    //! value is always a constant expression. The normal way of using
    //! `value` on an object `c` is
    //! @code
    //!     constexpr auto result = hana::value<decltype(c)>();
    //! @endcode
    //!
    //! However, for convenience, an overload of `value` is provided so that
    //! it can be called as:
    //! @code
    //!     constexpr auto result = hana::value(c);
    //! @endcode
    //!
    //! This overload works by taking a `const&` to its argument, and then
    //! forwarding to the first version of `value`. Since it does not use
    //! its argument, the result can still be a constant expression, even
    //! if the argument is not a constant expression.
    //!
    //! @note
    //! `value<T>()` is tag-dispatched as `value_impl<C>::%apply<T>()`, where
    //! `C` is the data type of `T`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/constant.cpp value
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename T>
    constexpr auto value = []() -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename C, typename = void>
    struct value_impl;

    template <typename T>
    constexpr decltype(auto) value();

    template <typename T>
    constexpr decltype(auto) value(T const&)
    { return value<T>(); }
#endif

    //! Equivalent to `value`, but can be passed to higher-order algorithms.
    //! @relates Constant
    //!
    //! This function object is equivalent to `value`, except it can be passed
    //! to higher order algorithms because it is a function object. `value`
    //! can't be passed to higher-order algorithms because it is implemented
    //! as an overloaded function.
    //!
    //! @note
    //! This function is a simple alias to `value`, and hence it is not
    //! tag-dispatched and can't be customized.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/constant.cpp value_of
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto value_of = [](auto const& c) -> decltype(auto) {
        return value(c);
    };
#else
    struct _value_of {
        template <typename T>
        constexpr decltype(auto) operator()(T const&) const
        { return value<T>(); }
    };

    constexpr _value_of value_of{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CONSTANT_HPP
