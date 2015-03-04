/*!
@file
Defines `boost::hana::Monoid`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONOID_HPP
#define BOOST_HANA_MONOID_HPP

#include <boost/hana/fwd/monoid.hpp>

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
#include <boost/hana/detail/std/is_same.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename X, typename Y, typename = typename detail::std::enable_if<
            has_operator<datatype_t<X>, decltype(plus)>::value ||
            has_operator<datatype_t<Y>, decltype(plus)>::value
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
    struct plus_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr void apply(X&&, Y&&) {
            static_assert(wrong<plus_impl<T, U>, X, Y>{},
            "no definition of boost::hana::plus for the given data types");
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct plus_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Monoid, T, U>{}
    >> {
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
    struct zero_impl<M, when<condition>> : default_ {
        template <typename ...Nothing>
        static constexpr void apply(Nothing&& ...) {
            static_assert(wrong<zero_impl<M>, Nothing...>{},
            "no definition of boost::hana::zero for the given data type");
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename M>
    struct models<Monoid(M)>
        : detail::std::integral_constant<bool,
            !is_default<zero_impl<M>>{} &&
            !is_default<plus_impl<M, M>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for non-boolean arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct plus_impl<T, T, when<
        detail::std::is_arithmetic<T>{} && !detail::std::is_same<T, bool>{}
    >> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) + detail::std::forward<Y>(y); }
    };

    template <typename T>
    struct zero_impl<T, when<
        detail::std::is_arithmetic<T>{} && !detail::std::is_same<T, bool>{}
    >> {
        static constexpr T apply()
        { return static_cast<T>(0); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Monoid
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct plus_impl<C, C, when<
        models<Constant(C)>{} && models<Monoid(typename C::value_type)>{}
    >> {
        using T = typename C::value_type;
        template <typename X, typename Y>
        struct _constant {
            static constexpr decltype(auto) get() {
                return boost::hana::plus(boost::hana::value<X>(),
                                         boost::hana::value<Y>());
            }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const&, Y const&)
        { return to<C>(_constant<X, Y>{}); }
    };

    template <typename C>
    struct zero_impl<C, when<
        models<Constant(C)>{} && models<Monoid(typename C::value_type)>{}
    >> {
        using T = typename C::value_type;
        struct _constant {
            static constexpr decltype(auto) get()
            { return boost::hana::zero<T>(); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        static constexpr decltype(auto) apply()
        { return to<C>(_constant{}); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONOID_HPP
