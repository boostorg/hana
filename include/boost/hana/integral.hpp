/*!
@file
Defines `boost::hana::Integral`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_INTEGRAL_HPP
#define BOOST_HANA_INTEGRAL_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/detail/integral_fwd.hpp>
#include <boost/hana/logical/mcd.hpp>
#include <boost/hana/orderable/less_mcd.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //! @details
    //! Two `Integral`s are equal if and only if the integral values they
    //! represent are equal. Conversions are allowed.
    //!
    //! @snippet example/integral/comparable.cpp main
    template <>
    struct Comparable::instance<Integral, Integral> : Comparable::equal_mcd {
        template <typename T, typename U>
        static constexpr auto equal_impl(T t, U u)
        { return bool_<t() == u()>; }
    };

    //! @details
    //! `Integral`s can be compared with objects of any integral type; the
    //! comparison is done by comparing their underlying integral value.
    template <typename T>
    struct Comparable::instance<Integral, T, when<std::is_integral<T>{}>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return x() == y; }
    };

    template <typename T>
    struct Comparable::instance<T, Integral, when<std::is_integral<T>{}>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return x == y(); }
    };

    //! @details
    //! An `Integral` is less than another if and only if its underlying
    //! integral value is less than the other's.
    //!
    //! @snippet example/integral/orderable.cpp main
    template <>
    struct Orderable::instance<Integral, Integral> : Orderable::less_mcd {
        template <typename T, typename U>
        static constexpr auto less_impl(T t, U u)
        { return bool_<(t() < u())>; }
    };

    //! @details
    //! `Integral`s can be ordered with objects of any integral type; the
    //! ordering is done by ordering their underlying integral value.
    template <typename T>
    struct Orderable::instance<Integral, T, when<std::is_integral<T>{}>>
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr auto less_impl(X x, Y y)
        { return x() < y; }
    };

    template <typename T>
    struct Orderable::instance<T, Integral, when<std::is_integral<T>{}>>
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr auto less_impl(X x, Y y)
        { return x < y(); }
    };

    //! @details
    //! An `Integral` is true-valued if and only if its integral value is
    //! true-valued, i.e. if
    //! @code
    //!     static_cast<bool>(n()) == true
    //! @endcode
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

        template <decltype(sizeof(int)) N>
        constexpr long long parse(const char (&arr)[N]) {
            long long number = 0, base = 1;
            for (decltype(N) i = 0; i < N; ++i) {
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
