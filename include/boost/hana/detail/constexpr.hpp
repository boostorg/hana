/*!
@file
Defines the `BOOST_HANA_CONSTEXPR_LAMBDA` and
`BOOST_HANA_MAKE_CONSTEXPR_LAMBDA` macros.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CONSTEXPR_HPP
#define BOOST_HANA_DETAIL_CONSTEXPR_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/remove_reference.hpp>


namespace boost { namespace hana { namespace lambda_detail {
    template <typename F>
    struct wrapper {
        // static_assert(std::is_empty<F>::value, "Lambdas must be empty");

        template <typename ...T>
        constexpr decltype(auto) operator()(T&& ...xs) const {
            return reinterpret_cast<F const&>(*this)(
                detail::std::forward<T>(xs)...
            );
        }
    };

    struct wrapper_factor {
        template <typename F>
        constexpr wrapper<F> operator += (F*) { return {}; }
    };

    struct addr_add {
        template <typename T>
        friend constexpr typename detail::std::remove_reference<T>::type*
        operator+(addr_add, T&& t)
        { return &t; }
    };
}}}

#ifdef BOOST_HANA_DOXYGEN_INVOKED

    //! @ingroup group-details
    //! Expands to `constexpr` if constexpr lambdas are supported and
    //! to nothing otherwise.
#   define BOOST_HANA_CONSTEXPR_LAMBDA constexpr

    //! @ingroup group-details
    //! Allows creating `constexpr` lambdas.
    //!
    //! Specifically, use this as
    //! @code
    //!     auto f = BOOST_HANA_MAKE_CONSTEXPR_LAMBDA(auto ...args) {
    //!         // ...
    //!     };
    //! @endcode
    //! to create a non-capturing `constexpr` lambda.
    //!
    //! @note
    //! This trick was taken from
    //! http://pfultz2.com/blog/2014/09/02/static-lambda/.
#   define BOOST_HANA_MAKE_CONSTEXPR_LAMBDA []

#else

#   define BOOST_HANA_CONSTEXPR_LAMBDA /* nothing */

#   define BOOST_HANA_MAKE_CONSTEXPR_LAMBDA                                 \
        ::boost::hana::lambda_detail::wrapper_factor{} +=                   \
            true ? nullptr : ::boost::hana::lambda_detail::addr_add{} + []  \
    /**/

#endif

#endif // !BOOST_HANA_DETAIL_CONSTEXPR_HPP
