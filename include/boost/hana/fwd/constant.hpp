/*!
@file
Forward declares `boost::hana::Constant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CONSTANT_HPP
#define BOOST_HANA_FWD_CONSTANT_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Constant` type class represents data that can be manipulated at
    //! compile-time.
    //!
    //! At its core, `Constant` is simply a generalization of the principle
    //! behind `std::integral_constant` to all types that can be constructed
    //! at compile-time, i.e. to all types with a `constexpr` constructor.
    //! A `Constant` is an object from which a `constexpr` value may be obtained
    //! (through the `value` method) regardless of the `constexpr`ness of
    //! the object itself. For this to be possible, the type of that object
    //! must look somewhat like
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
    //! of type `Something`. Of course, other implementations may be possible,
    //! but this gives the idea. The requirement that a `constexpr` value can
    //! be obtained from any object is embodied by the following laws.
    //!
    //!
    //! Laws
    //! ----
    //! For any `Constant` `c`, the following program must be well-formed:
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
    //!     constexpr auto t = value(x);
    //! @endcode
    //!
    //! appears in a context where `x` is _not_ a constant expression,
    //! this law also means that `value` must be able to return a constant
    //! expression even when called with something that isn't one. This
    //! requirement is the core of a `Constant`; it basically means that
    //! all of the information stored inside the `c` object that's used in a
    //! call to `value` must actually be stored inside its type.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `value`, satisfying the laws above.
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
    //! ### Example
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
