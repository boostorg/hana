/*!
@file
Defines `boost::hana::Type` and `boost::hana::Metafunction`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_HPP
#define BOOST_HANA_TYPE_HPP

#include <boost/hana/fwd/type.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/integral_constant.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // _type
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename T>
    struct _type<T>::_ : _type<T>, operators::adl {
        struct hana { using datatype = Type; };
        using type = T;

        constexpr _ operator+() const { return *this; }
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // template
    //////////////////////////////////////////////////////////////////////////
    template <template <typename ...> class f>
    struct _template {
        struct hana { using datatype = Metafunction; };

        template <typename ...xs>
        struct apply {
            using type = f<xs...>;
        };

        template <typename ...xs>
        constexpr auto operator()(xs...) const
        { return type<f<typename xs::type...>>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // metafunction
    //////////////////////////////////////////////////////////////////////////
    template <template <typename ...> class f>
    struct _metafunction {
        struct hana { using datatype = Metafunction; };

        template <typename ...xs>
        using apply = f<xs...>;

        template <typename ...xs>
        constexpr auto operator()(xs...) const -> decltype(
            type<typename f<typename xs::type...>::type>
        )
        { return type<typename f<typename xs::type...>::type>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct operators::of<Type>
        : operators::of<Comparable>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Type, Type> {
        template <typename T, typename U>
        static constexpr auto apply(T, U)
        { return false_; }

        template <typename T>
        static constexpr auto apply(T, T)
        { return true_; }
    };

    //! @cond
    template <typename T>
    constexpr auto _sizeof::operator()(T) const
    { return size_t<sizeof(typename T::type)>; }
    //! @endcond
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
