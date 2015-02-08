/*!
@file
Defines `boost::hana::ext::std::Ratio`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_RATIO_HPP
#define BOOST_HANA_EXT_STD_RATIO_HPP

#include <boost/hana/fwd/ext/std/ratio.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/integral_constant.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/ring.hpp>

#include <ratio>


namespace boost { namespace hana {
    template <>
    struct Comparable::instance<ext::std::Ratio, ext::std::Ratio>
        : Comparable::equal_mcd
    {
        template <typename R1, typename R2>
        static constexpr decltype(auto) equal_impl(R1, R2) {
            return bool_<::std::ratio_equal<R1, R2>::value>;
        }
    };

    template <typename I>
    struct to_impl<ext::std::Ratio, I, when<is_an<IntegralConstant, I>()>> {
        template <typename N>
        static constexpr auto apply(N n) {
            constexpr auto v = value(n);
            return ::std::ratio<v>{};
        }
    };

    template <>
    struct Group::instance<ext::std::Ratio, ext::std::Ratio>
        : Group::minus_mcd<ext::std::Ratio, ext::std::Ratio>
    {
        template <typename R1, typename R2>
        static constexpr decltype(auto) minus_impl(R1, R2) {
            return ::std::ratio_subtract<R1, R2>{};
        }
    };

    template <>
    struct IntegralDomain::instance<ext::std::Ratio, ext::std::Ratio>
        : IntegralDomain::mcd
    {
        template <typename R1, typename R2>
        static constexpr decltype(auto) quot_impl(R1, R2) {
            return ::std::ratio_divide<R1, R2>{};
        }

        template <typename R1, typename R2>
        static constexpr decltype(auto) mod_impl(R1, R2) {
            return ::std::ratio<0>{};
        }
    };

    template <>
    struct Monoid::instance<ext::std::Ratio, ext::std::Ratio> : Monoid::mcd {
        template <typename R1, typename R2>
        static constexpr decltype(auto) plus_impl(R1, R2) {
            return ::std::ratio_add<R1, R2>{};
        }

        static constexpr decltype(auto) zero_impl() {
            return ::std::ratio<0>{};
        }
    };

    template <>
    struct Orderable::instance<ext::std::Ratio, ext::std::Ratio>
        : Orderable::less_mcd
    {
        template <typename R1, typename R2>
        static constexpr decltype(auto) less_impl(R1, R2) {
            return bool_<::std::ratio_less<R1, R2>::value>;
        }
    };

    template <>
    struct Ring::instance<ext::std::Ratio, ext::std::Ratio> : Ring::mcd {
        template <typename R1, typename R2>
        static constexpr decltype(auto) mult_impl(R1, R2) {
            return ::std::ratio_multiply<R1, R2>{};
        }

        static constexpr decltype(auto) one_impl() {
            return ::std::ratio<1>{};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_RATIO_HPP
