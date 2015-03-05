/*!
@file
Defines `boost::hana::detail::array`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_ARRAY_HPP
#define BOOST_HANA_DETAIL_ARRAY_HPP

#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana { namespace detail {
    //! @ingroup group-details
    //! A minimal `std::array` with better `constexpr` support.
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
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_ARRAY_HPP
