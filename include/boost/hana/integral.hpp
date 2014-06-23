/*!
@file
Defines `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_HPP
#define BOOST_HANA_INTEGRAL_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/integral_fwd.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<Integral, Integral> : Comparable::equal_mcd {
        template <typename T, typename U>
        static constexpr auto equal_impl(T t, U u)
        { return bool_<t() == u()>; }
    };

    template <>
    struct Logical::instance<Integral> : Logical::mcd {
        template <typename Cond, typename Then, typename Else>
        static constexpr auto eval_if_impl(Cond c, Then t, Else e)
        { return eval_if_impl(bool_<static_cast<bool>(c())>, t, e); }

        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(decltype(true_), Then t, Else)
        { return t([](auto x) { return x; }); }

        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(decltype(false_), Then, Else e)
        { return e([](auto x) { return x; }); }

        template <typename Cond>
        static constexpr auto not_impl(Cond c)
        { return bool_<!c()>; }
    };

    namespace integral_detail {
        constexpr int to_int(char c)
        { return static_cast<int>(c) - 48; }

        template <std::size_t N>
        constexpr long long parse(const char (&arr)[N]) {
            long long number = 0, base = 1;
            for (std::size_t i = 0; i < N; ++i) {
                number += to_int(arr[N - 1 - i]) * base;
                base *= 10;
            }
            return number;
        }
    }

    namespace literals {
        //! Creates an `Integral` from a literal.
        //! @relates boost::hana::Integral
        //!
        //! The literal is parsed at compile-time and the result is returned
        //! as an `llong<...>`.
        //!
        //! @note
        //! We use `llong<...>` instead of `ullong<...>` because using an
        //! unsigned type leads to unexpected behavior when doing stuff like
        //! `-1_c`. If we used an unsigned type, `-1_c` would be something
        //! like `ullong<-1>` which is actually `ullong<something huge>`.
        //!
        //! ### Example
        //! @snippet example/integral/literals.cpp main
        //!
        //! @todo Add support for stuff like `0x1234_c`.
        template <char ...c>
        constexpr auto operator"" _c()
        { return llong<integral_detail::parse<sizeof...(c)>({c...})>; }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_INTEGRAL_HPP
