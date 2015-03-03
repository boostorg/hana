/*!
@file
Defines `boost::hana::detail::generate_integer_sequence` & friends.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_GENERATE_INTEGER_SEQUENCE_HPP
#define BOOST_HANA_DETAIL_GENERATE_INTEGER_SEQUENCE_HPP

#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/functional/id.hpp>


namespace boost { namespace hana { namespace detail {
    template <typename T, detail::std::size_t Size>
    struct array {
        constexpr T& operator[](detail::std::size_t n) { return elems_[n]; }
        constexpr T const& operator[](detail::std::size_t n) const { return elems_[n]; }
        constexpr detail::std::size_t size() const noexcept { return Size; }
        T elems_[Size > 0 ? Size : 1];

        constexpr T* begin() noexcept             { return elems_; }
        constexpr T const* begin() const noexcept { return elems_; }
        constexpr T* end() noexcept               { return elems_ + Size; }
        constexpr T const* end() const noexcept   { return elems_ + Size; }
    };

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
    //! constructed object of type `F` is called with an unspecified object
    //! of type `A` that behaves mostly like a builtin array, except it can be
    //! passed by value. See below for more information on the type `A`. The
    //! array passed to `F` contains the numbers in the range `[0, N-1)` of
    //! type `T`. The function `F` should return an object of the same type
    //! `A`, possibly with different contents. The contents of the returned
    //! object is what will end up in the final `integer_sequence`.
    //!
    //! By default, when no generator function is given,
    //! `generate_integer_sequence` is equivalent to `make_integer_sequence`.
    //!
    //! @note
    //! Basically, objects of type `A` can be indexed with `operator[]`, they
    //! have a `size()` method and they can be passed by value, unlike builtin
    //! arrays. They also have `begin()` and `end()` methods for
    //! interoperability with the standard containers.
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
