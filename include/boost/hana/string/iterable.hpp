/*!
@file
Defines the instance of `boost::hana::Iterable` for `boost::hana::String`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_ITERABLE_HPP
#define BOOST_HANA_STRING_ITERABLE_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/constant/constant.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/integral.hpp> // for char_
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/string/string.hpp>

#include <boost/hana/core/operators.hpp>
#include <boost/hana/iterable/operators.hpp>


namespace boost { namespace hana {
    //! Instance of `Iterable` for `String`s.
    //!
    //! For the purpose of being iterated over, `String`s are basically
    //! equivalent to a tuple of `IntegralConstant`s with an underlying
    //! type of `char`.
    //!
    //! ### Example
    //! @snippet example/string/iterable.cpp main
    //!
    //! @todo Don't duplicate `strlen`. Create generic constexpr algorithms
    //! in detail/ and use them.
    template <>
    struct Iterable::instance<String> : Iterable::mcd {
    private:
        static constexpr detail::std::size_t strlen(char const* s) {
            detail::std::size_t l = 0;
            while (*s++ != '\0')
                ++l;
            return l;
        }

    public:
        template <typename S>
        static constexpr auto head_impl(S) {
            return char_<S::get()[0]>;
        }

        template <typename S, detail::std::size_t ...i>
        static constexpr auto tail_helper(S, detail::std::index_sequence<i...>) {
            return string<S::get()[i+1]...>;
        }

        template <typename S>
        static constexpr auto tail_impl(S s) {
            return tail_helper(s,
                detail::std::make_index_sequence<strlen(S::get()) - 1>{});
        }

        template <typename S>
        static constexpr auto is_empty_impl(S) {
            return bool_<S::get()[0] == '\0'>;
        }

        template <typename I, typename S>
        static constexpr auto at_impl(I index, S) {
            constexpr auto i = value(index);
            return char_<S::get()[i]>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_ITERABLE_HPP
