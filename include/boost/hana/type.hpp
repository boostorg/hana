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
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/integral_constant.hpp>

#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // _type
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename T>
    struct _type<T>::_ : _type<T>, operators::adl {
        using hana = _;
        using datatype = Type;

        using type = T;

        constexpr auto operator+() const { return *this; }
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // decltype_
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <typename T, typename = Type>
        struct decltype_t {
            using type = typename std::remove_reference<T>::type;
        };

        template <typename T>
        struct decltype_t<T,
            typename std::remove_reference<T>::type::hana::datatype
        > {
            using type = typename std::remove_reference<T>::type::type;
        };
    }

    //! @cond
    template <typename T>
    constexpr auto decltype_t::operator()(T&&) const
    { return type<typename detail::decltype_t<T>::type>; }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // make<Type>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Type> {
        template <typename T>
        static constexpr auto apply(T&& t)
        { return hana::decltype_(static_cast<T&&>(t)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // sizeof_
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename T>
    constexpr auto sizeof_t::operator()(T&&) const
    { return hana::size_t<sizeof(typename detail::decltype_t<T>::type)>; }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // alignof_
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename T>
    constexpr auto alignof_t::operator()(T&&) const
    { return hana::size_t<alignof(typename detail::decltype_t<T>::type)>; }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // is_valid
    //////////////////////////////////////////////////////////////////////////
    namespace type_detail {
        template <typename F, typename ...Args, typename = decltype(
            std::declval<F&&>()(std::declval<Args&&>()...)
        )>
        constexpr auto is_valid_impl(int) { return hana::true_; }

        template <typename F, typename ...Args>
        constexpr auto is_valid_impl(...) { return hana::false_; }

        template <typename F>
        struct is_valid_fun {
            template <typename ...Args>
            constexpr auto operator()(Args&& ...) const
            { return is_valid_impl<F, Args&&...>(int{}); }
        };
    }

    //! @cond
    template <typename F>
    constexpr auto is_valid_t::operator()(F&&) const
    { return type_detail::is_valid_fun<F&&>{}; }

    template <typename F, typename ...Args>
    constexpr auto is_valid_t::operator()(F&&, Args&& ...) const
    { return type_detail::is_valid_impl<F&&, Args&&...>(int{}); }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // template_
    //////////////////////////////////////////////////////////////////////////
    template <template <typename ...> class F>
    struct template_t {
        using hana = template_t;
        using datatype = Metafunction;

        template <typename ...T>
        struct apply {
            using type = F<T...>;
        };

        template <typename ...T>
        constexpr auto operator()(T&& .../*t*/) const
        { return type<F<typename detail::decltype_t<T>::type...>>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // metafunction
    //////////////////////////////////////////////////////////////////////////
    template <template <typename ...> class F>
    struct metafunction_t {
        using hana = metafunction_t;
        using datatype = Metafunction;

        template <typename ...T>
        using apply = F<T...>;

        template <typename ...T>
        constexpr auto operator()(T&& ...) const -> decltype(
            type<typename F<typename detail::decltype_t<T>::type...>::type>
        ) { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // integral
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct integral_t {
        template <typename ...T>
        constexpr auto operator()(T&& ...) const {
            using Result = typename F::template apply<
                typename detail::decltype_t<T>::type...
            >::type;
            return Result{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<Type> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Type, Type> {
        template <typename T, typename U>
        static constexpr auto apply(_type<T> const&, _type<U> const&)
        { return hana::false_; }

        template <typename T>
        static constexpr auto apply(_type<T> const&, _type<T> const&)
        { return hana::true_; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
