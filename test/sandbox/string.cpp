/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/config.hpp>
#include "algorithm.hpp"

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <string>


namespace boost { namespace hana { namespace sandbox {
    namespace string_detail {
        template <typename SizeType, typename InputIterator>
        constexpr SizeType strlen(InputIterator it) {
            SizeType len{0};
            while (*it != '\0')
                ++it, ++len;
            return len;
        }
    }

    template <typename CharT, typename Traits = std::char_traits<CharT>>
    struct basic_string {
        //////////////////////////////////////////////////////////////////////
        // aliases and constants
        //////////////////////////////////////////////////////////////////////
        using traits_type               = Traits;
        using value_type                = typename traits_type::char_type;
        using allocator_type            = void;
        using size_type                 = std::size_t;
        using difference_type           = std::ptrdiff_t;
        using reference                 = value_type&;
        using const_reference           = value_type const&;
        using pointer                   = value_type*;
        using const_pointer             = value_type const*;
        using iterator                  = pointer;
        using const_iterator            = const_pointer;
        using reverse_iterator          = std::reverse_iterator<iterator>;
        using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
        static constexpr size_type npos = -1;


        //////////////////////////////////////////////////////////////////////
        // construction
        //////////////////////////////////////////////////////////////////////
        constexpr basic_string()
            : basic_string{""}
        { }

        constexpr basic_string(size_type count, value_type ch) = delete;
        constexpr basic_string(basic_string const& other, size_type pos, size_type count = basic_string::npos) = delete;

        constexpr basic_string(value_type const* s, size_type count)
            : buf_{s}, size_{count}
        { }

        constexpr basic_string(value_type const* s)
            : basic_string{s, string_detail::strlen<size_type>(s)}
        { }

        template <typename InputIt>
        constexpr basic_string(InputIt first, InputIt last) = delete;

        constexpr basic_string(basic_string const& other) = default;
        constexpr basic_string(basic_string&& other) = default;

        constexpr basic_string(std::initializer_list<value_type> init)
            : basic_string{init.begin(), init.size()}
        { }

        basic_string& operator=(basic_string const& str) = delete;
        basic_string& operator=(basic_string&& str) = delete;
        basic_string& operator=(value_type const* s) = delete;
        basic_string& operator=(value_type ch) = delete;
        basic_string& operator=(std::initializer_list<value_type> ilist) = delete;

        basic_string& assign(size_type count, value_type ch) = delete;
        basic_string& assign(basic_string const& str) = delete;
        basic_string& assign(basic_string const& str, size_type pos, size_type count) = delete;
        basic_string& assign(basic_string&& str) = delete;
        basic_string& assign(value_type const* s, size_type count) = delete;
        basic_string& assign(value_type const* s) = delete;
        template <typename InputIt>
        basic_string& assign(InputIt first, InputIt last) = delete;
        basic_string& assign(std::initializer_list<value_type> ilist) = delete;

        allocator_type get_allocator() const = delete;


        //////////////////////////////////////////////////////////////////////
        // element access
        //////////////////////////////////////////////////////////////////////
        constexpr const_reference operator[](size_type pos) const
        { return buf_[pos]; }
        constexpr const_reference at(size_type pos) const {
            if (pos >= size())
                throw std::out_of_range("at");
            return (*this)[pos];
        }
        constexpr const_reference front() const { return (*this)[0]; }
        constexpr const_reference back() const { return (*this)[size() - 1]; }

        constexpr reference operator[](size_type pos) = delete;
        constexpr reference at(size_type pos) = delete;
        constexpr reference front() = delete;
        constexpr reference back() = delete;
        constexpr value_type const* data() const noexcept = delete;
        constexpr value_type const* c_str() const noexcept = delete;


        //////////////////////////////////////////////////////////////////////
        // iterators
        //////////////////////////////////////////////////////////////////////
        constexpr iterator       begin() noexcept       { return {buf_}; }
        constexpr const_iterator begin() const noexcept { return {buf_}; }
        constexpr iterator       end() noexcept         { return begin() + size(); }
        constexpr const_iterator end() const noexcept   { return begin() + size(); }

        constexpr reverse_iterator rbegin() noexcept
        { return reverse_iterator{end()}; }
        constexpr const_reverse_iterator rbegin() const noexcept
        { return const_reverse_iterator{end()}; }
        constexpr reverse_iterator rend() noexcept
        { return reverse_iterator{begin()}; }
        constexpr const_reverse_iterator rend() const noexcept
        { return const_reverse_iterator{begin()}; }

        constexpr const_iterator cbegin() const noexcept
        { return begin(); }
        constexpr const_iterator cend() const noexcept
        { return end(); }
        constexpr const_reverse_iterator crbegin() const noexcept
        { return rbegin(); }
        constexpr const_reverse_iterator crend() const noexcept
        { return rend(); }


