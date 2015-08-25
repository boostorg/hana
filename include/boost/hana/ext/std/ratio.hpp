/*!
@file
Defines `boost::hana::ext::std::Ratio`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_RATIO_HPP
#define BOOST_HANA_EXT_STD_RATIO_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/fwd/core/convert.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/fwd/less.hpp>
#include <boost/hana/fwd/minus.hpp>
#include <boost/hana/fwd/mult.hpp>
#include <boost/hana/fwd/one.hpp>
#include <boost/hana/fwd/plus.hpp>
#include <boost/hana/fwd/quot.hpp>
#include <boost/hana/fwd/rem.hpp>
#include <boost/hana/fwd/zero.hpp>
#include <boost/hana/value.hpp>

#include <cstdint>
#include <ratio>
#include <type_traits>


namespace boost { namespace hana {
    namespace ext { namespace std { struct Ratio; }}

    template <std::intmax_t num, std::intmax_t den>
    struct datatype<std::ratio<num, den>> {
        using type = ext::std::Ratio;
    };

    //////////////////////////////////////////////////////////////////////////
    // Conversion from Constants
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct to_impl<ext::std::Ratio, C, when<
        _models<Constant, C>::value &&
        std::is_integral<typename C::value_type>::value
    >> {
        template <typename N>
        static constexpr auto apply(N const&) {
            constexpr auto v = hana::value<N>();
            return std::ratio<v>{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr auto apply(R1 const&, R2 const&)
        { return hana::bool_<std::ratio_equal<R1, R2>::value>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr auto apply(R1 const&, R2 const&)
        { return hana::bool_<std::ratio_less<R1, R2>::value>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monoid
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct plus_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr std::ratio_add<R1, R2> apply(R1 const&, R2 const&)
        { return {}; }
    };

    template <>
    struct zero_impl<ext::std::Ratio> {
        static constexpr std::ratio<0> apply()
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Group
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct minus_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr std::ratio_subtract<R1, R2> apply(R1 const&, R2 const&)
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Ring
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct mult_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr std::ratio_multiply<R1, R2> apply(R1 const&, R2 const&)
        { return {}; }
    };

    template <>
    struct one_impl<ext::std::Ratio> {
        static constexpr std::ratio<1> apply()
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct quot_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr std::ratio_divide<R1, R2> apply(R1 const&, R2 const&)
        { return {}; }
    };

    template <>
    struct rem_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr std::ratio<0> apply(R1 const&, R2 const&)
        { return {}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_RATIO_HPP
