/*!
@file
Defines `boost::hana::Logical::integral_constant_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_INTEGRAL_CONSTANT_MCD_HPP
#define BOOST_HANA_LOGICAL_INTEGRAL_CONSTANT_MCD_HPP

#include <boost/hana/constant/constant.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/integral_constant/integral_constant.hpp>
#include <boost/hana/logical/mcd.hpp>


namespace boost { namespace hana {
    //! Instance of `Logical` for `IntegralConstant`s.
    //!
    //! An `IntegralConstant` is true-valued if and only if its integral
    //! value is true-valued, i.e. if
    //! @code
    //!     static_cast<bool>(value(n)) == true
    //! @endcode
    template <typename I>
    struct Logical::integral_constant_mcd : Logical::mcd {
        template <bool> struct tval { };

        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(tval<true>, Then t, Else e)
        { return t([](auto x) { return x; }); }

        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(tval<false>, Then t, Else e)
        { return e([](auto x) { return x; }); }

        template <typename Cond, typename Then, typename Else>
        static constexpr auto eval_if_impl(Cond c, Then t, Else e)
        { return eval_if_impl(tval<static_cast<bool>(value(c))>{}, t, e); }

        template <typename Cond>
        static constexpr auto not_impl(Cond c) {
            constexpr auto nc = !value(c);
            return integral_constant<I, decltype(nc), nc>;
        }
    };

    template <typename I>
    struct Logical::instance<I, when<is_an<IntegralConstant, I>()>>
        : Logical::integral_constant_mcd<I>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_INTEGRAL_CONSTANT_MCD_HPP
