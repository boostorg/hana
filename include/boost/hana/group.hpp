/*!
@file
Defines `boost::hana::Group`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_GROUP_HPP
#define BOOST_HANA_GROUP_HPP

#include <boost/hana/fwd/group.hpp>

#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/monoid.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Group, datatype_t<X>>::value ||
            enable_operators<Group, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator-(X&& x, Y&& y) {
            return hana::minus(detail::std::forward<X>(x),
                               detail::std::forward<Y>(y));
        }

        template <typename X, typename = detail::std::enable_if_t<
            enable_operators<Group, datatype_t<X>>::value
        >>
        constexpr decltype(auto) operator-(X&& x)
        { return hana::negate(detail::std::forward<X>(x)); }
    }

    //////////////////////////////////////////////////////////////////////////
    // minus
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct minus_impl : minus_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct minus_impl<T, U, when<condition>> {
        static_assert(wrong<minus_impl<T, U>>{},
        "no definition of boost::hana::minus for the given data types");
    };

    template <typename T, bool condition>
    struct minus_impl<T, T, when<condition>> : default_ {
        static_assert(!is_default<negate_impl<T>>{},
        "no definition of boost::hana::minus for the given data type");

        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::plus(detail::std::forward<X>(x),
                              hana::negate(detail::std::forward<Y>(y)));
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct minus_impl<T, U, when<detail::has_common_embedding<Group, T, U>{}>> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::minus(to<C>(detail::std::forward<X>(x)),
                               to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // negate
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct negate_impl : negate_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct negate_impl<T, when<condition>> : default_ {
        static_assert(!is_default<minus_impl<T, T>>{},
        "no definition of boost::hana::negate for the given data type");

        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return hana::minus(zero<T>(), detail::std::forward<X>(x)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Group(T), when<detail::std::is_non_boolean_arithmetic<T>{}>>
        : detail::std::true_type
    { };

    template <typename T>
    struct minus_impl<T, T, when<detail::std::is_non_boolean_arithmetic<T>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) - detail::std::forward<Y>(y); }
    };

    template <typename T>
    struct negate_impl<T, when<detail::std::is_non_boolean_arithmetic<T>{}>> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return -detail::std::forward<X>(x); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Group
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct models<Group(C), when<
        models<Constant(C)>{} && models<Group(typename C::value_type)>{}
    >>
        : detail::std::true_type
    { };

    template <typename C>
    struct minus_impl<C, C, when<
        models<Constant(C)>{} && models<Group(typename C::value_type)>{}
    >> {
        using T = typename C::value_type;
        template <typename X, typename Y>
        struct _constant {
            static constexpr decltype(auto) get() {
                return boost::hana::minus(boost::hana::value(X{}),
                                          boost::hana::value(Y{}));
            }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const&, Y const&)
        { return to<C>(_constant<X, Y>{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_GROUP_HPP
