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

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/fwd/bool.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Equivalent to `equal`.
        //! @relates boost::hana::Comparable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Comparable, datatype_t<X>>::value ||
            enable_operators<Comparable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator==(X&& x, Y&& y) {
            return equal(detail::std::forward<X>(x),
                         detail::std::forward<Y>(y));
        }

        //! Equivalent to `not_equal`.
        //! @relates boost::hana::Comparable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Comparable, datatype_t<X>>::value ||
            enable_operators<Comparable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator!=(X&& x, Y&& y) {
            return not_equal(detail::std::forward<X>(x),
                             detail::std::forward<Y>(y));
        }
    }

    // equal
    /////////////////
    namespace comparable_detail {
        template <typename T, typename U, typename _, typename ...Nothing>
        constexpr bool has_Comparable_common(Nothing ...) { return false; }
        template <typename T, typename U, typename _, typename C = typename common<T, U>::type>
        constexpr bool has_Comparable_common()
        { return is_implemented<equal_impl<C, C>, _>; }

        template <typename T, typename U, typename ...Nothing>
        constexpr bool has_common(Nothing ...) { return false; }
        template <typename T, typename U, typename = typename common<T, U>::type>
        constexpr bool has_common() { return true; }

        template <typename T, typename U, typename ...Nothing>
        constexpr bool has_operator_equal(Nothing ...) { return false; }
        template <typename T, typename U, typename = decltype(
            detail::std::declval<T>() == detail::std::declval<U>()
        )>
        constexpr bool has_operator_equal() { return true; }
    }

    // 1. If `not_equal` is implemented, use that.
    template <typename T, typename U, typename _>
    struct equal_impl<T, U, when<is_implemented<not_equal_impl<T, U>, _>>, _> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return not_(not_equal(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            ));
        }
    };

    // 2. Otherwise, if `T` and `U` can be compared using `==`, use that.
    template <typename T, typename U, typename _>
    struct equal_impl<T, U, when<
        !is_implemented<not_equal_impl<T, U>, _> &&
        comparable_detail::has_operator_equal<T, U>()
    >, _> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return detail::std::forward<X>(x) == detail::std::forward<Y>(y);
        }
    };

    // 3. Otherwise, if `T` and `U` are distinct and there exists a
    //    `common_t<T, U>` that is Comparable, perform the comparison
    //    in that data type.
    template <typename T, typename U, typename _>
    struct equal_impl<T, U, when<
        !is_implemented<not_equal_impl<T, U>, _> &&
        !comparable_detail::has_operator_equal<T, U>() &&
        !detail::std::is_same<T, U>::value &&
        comparable_detail::has_Comparable_common<T, U, _>()
    >, _> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            using C = typename common<T, U>::type;
            return equal(
                to<C>(detail::std::forward<X>(x)),
                to<C>(detail::std::forward<Y>(y))
            );
        }
    };

    // 4. Otherwise, if `T` and `U` are distinct and there is no
    //    common data type at all, return `false_`.
    template <typename T, typename U, typename _>
    struct equal_impl<T, U, when<
        !is_implemented<not_equal_impl<T, U>, _> &&
        !comparable_detail::has_operator_equal<T, U>() &&
        !detail::std::is_same<T, U>::value &&
        !comparable_detail::has_common<T, U>()
    >, _> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&&, Y&&)
        { return false_; }
    };


    // not_equal
    /////////////////
    template <typename T, typename U, typename _>
    struct not_equal_impl<T, U, when<
        is_implemented<equal_impl<T, U>, _>
    >, _> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return not_(equal(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            ));
        }
    };

    // is_a
    /////////////////
    template <typename T, typename U>
    constexpr auto is_a<Comparable, T, U> = bool_<
        is_implemented<equal_impl<T, U>> ||
        is_implemented<not_equal_impl<T, U>>
    >;
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_HPP

#include <boost/hana/bool.hpp>
