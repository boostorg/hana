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

        constexpr auto operator+() const { return *this; }
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // sizeof_
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename T>
    constexpr auto _sizeof::operator()(T const&) const
    { return size_t<sizeof(typename T::type)>; }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // template
    //////////////////////////////////////////////////////////////////////////
    template <template <typename ...> class F>
    struct _template {
        struct hana { using datatype = Metafunction; };

        template <typename ...T>
        struct apply {
            using type = F<T...>;
        };

        template <typename ...T>
        constexpr auto operator()(T const& ...) const
        { return type<F<typename T::type...>>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // metafunction
    //////////////////////////////////////////////////////////////////////////
    template <template <typename ...> class F>
    struct _metafunction {
        struct hana { using datatype = Metafunction; };

        template <typename ...T>
        using apply = F<T...>;

        template <typename ...T>
        constexpr auto operator()(T const& ...) const -> decltype(
            type<typename F<typename T::type...>::type>
        )
        { return type<typename F<typename T::type...>::type>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // trait
    //////////////////////////////////////////////////////////////////////////
    template <template <typename ...> class F>
    struct _trait {
       template <typename ...T>
       constexpr auto operator()(T const& ...) const
       { return F<typename T::type...>{}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // trait_
    //////////////////////////////////////////////////////////////////////////
    template <template <typename ...> class F>
    struct _trait_ {
        template <typename ...T>
        constexpr auto operator()(T const& ...) const
        { return F<T...>{}; }
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
        static constexpr auto apply(_type<T> const&, _type<U> const&)
        { return false_; }

        template <typename T>
        static constexpr auto apply(_type<T> const&, _type<T> const&)
        { return true_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
