/*!
@file
Defines `boost::hana::Enumerable::integral_constant_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ENUMERABLE_INTEGRAL_CONSTANT_MCD_HPP
#define BOOST_HANA_ENUMERABLE_INTEGRAL_CONSTANT_MCD_HPP

#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/enumerable/mcd.hpp>
#include <boost/hana/integral_constant/integral_constant.hpp>


namespace boost { namespace hana {
    template <typename I>
    struct Enumerable::integral_constant_mcd : Enumerable::mcd {
    private:
        template <typename X> static constexpr auto inc(X x) { return ++x; }
        template <typename X> static constexpr auto dec(X x) { return --x; }

    public:
        template <typename X>
        static constexpr decltype(auto) succ_impl(X x) {
            constexpr auto n = value(x);
            return integral_constant<I, decltype(inc(n)), inc(n)>;
        }

        template <typename X>
        static constexpr decltype(auto) pred_impl(X x) {
            constexpr auto n = value(x);
            return integral_constant<I, decltype(dec(n)), dec(n)>;
        }
    };

    //! Instance of `Enumerable` for `IntegralConstant`s.
    //!
    //! This instance of `Enumerable` reflects the usual definition of
    //! `succ` and `pred` for natural numbers. Specifically, `succ(n)`
    //! is an `IntegralConstant` holding `++value(n)`, and `pred(n)` is
    //! an `IntegralConstant` holding `--value(n)`.
    //!
    //! ### Example
    //! @snippet example/integral_constant.cpp enumerable
    template <typename I>
    struct Enumerable::instance<I, when<is_an<IntegralConstant, I>()>>
        : Enumerable::integral_constant_mcd<I>
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ENUMERABLE_INTEGRAL_CONSTANT_MCD_HPP
