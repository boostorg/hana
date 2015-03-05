/*!
@file
Defines `boost::hana::detail::generate_integer_sequence` & friends.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_GENERATE_INTEGER_SEQUENCE_HPP
#define BOOST_HANA_DETAIL_GENERATE_INTEGER_SEQUENCE_HPP

#include <boost/hana/detail/array.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/functional/id.hpp>


namespace boost { namespace hana { namespace detail {
    template <typename T, typename F, detail::std::size_t ...i>
    constexpr auto generate_integer_sequence_impl(detail::std::index_sequence<i...>) {
        constexpr array<T, sizeof...(i)> a = {{static_cast<T>(i)...}};
        constexpr auto sequence = F{}(a);
        return detail::std::integer_sequence<T, sequence[i]...>{};
    }

    //! @ingroup group-details
    //! Generates an `integer_sequence` from an array-returning function.
    //!
    //! First, `F` must be a constexpr-callable type and `T` must be a type
    //! that can be used to create an `integer_sequence`. The following
    //! happens when `generate_integer_sequence<T, N, F>` is used: a default-
    //! constructed object of type `F` is called with a `detail::array`
    //! containing the numbers in the range `[0, N-1)` of type `T`. The
    //! function `F` should return a `detail::array`, possibly with different
    //! contents. The contents of the returned array is what will end up in
    //! the final `integer_sequence`.
    //!
    //! By default, when no generator function is given,
    //! `generate_integer_sequence` is equivalent to `make_integer_sequence`.
    template <typename T, detail::std::size_t N, typename F = decltype(id)>
    using generate_integer_sequence = decltype(generate_integer_sequence_impl<T, F>(
        detail::std::make_index_sequence<N>{}
    ));

    //! @ingroup group-details
    //! Equivalent to `generate_integer_sequence<std::size_t, N, F>`.
    template <detail::std::size_t N, typename F = decltype(id)>
    using generate_index_sequence = generate_integer_sequence<detail::std::size_t, N, F>;
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_GENERATE_INTEGER_SEQUENCE_HPP
