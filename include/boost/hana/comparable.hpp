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
#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
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

    // Additional tag-dispatching for `equal_impl`:
    //
    // 4. If `T` and `U` can be compared using `==`, use that.
    template <typename T, typename U, typename Context>
    struct dispatch_impl<4, equal_impl<T, U>, Context> {
        template <typename T_ = T, typename U_ = U, typename = void>
        struct impl : dispatch_impl<5, equal_impl<T_, U_>, Context>::type { };

        template <typename T_, typename U_>
        struct impl<T_, U_, decltype((void)(
            detail::std::declval<T_>() == detail::std::declval<U_>()
        ))> {
            template <typename X, typename Y>
            static constexpr decltype(auto) apply(X&& x, Y&& y)
            { return detail::std::forward<X>(x) == detail::std::forward<Y>(y); }
        };

        using type = impl<>;
    };

    // 5.1 If `T` and `U` are the same data type, then fail.
    template <typename T, typename Context>
    struct dispatch_impl<5, equal_impl<T, T>, Context> {
        using type = not_implemented<equal_impl<T, T>>;
    };

    // 5.2 If `T` and `U` are distinct and have a Comparable common type,
    //     then perform the comparison in that data type.
    template <typename T, typename U, typename Context>
    struct dispatch_impl<5, equal_impl<T, U>, Context> {
        template <typename C>
        struct impl {
            template <typename X, typename Y>
            static constexpr decltype(auto) apply(X&& x, Y&& y) {
                return equal(to<C>(detail::std::forward<X>(x)),
                             to<C>(detail::std::forward<Y>(y)));
            }
        };

        template <typename T_, typename U_,
                  typename C = typename common<T_, U_>::type>
        static impl<C> check(detail::std::integral_constant<bool,
            models<Comparable, C, Context>
        >);

        template <typename T_, typename U_>
        static typename dispatch_impl<6, equal_impl<T_, U_>, Context>::type check(...);

        using type = decltype(check<T, U>(detail::std::true_type{}));
    };

    // 6. Otherwise, if `T` and `U` are unrelated, i.e. they don't have a
    //    common type, always return `false_`.
    template <typename T, typename U, typename Context>
    struct dispatch_impl<6, equal_impl<T, U>, Context> {
        template <typename T_, typename U_,
                  typename C = typename common<T_, U_>::type>
        static not_implemented<equal_impl<T, U>> check(void*);

        struct impl {
            template <typename X, typename Y>
            static constexpr decltype(auto) apply(X&&, Y&&)
            { return false_; }
        };

        template <typename T_, typename U_>
        static impl check(...);

        using type = decltype(check<T, U>(nullptr));
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

    template <>
    struct models_impl<Comparable> {
        template <typename T, typename Context>
        static constexpr bool apply = is_implemented<equal_impl<T, T>, Context>;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_COMPARABLE_HPP

#include <boost/hana/bool.hpp>
