/*!
@file
Defines `boost::hana::Monoid`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_HPP
#define BOOST_HANA_MONOID_HPP

#include <boost/hana/fwd/monoid.hpp>

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        //! Equivalent to `plus`.
        //! @relates boost::hana::Monoid
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            enable_operators<Monoid, datatype_t<X>>::value ||
            enable_operators<Monoid, datatype_t<Y>>::value
        >::type>
        constexpr decltype(auto) operator+(X&& x, Y&& y) {
            return hana::plus(detail::std::forward<X>(x),
                              detail::std::forward<Y>(y));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // plus
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct plus_impl : plus_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct plus_impl<T, U, when<condition>> {
        static_assert(wrong<plus_impl<T, U>>{},
        "no definition of boost::hana::plus for the given data types");
    };

    // Cross-type overload
    template <typename T, typename U>
    struct plus_impl<T, U, when<detail::has_common_embedding<Monoid, T, U>{}>> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::plus(to<C>(detail::std::forward<X>(x)),
                              to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // zero
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct zero_impl : zero_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct zero_impl<M, when<condition>> {
        static_assert(wrong<zero_impl<M>>{},
        "no definition of boost::hana::zero for the given data type");
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Monoid(T), when<detail::std::is_arithmetic<T>{}>>
        : detail::std::true_type
    { };

    template <typename T>
    struct plus_impl<T, T, when<detail::std::is_arithmetic<T>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) + detail::std::forward<Y>(y); }
    };

    template <typename T>
    struct zero_impl<T, when<detail::std::is_arithmetic<T>{}>> {
        static constexpr T apply()
        { return static_cast<T>(0); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_HPP
