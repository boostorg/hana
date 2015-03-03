/*!
@file
Forward declares `boost::hana::Constant`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CONSTANT_HPP
#define BOOST_HANA_FWD_CONSTANT_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Constant` type class represents data that can be manipulated at
    //! compile-time.
    //!
    //! At its core, `Constant` is simply a generalization of the principle
    //! behind `std::integral_constant` to all types that can be constructed
    //! at compile-time, i.e. to all types with a `constexpr` constructor
    //! (also called [Literal][1] types). More specifically, a `Constant` is
    //! an object from which a `constexpr` value may be obtained (through the
    //! `value` method) regardless of the `constexpr`ness of the object itself.
    //! For this to be possible, the type of that object must look somewhat like
    //! @code
    //!     struct Something {
    //!         static constexpr auto the_constexpr_value = ...;
    //!     };
    //! @endcode
    //!
    //! Then, the `value` method can be implemented as
    //! @code
    //!     constexpr auto value(Something) {
    //!         return Something::the_constexpr_value;
    //!     }
    //! @endcode
    //!
    //! Holding the value as a static constant makes it possible to obtain a
    //! `constexpr` result even when calling `value` on a non-constexpr object
    //! of type `Something`. If the function had used the _value_ of its
    //! argument, it could not have returned a constant expression when its
    //! argument is not one. Hence, the function is only allowed to depend on
    //! the _type_ of its argument.
    //!
    //! Also, all `Constant`s must be somewhat equivalent, in the following
    //! sense. Let `C<T>` and `D<U>` denote the data types of `Constant`s
    //! holding objects of type `T` and `U`, respectively. Then, an object
    //! of data type `D<U>` must be convertible to an object of type `C<T>`
    //! whenever `U` is convertible to `T`, has determined by `is_convertible`.
    //! The interpretation here is that a `Constant` is just a box holding an
    //! object of some type, and it should be possible to swap between boxes
    //! whenever the objects inside the boxes can be swapped.
    //!
    //! Because of this last requirement, one could be tempted to think that
    //! specialized "boxes" like `std::integral_constant` are prevented from
    //! being `Constant`s because they are not able to hold objects of any
    //! type `T` (`std::integral_constant` may only hold integral types).
    //! This is false; the requirement should be interpreted as saying that
    //! whenever `C<T>` is _meaningful_ (e.g. only when `T` is integral for
    //! `std::integral_constant`) _and_ there exists a conversion from `U`
    //! to `T`, then a conversion from `D<U>` to `C<T>` should also exist.
    //! The precise requirements for being a `Constant` are embodied in the
    //! following laws.
    //!
    //!
    //! Laws
    //! ----
    //! Let `c` be an object of a data type `C`, which represents a `Constant`
    //! holding an object of data type `T`. The first law ensures that the
    //! value of the wrapped object can always be obtained as a constant
    //! expression, by requiring that the following program be well-formed:
    //! @code
    //!     template <typename X>
    //!     void f(X x) {
    //!         constexpr auto y = value(x);
    //!     }
    //!
    //!     int main() {
    //!         f(c);
    //!     }
    //! @endcode
    //!
    //! This means that the `value` function must return an object that can
    //! be constructed at compile-time. It is important to note that since
    //! @code
    //!     constexpr auto y = value(x);
    //! @endcode
    //!
    //! appears in a context where `x` is _not_ a constant expression (function
    //! arguments are never `constexpr` inside the function's context), this
    //! law precisely means that `value` must be able to return a constant
    //! expression even when called with something that isn't one. This
    //! requirement is the core of the `Constant` concept; it means that
    //! the only information required to implement `value` must be stored
    //! in the _type_ of its argument, and hence be available statically.
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
    //!     to<U>(c) == to<U>(value(c))
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
    //! A specialization of the `common` metafunction is provided for `Constant`s
    //! to embody this.
    //!
    //!
    //! @todo
    //! - Replace value by an equivalent version that works with types only.
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
    //! This function returns a value which is always a constant expression,
    //! and this function can always be called inside a constant expression.
    //! This imposes some rather heavy restrictions on the implementation;
    //! specifically, it means that the implementation is fully determined
    //! by the type of its argument, and that it does not use the value of
    //! its argument at all.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/constant.cpp value
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto value = [](auto&& constant) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename C, typename = void>
    struct value_impl;

    struct _value {
        template <typename C>
        constexpr decltype(auto) operator()(C&& constant) const {
            return value_impl<typename datatype<C>::type>::apply(
                detail::std::forward<C>(constant)
            );
        }
    };

    constexpr _value value{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CONSTANT_HPP
