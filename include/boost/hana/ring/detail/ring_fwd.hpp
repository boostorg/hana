/*!
@file
Internal header to break cyclic dependencies.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RING_DETAIL_RING_FWD_HPP
#define BOOST_HANA_RING_DETAIL_RING_FWD_HPP

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    namespace ring_detail { namespace operators { struct enable { }; } }

    //! @ingroup group-typeclasses
    //! The `Ring` type class is used for `Group`s that also form a `Monoid`
    //! under a second binary operation that distributes over the first.
    //!
    //! The method names refer to the ring of numbers under multiplication.
    //!
    //! ### Requires
    //! `Group`
    //!
    //! ### Laws
    //! For all objects `x`, `y`, `z` of a `Ring` `R`, the following laws must
    //! be satisfied:
    //! @code
    //!     mult(x, mult(y, z)) == mult(mult(x, y), z)          // associativity
    //!     mult(x, one<R>) == x                                // right identity
    //!     mult(one<R>, x) == x                                // left identity
    //!     mult(x, plus(y, z)) == plus(mult(x, y), mult(x, z)) // distributivity
    //! @endcode
    struct Ring {
        BOOST_HANA_BINARY_TYPECLASS(Ring);
        //! Minimal complete definition : `one` and `mult`
        struct mcd { };
        using operators = ring_detail::operators::enable;

        struct laws;
    };

    //! Associative operation of a `Ring`.
    //! @relates Ring
    //!
    //! ### Example
    //! @snippet example/ring.cpp mult
    BOOST_HANA_CONSTEXPR_LAMBDA auto mult = [](auto&& x, auto&& y) -> decltype(auto) {
        return Ring::instance<
            datatype_t<decltype(x)>, datatype_t<decltype(y)>
        >::mult_impl(
            detail::std::forward<decltype(x)>(x),
            detail::std::forward<decltype(y)>(y)
        );
    };

    //! Identity of `mult`.
    //! @relates Ring
    //!
    //! Since `Ring` is a binary type class and `one` is a nullary method,
    //! `one<R>` is dispatched to the type class instance for `R` and `R`,
    //! i.e. `Ring::instance<R, R>`.
    //!
    //! @tparam R
    //! The data type (a `Ring`) of the returned identity.
    //!
    //! ### Example
    //! @snippet example/ring.cpp one
    template <typename R>
    constexpr auto one = Ring::instance<R, R>::one_impl();

    namespace ring_detail { namespace operators {
        //! Equivalent to `mult`.
        //! @relates boost::hana::Ring
        template <typename X, typename Y>
        constexpr decltype(auto) operator*(X&& x, Y&& y) {
            return mult(
                detail::std::forward<decltype(x)>(x),
                detail::std::forward<decltype(y)>(y)
            );
        }
    }}
}} // end namespace boost::hana

#endif // !BOOST_HANA_RING_DETAIL_RING_FWD_HPP
