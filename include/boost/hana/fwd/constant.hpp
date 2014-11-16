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
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-typeclasses
    //! The `Constant` type class is used for data types representing
    //! compile-time constants.
    //!
    //!
    //! ### Laws
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
    //! be constructed at compile-time. It is important to note that since the
    //!
    //! @code
    //!     constexpr auto t = value(x);
    //! @endcode
    //!
    //! expression appears in a context where `x` is _not_ a constant
    //! expression, this law also means that `value` must be able to return
    //! a constant expression even when called with something that isn't one.
    //! This requirement is the core of a `Constant`; it basically means that
    //! all of the information stored inside the `c` object that's used in a
    //! call to `value` must actually be stored inside its type.
    //!
    //!
    //! @todo
    //! Should we provide a `Comparable::constant_mcd` like we used to?
    struct Constant {
        BOOST_HANA_TYPECLASS(Constant);
        struct mcd;
    };

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
    namespace constant_detail {
        struct value {
            template <typename C>
            constexpr decltype(auto) operator()(C&& constant) const {
                return Constant::instance<
                    datatype_t<C>
                >::value_impl(detail::std::forward<C>(constant));
            }
        };
    }

    constexpr constant_detail::value value{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CONSTANT_HPP
