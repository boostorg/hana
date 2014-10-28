/*!
@file
Defines `boost::hana::detail::closure`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CLOSURE_HPP
#define BOOST_HANA_DETAIL_CLOSURE_HPP

#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana { namespace detail {
    template <detail::std::size_t Index, typename X>
    struct closure_element { X get; };

    void swallow(...);

    // This type is only used for pattern matching.
    template <typename ...Xn>
    struct closure
        : Xn...
    {
        closure() = default;

        // Make sure the constructor is SFINAE-friendly.
        template <typename ...Yn, typename = decltype(swallow(
            (Xn{detail::std::declval<Yn>()}, 0)...
        ))>
        explicit constexpr closure(Yn&& ...yn)
            : Xn{detail::std::forward<Yn>(yn)}...
        { }
    };

    template <typename Indices, typename ...Xn>
    struct make_closure_impl;

    template <detail::std::size_t ...Indices, typename ...Xn>
    struct make_closure_impl<detail::std::index_sequence<Indices...>, Xn...> {
        using type = closure<closure_element<Indices, Xn>...>;
    };

    // This alias creates the type of a closure containing `Xn...`.
    template <typename ...Xn>
    using closure_t = typename make_closure_impl<
        detail::std::make_index_sequence<sizeof...(Xn)>,
        Xn...
    >::type;
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_CLOSURE_HPP
