/*!
@file
Defines the instance of `boost::hana::Foldable` for `boost::hana::String`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRING_FOLDABLE_HPP
#define BOOST_HANA_STRING_FOLDABLE_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/foldable/unpack_mcd.hpp>
#include <boost/hana/integral.hpp> // for char_, size_t
#include <boost/hana/string/string.hpp>


namespace boost { namespace hana {
    //! Instance of `Foldable` for `String`s.
    //!
    //! For the purpose of being folded, `String`s are basically equivalent to
    //! a tuple in which `IntegralConstant`s of underlying type `char` are
    //! stored.
    //!
    //! ### Example
    //! @snippet example/string/foldable.cpp main
    template <>
    struct Foldable::instance<String> : Foldable::unpack_mcd {
    private:
        static constexpr detail::std::size_t strlen(char const* s) {
            detail::std::size_t l = 0;
            while (*s++ != '\0')
                ++l;
            return l;
        }

        template <typename S, typename F, detail::std::size_t ...i>
        static constexpr decltype(auto) unpack_helper(S, F&& f,
                                            detail::std::index_sequence<i...>)
        {
            constexpr char const* s = S::get();
            return detail::std::forward<F>(f)(char_<s[i]>...);
        }

    public:
        template <typename S, typename F>
        static constexpr decltype(auto) unpack_impl(S s, F&& f) {
            return unpack_helper(s, detail::std::forward<F>(f),
                detail::std::make_index_sequence<strlen(S::get())>{}
            );
        }

        template <typename S>
        static constexpr auto length_impl(S) {
            return size_t<strlen(S::get())>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRING_FOLDABLE_HPP
