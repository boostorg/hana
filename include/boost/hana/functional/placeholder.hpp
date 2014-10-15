/*!
@file
Defines `boost::hana::_`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_PLACEHOLDER_HPP
#define BOOST_HANA_FUNCTIONAL_PLACEHOLDER_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Create simple functions representing C++ operators inline.
    //!
    //! Specifically, `_` is an object used as a placeholder to build
    //! function objects representing calls to C++ operators. It works
    //! by overloading the operators between `_` and any object so that
    //! they return a function object which actually calls the corresponding
    //! operator on its argument(s). Hence, for any supported operator `@`:
    //! @code
    //!     (_ @ _)(x, y) == x @ y
    //! @endcode
    //!
    //! Operators may also be partially applied to one argument inline:
    //! @code
    //!     (x @ _)(y) == x @ y
    //!     (_ @ y)(x) == x @ y
    //! @endcode
    //!
    //! When invoked with more arguments than required, functions created with
    //! `_` will discard the superfluous instead of triggering an error:
    //! @code
    //!     (_ @ _)(x, y, z...) == x @ y
    //! @endcode
    //!
    //! This makes functions created with `_` easier to use in higher-order
    //! algorithms, which sometime provide more information than necessary
    //! to their callbacks.
    //!
    //! ### Supported operators
    //! - Arithmetic: binary `+`, binary `-`, `/`, `*`, `%`, unary `+`, unary `-`
    //! - Bitwise: `~`, `&`, `|`, `^`, `<<`, `>>`
    //! - Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
    //! - %Logical: `||`, `&&`, `!`
    //! - Member access: `*` (dereference), `[]` (array subscript)
    //! - Other: `()` (function call)
    //!
    //! More complex functionality like the ability to compose placeholders
    //! into larger function objects inline are not supported. This is on
    //! purpose; you should either use C++14 generic lambdas or a library
    //! like [Boost.Phoenix][] if you need bigger guns. The goal here is
    //! to save you a couple of characters in simple situations.
    //!
    //! ### Example
    //! @snippet example/functional/placeholder.cpp main
    //!
    //! [Boost.Phoenix]: http://www.boost.org/doc/libs/release/libs/phoenix/doc/html/index.html
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr unspecified _{};
#else
    namespace functional_detail {
        struct placeholder {
            template <typename X>
            constexpr auto operator[](X x) const {
                return [x(detail::std::move(x))](auto&& c, auto&& ...z) -> decltype(auto) {
                    return detail::std::forward<decltype(c)>(c)[x];
                };
            }

            template <typename ...X>
            constexpr auto operator()(X ...x) const {
                return [x...](auto&& f, auto&& ...z) -> decltype(auto) {
                    return detail::std::forward<decltype(f)>(f)(x...);
                };
            }
        };

#define BOOST_HANA_PLACEHOLDER_BINARY_OP(op)                                            \
    template <typename X>                                                               \
    constexpr auto operator op (X x, placeholder) {                                     \
        return [x(detail::std::move(x))](auto&& y, auto&& ...z) -> decltype(auto) {     \
            return x op detail::std::forward<decltype(y)>(y);                           \
        };                                                                              \
    }                                                                                   \
                                                                                        \
    template <typename Y>                                                               \
    constexpr auto operator op (placeholder, Y y) {                                     \
        return [y(detail::std::move(y))](auto&& x, auto&& ...z) -> decltype(auto) {     \
            return detail::std::forward<decltype(x)>(x) op y;                           \
        };                                                                              \
    }                                                                                   \
                                                                                        \
    inline BOOST_HANA_CONSTEXPR_LAMBDA auto operator op (placeholder, placeholder) {    \
        return [](auto&& x, auto&& y, auto&& ...z) -> decltype(auto) {                  \
            return detail::std::forward<decltype(x)>(x) op                              \
                   detail::std::forward<decltype(y)>(y);                                \
        };                                                                              \
    }                                                                                   \
/**/

#define BOOST_HANA_PLACEHOLDER_UNARY_OP(op)                                             \
    inline BOOST_HANA_CONSTEXPR_LAMBDA auto operator op (placeholder) {                 \
        return [](auto&& x, auto&& ...z) -> decltype(auto) {                            \
            return op detail::std::forward<decltype(x)>(x);                             \
        };                                                                              \
    }                                                                                   \
/**/
            // Arithmetic
            BOOST_HANA_PLACEHOLDER_UNARY_OP(+)
            BOOST_HANA_PLACEHOLDER_UNARY_OP(-)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(+)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(-)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(*)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(/)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(%)

            // Bitwise
            BOOST_HANA_PLACEHOLDER_UNARY_OP(~)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(&)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(|)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(^)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(<<)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(>>)

            // Comparison
            BOOST_HANA_PLACEHOLDER_BINARY_OP(==)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(!=)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(<)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(<=)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(>)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(>=)

            // Logical
            BOOST_HANA_PLACEHOLDER_BINARY_OP(||)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(&&)
            BOOST_HANA_PLACEHOLDER_UNARY_OP(!)

            // Member access (array subscript is a member function)
            BOOST_HANA_PLACEHOLDER_UNARY_OP(*)

            // Other (function call is a member function)

#undef BOOST_HANA_PREFIX_PLACEHOLDER_OP
#undef BOOST_HANA_BINARY_PLACEHOLDER_OP
    }

    constexpr functional_detail::placeholder _{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_PLACEHOLDER_HPP