        //////////////////////////////////////////////////////////////////////
        // capacity
        //////////////////////////////////////////////////////////////////////
        constexpr size_type size()     const noexcept { return size_; }
        constexpr size_type length()   const noexcept { return size(); }
        constexpr bool      empty()    const noexcept { return size() == 0; }
        constexpr size_type max_size() const noexcept { return -1; }
        constexpr size_type capacity() const noexcept { return size(); }

        void reserve(size_type new_cap) = delete;
        void shrink_to_fit() = delete;


        //////////////////////////////////////////////////////////////////////
        // operations
        //////////////////////////////////////////////////////////////////////
        // ...

    private:
        //////////////////////////////////////////////////////////////////////
        // internals
        //////////////////////////////////////////////////////////////////////
        value_type const* buf_;
        size_type size_;

#if 0
        BOOST_HANA_CONSTEXPR_LAMBDA string push_back(char c) const {
            constexpr std::size_t MAX_SIZE = 10000000;

            if (length(*this) >= MAX_SIZE)
                throw "can't handle a string that big";

            auto storage = [] {
                static char tmp[MAX_SIZE] = {'\0'};
                return tmp;
            };
            char* s = storage();

            for (std::size_t i = 0; i < length(*this); ++i)
                s[i] = (*this)[i];
            s[length(*this)] = c;
            return string{s, length(*this) + 1};
        }
#endif
    };

    using string = basic_string<char>;

    //////////////////////////////////////////////////////////////////////
    // comparison
    //////////////////////////////////////////////////////////////////////
    template <typename CharT, typename TraitsT, typename CharU, typename TraitsU>
    constexpr bool operator==(basic_string<CharT, TraitsT> const& a, basic_string<CharU, TraitsU> const& b) {
        return std_equal(a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::equal_to<>{});
    }

    template <typename CharT, typename TraitsT, typename CharU, typename TraitsU>
    constexpr bool operator!=(basic_string<CharT, TraitsT> const& a, basic_string<CharU, TraitsU> const& b)
    { return !(a == b); }

    template <typename CharT, typename TraitsT, typename CharU, typename TraitsU>
    constexpr bool operator<(basic_string<CharT, TraitsT> const& a, basic_string<CharU, TraitsU> const& b) {
        return std_lexicographical_compare(a.cbegin(), a.cend(), b.cbegin(), b.cend(), std::less<>{});
    }

    template <typename CharT, typename TraitsT, typename CharU, typename TraitsU>
    constexpr bool operator>(basic_string<CharT, TraitsT> const& a, basic_string<CharU, TraitsU> const& b)
    { return b < a; }

    template <typename CharT, typename TraitsT, typename CharU, typename TraitsU>
    constexpr bool operator<=(basic_string<CharT, TraitsT> const& a, basic_string<CharU, TraitsU> const& b)
    { return !(b < a); }

    template <typename CharT, typename TraitsT, typename CharU, typename TraitsU>
    constexpr bool operator>=(basic_string<CharT, TraitsT> const& a, basic_string<CharU, TraitsU> const& b)
    { return !(a < b); }

    //////////////////////////////////////////////////////////////////////////
    // input/output
    //////////////////////////////////////////////////////////////////////////
    template <typename Ostream, typename CharT, typename Traits>
    constexpr decltype(auto) operator<<(Ostream &os, basic_string<CharT, Traits> const& s) {
        using size_type = typename basic_string<CharT, Traits>::size_type;
        for (size_type i = 0; i < s.size(); ++i)
            os << s[i];
        return os;
    }

    namespace literals {
        constexpr basic_string<char> operator"" _s(char const* str, std::size_t size)
        { return {str, size}; }
    }
}}} // end namespace boost::hana::sandbox


//////////////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace boost::hana;
using namespace sandbox::literals;
using sandbox::string;


namespace test_capacity {
    static_assert(string{""}.size() == 0, "");
    static_assert(string{"a"}.size() == 1, "");
    static_assert(string{"abcd"}.size() == 4, "");
}

namespace test_access {
    constexpr string s{"abcdef"};
    static_assert(s[0] == 'a' && s[1] == 'b', "");
    // non-const operator[] will be preferred, but it is deleted
    // static_assert(string{"abcdef"}[0] == 'a', "");
}

namespace test_comparison {
    static_assert(string{"foo"} < string{"fool"}, "");
    static_assert(string{"abc"} < string{"abd"}, "");
    static_assert(string{"fool"} > string{"foo"}, "");
    static_assert(string{"abd"} > string{"abc"}, "");

    static_assert(string{""} == string{""}, "");
    static_assert(string{"x"} == string{"x"}, "");
    static_assert(string{"x"} != string{"y"}, "");
    static_assert(string{"x"} != string{"xx"}, "");
    static_assert(string{"xyz"} == string{"xyz"}, "");
}

namespace test_literals {
    static_assert("abcd"_s == string{"abcd"}, "");
}


int main() {
    constexpr string hello{"hello"}, world{"world"};
    static_assert(!hello.empty(), "");
    static_assert(hello.size() == 5, "");
    std::cout << hello << ' ' << world << '\n';
}
