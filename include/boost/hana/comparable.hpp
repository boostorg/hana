/*!
@file
Defines `boost::hana::Comparable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_COMPARABLE_HPP
#define BOOST_HANA_COMPARABLE_HPP

#include <boost/hana/fwd/comparable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/equality_comparable.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        //! Equivalent to `equal`.
        //! @relates boost::hana::Comparable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Comparable, datatype_t<X>>::value ||
            enable_operators<Comparable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator==(X&& x, Y&& y) {
            return hana::equal(detail::std::forward<X>(x),
                               detail::std::forward<Y>(y));
        }

        //! Equivalent to `not_equal`.
        //! @relates boost::hana::Comparable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Comparable, datatype_t<X>>::value ||
            enable_operators<Comparable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator!=(X&& x, Y&& y) {
            return hana::not_equal(detail::std::forward<X>(x),
                                   detail::std::forward<Y>(y));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // equal
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct equal_impl : equal_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct equal_impl<T, U, when<condition>> {
        static_assert(!is_convertible<T, U>{} && !is_convertible<U, T>{},
        "no default implementation of boost::hana::equal is provided for "
        "related data types that can't be safely embedded into a common type");

        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&&, Y&&)
        { return false_; }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct equal_impl<T, U, when<detail::has_common_embedding<Comparable, T, U>{}>> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::equal(to<C>(detail::std::forward<X>(x)),
                               to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // not_equal
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct not_equal_impl : not_equal_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct not_equal_impl<T, U, when<condition>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::not_(hana::equal(detail::std::forward<X>(x),
                                          detail::std::forward<Y>(y)));
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct not_equal_impl<T, U, when<detail::has_common_embedding<Comparable, T, U>{}>> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::not_equal(to<C>(detail::std::forward<X>(x)),
                                   to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for EqualityComparable data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Comparable(T), when<detail::concept::EqualityComparable<T>{}>>
        : detail::std::true_type
    { };

    template <typename T>
    struct equal_impl<T, T, when<detail::concept::EqualityComparable<T>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) == detail::std::forward<Y>(y); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_HPP
