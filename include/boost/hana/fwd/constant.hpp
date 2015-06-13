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
    //! Minimal complete definition
    //! ---------------------------
    //! `value`, satisfying the laws below.
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
    //! Finally, the data type `C` must provide a nested `value_type` alias
    //! to `T`, which allows us to query the data type of the inner value held
    //! by objects of data type `C`. In other words, the following must be
    //! true for any object `c` of data type `C`:
    //! @code
    //!     std::is_same<
    //!         C::value_type,
    //!         datatype_t<decltype(hana::value(c))>
    //!     >::value
    //! @endcode
    //!
    //!
    //! Superclasses
    //! ------------
    //! In certain cases, a `Constant` can automatically be made a model of
    //! another concept. In particular, if a `Constant` `C` is holding an
    //! object of type `T`, and if `T` models a concept `X`, then `C` may
    //! in most cases model `X` by simply performing whatever operation is
    //! required on its underlying value, and then wrapping the result back
    //! in a `C`.
    //!
    //! More specifically, if a `Constant` `C` has an underlying value
    //! (`C::value_type`) which is a model of `Comparable`, `Orderable`,
    //! `Enumerable`, `Logical`, or `Monoid` up to `IntegralDomain`, then
    //! all of those concepts become superclasses of `Constant` for `C`.
    //! In other words, when `C::value_type` models one of the listed
    //! concepts, `C` itself must also model that concept. However, note
    //! that free models are provided for those concepts, so no additional
    //! work must be done.
    //!
    //! While it would be possible in theory to provide models for concepts
    //! like `Foldable` too, only a couple of concepts are useful to have as
    //! `Constant` in practice. Providing free models for the concepts listed
    //! above is useful because it allows various types of integral constants
    //! (`std::integral_constant`, `mpl::integral_c`, etc...) to easily have
    //! models for them just by defining the `Constant` concept.
    //!
    //! @remark
    //! An interesting observation is that `Constant` is actually the
    //! canonical embedding of the subcategory of `constexpr` things
    //! into the Hana category, which contains everything in this library.
    //! Hence, whatever is true in that subcategory is also true here, via
    //! this functor. This is why we can provide models of any concept that
    //! works on `constexpr` things for Constants, by simply passing them
    //! through that embedding.
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `IntegralConstant`
    //!
    //!
    //! Provided conversion to the data type of the underlying value
    //! ------------------------------------------------------------
    //! Any `Constant` `c` holding an underlying value of data type `T` is
    //! convertible to any data type `U` such that `T` is convertible to `U`.
    //! Specifically, the conversion is equivalent to
    //! @code
    //!     to<U>(c) == to<U>(value<decltype(c)>())
    //! @endcode
    //!
    //! Also, those conversions are marked as an embedding whenever the
    //! conversion of underlying types is an embedding. This is to allow
    //! Constants to inter-operate with `constexpr` objects easily:
    //! @code
    //!     plus(int_<1>, 1) == 2
    //! @endcode
    //!
    //! Strictly speaking, __this is sometimes a violation__ of what it means
    //! to be an embedding. Indeed, while there exists an embedding from any
    //! Constant to a `constexpr` object (since Constant is just the canonical
    //! inclusion), there is no embedding from a Constant to a runtime
    //! object since we would lose the ability to define the `value` method
    //! (the `constexpr`ness of the object would have been lost). Since there
    //! is no way to distinguish `constexpr` and non-`constexpr` objects based
    //! on their type, Hana has no way to know whether the conversion is to a
    //! `constexpr` object of not. In other words, the `to` method has no way
    //! to differentiate between
    //! @code
    //!     constexpr int i = hana::to<int>(int_<1>);
    //! @endcode
    //! which is an embedding, and
    //! @code
    //!     int i = hana::to<int>(int_<1>);
    //! @endcode
    //!
    //! which isn't. To be on the safer side, we could mark the conversion
    //! as not-an-embedding. However, if e.g. the conversion from
    //! `IntegralConstant<int>` to `int` was not marked as an embedding,
    //! we would have to write `plus(to<int>(int_<1>), 1)` instead of just
    //! `plus(int_<1>, 1)`, which is cumbersome. Hence, the conversion is
    //! marked as an embedding, but this also means that code like
    //! @code
    //!     int i = 1;
    //!     plus(int_<1>, i);
    //! @endcode
    //! will be considered valid, which implicitly loses the fact that
    //! `int_<1>` is a Constant, and hence does not follow the usual rules
    //! for cross-type operations in Hana.
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
    //! In the same vein, a common data type is also provided from any
    //! constant `A` to a type `T` such that `A::value_type` and `T` share
    //! a common type. The common type between `A` and `T` is obviously the
    //! common type between `A::value_type` and `T`. As explained above in
    //! the section on conversions, this is sometimes a violation of the
    //! definition of a common type, because there must be an embedding
    //! to the common type, which is not always the case. For the same
    //! reasons as explained above, this common type is still provided.
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
