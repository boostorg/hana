/*!
@file
Defines `boost::hana::detail::closure`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CLOSURE_HPP
#define BOOST_HANA_DETAIL_CLOSURE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/fast_and.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>


namespace boost { namespace hana { namespace detail {
    // We use intrinsics if they are available because it speeds up the
    // compile-times.
#if defined(BOOST_HANA_CONFIG_CLANG)
#   if __has_extension(is_empty)
#       define BOOST_HANA_TT_IS_EMPTY(T) __is_empty(T)
#   endif

#   if __has_extension(is_final)
#       define BOOST_HANA_TT_IS_FINAL(T) __is_final(T)
#   endif

    // TODO: Right now, this intrinsic is never used directly because of
    //       https://llvm.org/bugs/show_bug.cgi?id=24173
#   if __has_extension(is_constructible) && false
#       define BOOST_HANA_TT_IS_CONSTRUCTIBLE(...) __is_constructible(__VA_ARGS__)
#   endif

#   if __has_extension(is_assignable)
#       define BOOST_HANA_TT_IS_ASSIGNABLE(T, U) __is_assignable(T, U)
#   endif
#endif

#if !defined(BOOST_HANA_TT_IS_EMPTY)
#   define BOOST_HANA_TT_IS_EMPTY(T) ::std::is_empty<T>::value
#endif

#if !defined(BOOST_HANA_TT_IS_FINAL)
#   define BOOST_HANA_TT_IS_FINAL(T) ::std::is_final<T>::value
#endif

#if !defined(BOOST_HANA_TT_IS_CONSTRUCTIBLE)
#   define BOOST_HANA_TT_IS_CONSTRUCTIBLE(...) ::std::is_constructible<__VA_ARGS__>::value
#endif

#if !defined(BOOST_HANA_TT_IS_ASSIGNABLE)
#   define BOOST_HANA_TT_IS_ASSIGNABLE(T, U) ::std::is_assignable<T, U>::value
#endif

    //////////////////////////////////////////////////////////////////////////
    // element<n, Xn>
    //
    // Wrapper holding the actual elements of a tuple. It takes care of
    // optimizing the storage for empty types.
    //////////////////////////////////////////////////////////////////////////
    template <std::size_t n, typename Xn, bool =
        BOOST_HANA_TT_IS_EMPTY(Xn) && !BOOST_HANA_TT_IS_FINAL(Xn)
    >
    struct element;

    // Specialize storage for empty types
    template <std::size_t n, typename Xn>
    struct element<n, Xn, true> : Xn {
        element(element const&) = default;
        element(element&&) = default;
        element(element&) = default;

        template <typename ...T>
        constexpr element(T&& ...t)
            : Xn(static_cast<T&&>(t)...)
        { }

        template <typename T>
        element& operator=(T&& t) {
            Xn::operator=(static_cast<T&&>(t));
            return *this;
        }
    };

    template <std::size_t n, typename Xn>
    constexpr Xn const& get_impl(element<n, Xn, true> const& xn)
    { return xn; }

    template <std::size_t n, typename Xn>
    constexpr Xn& get_impl(element<n, Xn, true>& xn)
    { return xn; }

    template <std::size_t n, typename Xn>
    constexpr Xn&& get_impl(element<n, Xn, true>&& xn)
    { return static_cast<Xn&&>(xn); }


    // Specialize storage for non-empty types
    template <std::size_t n, typename Xn>
    struct element<n, Xn, false> {
        element(element const&) = default;
        element(element&&) = default;
        element(element&) = default;

        template <typename ...T>
        constexpr element(T&& ...t)
            : data_(static_cast<T&&>(t)...)
        { }

        template <typename T>
        element& operator=(T&& t) {
            this->data_ = static_cast<T&&>(t);
            return *this;
        }

        Xn data_;
    };

    template <std::size_t n, typename Xn>
    constexpr Xn const& get_impl(element<n, Xn, false> const& xn)
    { return xn.data_; }

    template <std::size_t n, typename Xn>
    constexpr Xn& get_impl(element<n, Xn, false>& xn)
    { return xn.data_; }

    template <std::size_t n, typename Xn>
    constexpr Xn&& get_impl(element<n, Xn, false>&& xn)
    { return static_cast<Xn&&>(xn.data_); }

    //////////////////////////////////////////////////////////////////////////
    // closure_impl
    //////////////////////////////////////////////////////////////////////////
    template <typename Indices, typename ...Xn>
    struct closure_impl;

    template <std::size_t ...n, typename ...Xn>
    struct closure_impl<std::index_sequence<n...>, Xn...>
        : element<n, Xn>...
    {
        // 1)
        //
        // Note: We use dummy to delay the instantiation of that constructor.
        template <std::size_t dummy = 0>
        explicit constexpr closure_impl()
            : element<(dummy, n), Xn>()...
        { }

        // 2) & 3)
        template <typename ...Yn>
        explicit constexpr closure_impl(Yn&& ...yn)
            : element<n, Xn>(static_cast<Yn&&>(yn))...
        { }

        // 4)
        template <typename ...Yn>
        constexpr closure_impl(closure_impl<std::index_sequence<n...>, Yn...> const& other)
            : element<n, Xn>(detail::get_impl<n>(other))...
        { }

        // 5)
        template <typename ... Yn>
        constexpr closure_impl(closure_impl<std::index_sequence<n...>, Yn...>&& other)
            : element<n, Xn>(static_cast<Yn&&>(detail::get_impl<n>(other)))...
        { }

        // 8)
        closure_impl(closure_impl const&) = default;

        // 9)
        closure_impl(closure_impl&&) = default;

        // Extension:
        closure_impl(closure_impl&) = default;

        // 1)
        closure_impl& operator=(closure_impl const& other) {
            int sequence[] = {int{}, ((void)(
                detail::get_impl<n>(static_cast<element<n, Xn>&>(*this))
                                =
                detail::get_impl<n>(static_cast<element<n, Xn> const&>(other))
            ), int{})...};
            (void)sequence;
            return *this;
        }

        // 2)
        closure_impl& operator=(closure_impl&& other) {
            int sequence[] = {int{}, ((void)(
                detail::get_impl<n>(static_cast<element<n, Xn>&>(*this))
                                =
                detail::get_impl<n>(static_cast<element<n, Xn>&&>(other))
            ), int{})...};
            (void)sequence;
            return *this;
        }

        // 3)
        template <typename ...Yn>
        closure_impl&
        operator=(closure_impl<std::index_sequence<n...>, Yn...> const& other) {
            int sequence[] = {int{}, ((void)(
                detail::get_impl<n>(static_cast<element<n, Xn>&>(*this))
                                =
                detail::get_impl<n>(static_cast<element<n, Yn> const&>(other))
            ), int{})...};
            (void)sequence;
            return *this;
        }

        // 4)
        template <typename ...Yn>
        closure_impl&
        operator=(closure_impl<std::index_sequence<n...>, Yn...>&& other) {
            int sequence[] = {int{}, ((void)(
                detail::get_impl<n>(static_cast<element<n, Xn>&>(*this))
                                =
                detail::get_impl<n>(static_cast<element<n, Yn>&&>(other))
            ), int{})...};
            (void)sequence;
            return *this;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // closure
    //
    // Front-end closure object.
    //////////////////////////////////////////////////////////////////////////
    template <typename ...Xn>
    struct closure {
        using Indices = std::make_index_sequence<sizeof...(Xn)>;
        using Storage = closure_impl<Indices, Xn...>;
        Storage storage_;

        //////////////////////////////////////////////////////////////////////
        // Constructors. The specification is taken from cppreference.org
        //////////////////////////////////////////////////////////////////////

        // 1) Default constructor. Value-initializes all elements.
        template <typename ...dummy, typename = typename std::enable_if<
            fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, dummy...)...>::value
        >::type>
        constexpr closure() { }

        // 2) Initializes each element of the closure with the
        //    corresponding parameter.
        //
        //    Extension: This constructor is not explicit.
        template <typename ...dummy, typename = typename std::enable_if<
            fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Xn const&, dummy...)...>::value
        >::type>
        /*explicit*/ constexpr closure(Xn const& ...xn)
            : storage_(xn...)
        { }

        // 3) Initializes each element of the closure with the corresponding
        //    value in std::forward<Yn>(yn).
        //
        //    Extension: This constructor is not explicit.
        template <typename ...Yn, typename = typename std::enable_if<
            fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Yn&&)...>::value
        >::type>
        /*explicit*/ constexpr closure(Yn&& ...yn)
            : storage_(static_cast<Yn&&>(yn)...)
        { }

        // 4) For all i in sizeof...(Yn), initializes ith element of the
        //    closure with the ith element of 'other'.
        template <typename ...Yn, typename = typename std::enable_if<
            fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Yn const&)...>::value
        >::type>
        constexpr closure(closure<Yn...> const& other)
            : storage_(other.storage_)
        { }

        // 5) For all i in sizeof...(Yn), initializes ith element of the
        //    closure with std::forward<Yi>(std::get<i>(other)).
        template <typename ...Yn, typename = typename std::enable_if<
            fast_and<BOOST_HANA_TT_IS_CONSTRUCTIBLE(Xn, Yn&&)...>::value
        >::type>
        constexpr closure(closure<Yn...>&& other)
            : storage_(static_cast<closure<Yn...>&&>(other).storage_)
        { }

        // 6) and 7) are the pair constructors. They are not provided in Hana.

        // 8) Compiler-generated copy constructor. Initializes each element of
        //    the closure with the corresponding element of 'other'. This
        //    constructor is constexpr if every operation it performs is
        //    constexpr. For the empty closure, it is constexpr.
        closure(closure const&) = default;

        // 9) Compiler-generated move constructor. Initializes each ith
        //    element of the closure with std::forward<Ui>(std::get<i>(other)).
        //    This constructor is constexpr if every operation it performs is
        //    constexpr. For the empty closure, it is constexpr.
        closure(closure&&) = default;

        // Extension:
        // We provide a defaulted copy constructor from non-const closures,
        // because otherwise the Yn&& constructor would be selected instead,
        // which would try to see the closure as an element of itself.
        closure(closure&) = default;


        //////////////////////////////////////////////////////////////////////
        // Assignment. The specification is taken from cppreference.org
        //////////////////////////////////////////////////////////////////////

        // 1) Copy assignment operator. Replaces each element with a copy of
        //    the corresponding element of 'other'.
        closure& operator=(closure const& other) {
            this->storage_ = other.storage_;
            return *this;
        }

        // 2) Move assignment operator. Replaces each element with the
        //    corresponding element of other using move semantics.
        closure& operator=(closure&& other) {
            this->storage_ = static_cast<closure&&>(other).storage_;
            return *this;
        }

        // 3) For all i, assigns std::get<i>(other) to std::get<i>(*this).
        template <typename ...Yn, typename = typename std::enable_if<
            fast_and<BOOST_HANA_TT_IS_ASSIGNABLE(Xn&, Yn const&)...>::value
        >::type>
        closure& operator=(closure<Yn...> const& other) {
            this->storage_ = other.storage_;
            return *this;
        }

        // 4) For all i, assigns std::forward<Ui>(std::get<i>(other)) to
        //    std::get<i>(*this).
        template <typename ...Yn, typename = typename std::enable_if<
            fast_and<BOOST_HANA_TT_IS_ASSIGNABLE(Xn&, Yn&&)...>::value
        >::type>
        closure& operator=(closure<Yn...>&& other) {
            this->storage_ = static_cast<closure<Yn...>&&>(other).storage_;
            return *this;
        }

        // 5) & 6) are assignment from pairs, and they are not supported in Hana.
    };

    template <>
    struct closure<> {
        constexpr closure() { }
    };

    //////////////////////////////////////////////////////////////////////////
    // get
    //////////////////////////////////////////////////////////////////////////
    template <std::size_t i, typename Closure>
    constexpr decltype(auto) get(Closure&& xs) {
        return detail::get_impl<i>(static_cast<Closure&&>(xs).storage_);
    }
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_CLOSURE_HPP
