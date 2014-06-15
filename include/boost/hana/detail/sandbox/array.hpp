/*!
@file
Defines `boost::hana::sandbox::array`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_SANDBOX_ARRAY_HPP
#define BOOST_HANA_DETAIL_SANDBOX_ARRAY_HPP

#include <boost/hana/detail/sandbox/algorithm.hpp>

#include <cstddef>      // for std::size_t and std::ptrdiff_t
#include <functional>   // for std::less and std::equal_to
#include <iterator>     // for std::reverse_iterator<>
#include <stdexcept>    // for std::out_of_range
#include <type_traits>  // for std::integral_constant
#include <utility>      // for std::swap and std::declval


namespace boost { namespace hana { namespace sandbox {
    namespace array_detail {
        template <typename T>
        constexpr bool is_nothrow_swappable = noexcept(
            std::swap(std::declval<T>(), std::declval<T>())
        );
    }

    template <typename T, std::size_t Size>
    struct array {
        //////////////////////////////////////////////////////////////////////
        // Aliases
        //////////////////////////////////////////////////////////////////////
        using value_type             = T;
        using reference              = value_type&;
        using const_reference        = value_type const&;
        using iterator               = value_type*;
        using const_iterator         = value_type const*;
        using pointer                = value_type*;
        using const_pointer          = value_type const*;
        using size_type              = std::size_t;
        using difference_type        = std::ptrdiff_t;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;


        //////////////////////////////////////////////////////////////////////
        // Element access
        //////////////////////////////////////////////////////////////////////
        reference operator[](size_type n) { return elems_[n]; }
        constexpr const_reference operator[](size_type n) const { return elems_[n]; }

        constexpr reference at(size_type n) {
            if (n >= size())
                throw std::out_of_range(__func__);
            return (*this)[n];
        }

        constexpr const_reference at(size_type n) const {
            if (n >= size())
                throw std::out_of_range(__func__);
            return (*this)[n];
        }

        constexpr reference front()                   { return (*this)[0]; }
        constexpr const_reference front() const       { return (*this)[0]; }
        constexpr reference back()                    { return (*this)[size() - 1]; }
        constexpr const_reference back() const        { return (*this)[size() - 1]; }
        constexpr pointer data() noexcept             { return &(*this)[0]; }
        constexpr const_pointer data() const noexcept { return &(*this)[0]; }


        //////////////////////////////////////////////////////////////////////
        // Iterators
        //////////////////////////////////////////////////////////////////////
        constexpr iterator begin() noexcept             { return data(); }
        constexpr const_iterator begin() const noexcept { return data(); }
        constexpr iterator end() noexcept               { return begin() + size(); }
        constexpr const_iterator end() const noexcept   { return begin() + size(); }

        constexpr reverse_iterator rbegin() noexcept { return reverse_iterator{end()}; }
        constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{end()}; }
        constexpr reverse_iterator rend() noexcept { return reverse_iterator{begin()}; }
        constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator{begin()}; }

        constexpr const_iterator cbegin() const noexcept { return begin(); }
        constexpr const_iterator cend() const noexcept { return end(); }
        constexpr const_reverse_iterator crbegin() const noexcept { return rbegin(); }
        constexpr const_reverse_iterator crend() const noexcept { return rend(); }


        //////////////////////////////////////////////////////////////////////
        // Capacity
        //////////////////////////////////////////////////////////////////////
        constexpr size_type size() const noexcept { return Size; }
        constexpr size_type max_size() const noexcept { return size(); }
        constexpr bool empty() const noexcept { return size() == 0; }


        //////////////////////////////////////////////////////////////////////
        // Operations
        //////////////////////////////////////////////////////////////////////
        void fill(const_reference value) {
            for (size_type i = 0; i < size(); ++i)
                (*this)[i] = value;
        }

        void swap(array& other)
            noexcept(Size == 0 || array_detail::is_nothrow_swappable<T&>)
        {
            using std::swap;
            for (size_type i = 0; i < size(); ++i)
                swap((*this)[i], other[i]);
        }


        //////////////////////////////////////////////////////////////////////
        // Internal
        //////////////////////////////////////////////////////////////////////
        value_type elems_[Size > 0 ? Size : 1];
    };

    //////////////////////////////////////////////////////////////////////
    // Comparison
    //////////////////////////////////////////////////////////////////////
    template <typename T, std::size_t SizeT, typename U, std::size_t SizeU>
    constexpr bool operator==(array<T, SizeT> const& x, array<U, SizeU> const& y) {
        return std_equal(x.cbegin(), x.cend(), y.cbegin(), y.cend(), std::equal_to<>{});
    }

    template <typename T, std::size_t SizeT, typename U, std::size_t SizeU>
    constexpr bool operator!=(array<T, SizeT> const& x, array<U, SizeU> const& y)
    { return !(x == y); }

    template <typename T, std::size_t SizeT, typename U, std::size_t SizeU>
    constexpr bool operator<(array<T, SizeT> const& x, array<U, SizeU> const& y) {
        return std_lexicographical_compare(x.cbegin(), x.cend(), y.cbegin(), y.cend(), std::less<>{});
    }

    template <typename T, std::size_t SizeT, typename U, std::size_t SizeU>
    constexpr bool operator>(array<T, SizeT> const& x, array<U, SizeU> const& y)
    { return y < x; }

    template <typename T, std::size_t SizeT, typename U, std::size_t SizeU>
    constexpr bool operator<=(array<T, SizeT> const& x, array<U, SizeU> const& y)
    { return !(y < x); }

    template <typename T, std::size_t SizeT, typename U, std::size_t SizeU>
    constexpr bool operator>=(array<T, SizeT> const& x, array<U, SizeU> const& y)
    { return !(x < y); }


    //////////////////////////////////////////////////////////////////////
    // get<>(...)
    //////////////////////////////////////////////////////////////////////
    template <std::size_t I, typename T, std::size_t N>
    constexpr T& get(array<T, N>& a) {
        static_assert(I < N,
        "index out of bounds in boost::hana::get<>(boost::hana::array&)");
        return a[I];
    }

    template <std::size_t I, typename T, std::size_t N>
    constexpr T&& get(array<T, N>&& a) {
        static_assert(I < N,
        "index out of bounds in boost::hana::get<>(boost::hana::array&&)");
        return std::move(a[I]);
    }

    template <std::size_t I, typename T, std::size_t N>
    constexpr T const& get(array<T, N> const& a) {
        static_assert(I < N,
        "index out of bounds in boost::hana::get<>(boost::hana::array const&)");
        return a[I];
    }

    template <typename T, std::size_t N>
    void swap(array<T, N>& x, array<T, N>& y) noexcept(noexcept(x.swap(y)))
    { x.swap(y); }
}}} // end namespace boost::hana::sandbox

#if 0 // for some reason, this does not work
namespace std {
    // Seriously guys, using a class for `tuple_size`?
    template <typename T>
    class tuple_size;

    template <typename T, size_t N>
    class tuple_size< ::boost::hana::sandbox::array<T, N>>
        : public integral_constant<size_t, N>
    { };

    template <size_t I, typename Tuple>
    struct tuple_element;

    template <size_t I, typename T, size_t N>
    struct tuple_element<I, ::boost::hana::sandbox::array<T, N>> {
        using type = T;
    };
} // end namespace std
#endif

#endif // !BOOST_HANA_DETAIL_SANDBOX_ARRAY_HPP
