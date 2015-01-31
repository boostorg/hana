/*!
@file
Defines `boost::hana::Constant`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONSTANT_HPP
#define BOOST_HANA_CONSTANT_HPP

#include <boost/hana/fwd/constant.hpp>

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // value
    //////////////////////////////////////////////////////////////////////////
    template <typename C, typename>
    struct value_impl : value_impl<C, when<true>> { };

    template <typename C, bool condition>
    struct value_impl<C, when<condition>> {
        static_assert(wrong<value_impl<C>>{},
        "no definition of boost::hana::value for the given data type");
    };

    //////////////////////////////////////////////////////////////////////////
    // Provided conversions
    //////////////////////////////////////////////////////////////////////////
    template <typename To, typename From>
    struct convert<To, From, when<
        models<Constant(From)>{} && is_convertible<typename From::value_type, To>{}
    >> {
        template <typename X>
        static constexpr decltype(auto) apply(X x)
        { return to<To>(hana::value(x)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Provided common data type
    //////////////////////////////////////////////////////////////////////////
    template <typename A, typename B>
    struct common<A, B, when<
        models<Constant(A)>{} &&
        models<Constant(B)>{} &&
        has_common<typename A::value_type, typename B::value_type>{}
    >> {
        using type = detail::CanonicalConstant<
            typename common<typename A::value_type,
                            typename B::value_type>::type
        >;
    };

    //////////////////////////////////////////////////////////////////////////
    // detail::CanonicalConstant
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Constant(detail::CanonicalConstant<T>)>
        : detail::std::true_type
    { };

    template <typename T>
    struct value_impl<detail::CanonicalConstant<T>> {
        template <typename X>
        static constexpr decltype(auto) apply(X)
        { return X::get(); }
    };

    template <typename T, typename C>
    struct convert<detail::CanonicalConstant<T>, C, when<
        models<Constant(C)>{} && is_convertible<typename C::value_type, T>{}
    >>
        : embedding_if<is_embedding<convert<T, typename C::value_type>>{}>
    {
        template <typename X>
        struct _constant {
            static constexpr decltype(auto) get()
            { return to<T>(boost::hana::value2<X>()); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename X>
        static constexpr _constant<X> apply(X const&)
        { return {}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONSTANT_HPP
