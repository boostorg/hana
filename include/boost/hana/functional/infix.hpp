/*!
@file
Defines `boost::hana::infix`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_INFIX_HPP
#define BOOST_HANA_FUNCTIONAL_INFIX_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Return an equivalent function that can also be applied in infix
    //! notation.
    //!
    //! Specifically, `infix(f)` is an object such that:
    //! @code
    //!     infix(f)(x1, ..., xn) == f(x1, ..., xn)
    //!     x ^infix(f)^ y == f(x, y)
    //! @endcode
    //!
    //! Hence, the returned function can still be applied using the usual
    //! function call syntax, but it also gains the ability to be applied in
    //! infix notation. The infix syntax allows a great deal of expressiveness,
    //! especially when used in combination with some higher order algorithms.
    //! Since `operator^` is left-associative, `x ^infix(f)^ y` is actually
    //! parsed as `(x ^infix(f))^ y`. However, for flexibility, the order in
    //! which both arguments are applied in infix notation does not matter.
    //! Hence, it is always the case that
    //! @code
    //!     (x ^ infix(f)) ^ y == x ^ (infix(f) ^ y)
    //! @endcode
    //!
    //! However, note that applying more than one argument in infix
    //! notation to the same side of the operator will result in a
    //! compile-time assertion:
    //! @code
    //!     (infix(f) ^ x) ^ y; // compile-time assertion
    //!     y ^ (x ^ infix(f)); // compile-time assertion
    //! @endcode
    //!
    //! Additionally, a function created with `infix` may be partially applied
    //! in infix notation. Specifically,
    //! @code
    //!     (x ^ infix(f))(y1, ..., yn) == f(x, y1, ..., yn)
    //!     (infix(f) ^ y)(x1, ..., xn) == f(x1, ..., xn, y)
    //! @endcode
    //!
    //! @internal
    //! ### Rationales
    //! 1. The `^` operator was chosen because it is left-associative and
    //!    has a low enough priority so that most expressions will render
    //!    the expected behavior.
    //! 2. The operator can't be customimzed because that would require more
    //!    sophistication in the implementation; I want to keep it as simple
    //!    as possible. There is also an advantage in having a uniform syntax
    //!    for infix application.
    //! @endinternal
    //!
    //! @param f
    //! The function which gains the ability to be applied in infix notation.
    //! The function must be at least binary; a compile-time error will be
    //! triggered otherwise.
    //!
    //! ### Example
    //! @snippet example/functional/infix.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto infix = [](auto f) {
        return unspecified;
    };
#else
    namespace infix_detail {
        template <typename F, bool left = false, bool right = false>
        struct infix {
            F f;

            template <typename ...X>
            constexpr auto operator()(X ...x) const
            { return f(x...); }
        };

        // infix(f) ^ y
        template <typename F, bool left, bool right, typename Y>
        constexpr auto operator^(infix<F, left, right> f, Y y) {
            static_assert(!right,
            "invalid usage of boost::hana::infix with a double right operand");

            auto g = [=](auto ...x) { return f(x..., y); };
            return infix<decltype(g), left, true>{g};
        }

        template <typename F, bool right, typename Y>
        constexpr auto operator^(infix<F, true, right> f, Y y) {
            static_assert(!right,
            "invalid usage of boost::hana::infix with a double right operand");

            return f(y);
        }

        // x ^ infix(f)
        template <typename X, typename F, bool left, bool right>
        constexpr auto operator^(X x, infix<F, left, right> f) {
            static_assert(!left,
            "invalid usage of boost::hana::infix with a double left operand");

            auto g = [=](auto ...y) { return f(x, y...); };
            return infix<decltype(g), true, right>{g};
        }

        template <typename X, typename F, bool left>
        constexpr auto operator^(X x, infix<F, left, true> f) {
            static_assert(!left,
            "invalid usage of boost::hana::infix with a double left operand");

            return f(x);
        }
    } // end namespace infix_detail

    constexpr auto infix = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto f) {
        return infix_detail::infix<decltype(f)>{f};
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_INFIX_HPP
