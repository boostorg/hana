/*!
@file
Defines `boost::hana::detail::closure`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CLOSURE_HPP
#define BOOST_HANA_DETAIL_CLOSURE_HPP

#include <cstddef>
#include <utility>


namespace boost { namespace hana { namespace detail {
    template <std::size_t n, typename Xn>
    struct element { Xn get; using get_type = Xn; };

    void swallow(...);

    // This type is only used for pattern matching.
    template <typename ...Xs>
    struct closure_impl : Xs... {
        closure_impl() = default;
        closure_impl(closure_impl&&) = default;
        closure_impl(closure_impl const&) = default;
        closure_impl(closure_impl&) = default;

        // This constructor makes it possible to use brace initializers
        // to initialize members of the closure.
        constexpr closure_impl(typename Xs::get_type&& ...xs)
            : Xs{static_cast<typename Xs::get_type&&>(xs)}...
        { }

        // Make sure the constructor is SFINAE-friendly.
        template <typename ...Ys, typename = decltype(swallow(
            (Xs{std::declval<Ys>()}, void(), 0)...
        ))>
        constexpr closure_impl(Ys&& ...y)
            : Xs{static_cast<Ys&&>(y)}...
        { }
    };

    template <>
    struct closure_impl<> { };

    template <typename Indices, typename ...Xs>
    struct make_closure_impl;

    template <std::size_t ...n, typename ...Xn>
    struct make_closure_impl<std::index_sequence<n...>, Xn...> {
        using type = closure_impl<element<n, Xn>...>;
    };

    //! @ingroup group-details
    //! Minimalistic closure implemented through multiple inheritance.
    //!
    //! This is intended to be used as a building block for other more complex
    //! data structures. The following is guaranteed by a `closure`:
    //! `closure<X1, ..., Xn>` is a type representing a closure holding
    //! objects of types `X1, ..., Xn` (basically a tuple). More specifically,
    //! `closure<X1, ..., Xn>` is an alias to
    //!
    //! @code
    //!     closure_impl<
    //!         element<0, X0>,
    //!         element<1, X1>,
    //!         ...,
    //!         element<n, Xn>
    //!     >
    //! @endcode
    //!
    //! Note that this makes `closure<X1, ..., Xn>` a dependent type, which
    //! means that pattern matching is not allowed on it. `closure_impl`
    //! inherits publicly from each of these `element`s, which makes it
    //! possible to retrieve an object from the closure based on its index.
    //! Also note that `element<n, Xn>` is guaranteed to have a nested
    //! `get_type` alias equivalent to `Xn`, which is handy to retrieve
    //! that `Xn` without having to do pattern matching.
    template <typename ...Xs>
    using closure = typename make_closure_impl<
        std::make_index_sequence<sizeof...(Xs)>, Xs...
    >::type;

    //! @ingroup group-details
    //! Get the nth element of a `closure`.
    template <std::size_t n, typename Xn>
    static constexpr Xn const&
    get(element<n, Xn> const& x)
    { return x.get; }

    template <std::size_t n, typename Xn>
    static constexpr Xn&
    get(element<n, Xn>& x)
    { return x.get; }

    template <std::size_t n, typename Xn>
    static constexpr Xn&&
    get(element<n, Xn>&& x)
    { return static_cast<element<n, Xn>&&>(x).get; }
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_CLOSURE_HPP
