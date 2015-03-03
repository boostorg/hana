/*!
@file
Defines `boost::hana::ext::std::Ratio`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_RATIO_HPP
#define BOOST_HANA_EXT_STD_RATIO_HPP

#include <boost/hana/fwd/ext/std/ratio.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_integral.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/ring.hpp>

#include <ratio>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Conversion from Constants
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct to_impl<ext::std::Ratio, C, when<
        models<Constant(C)>{} &&
        detail::std::is_integral<typename C::value_type>{}
    >> {
        template <typename N>
        static constexpr auto apply(N n) {
            constexpr auto v = hana::value(n);
            return ::std::ratio<v>{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr auto apply(R1, R2)
        { return bool_< ::std::ratio_equal<R1, R2>::value>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct less_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr auto apply(R1, R2)
        { return bool_< ::std::ratio_less<R1, R2>::value>; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Monoid
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct plus_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr ::std::ratio_add<R1, R2> apply(R1, R2)
        { return {}; }
    };

    template <>
    struct zero_impl<ext::std::Ratio> {
        static constexpr ::std::ratio<0> apply()
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Group
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct minus_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr ::std::ratio_subtract<R1, R2> apply(R1, R2)
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // Ring
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct mult_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr ::std::ratio_multiply<R1, R2> apply(R1, R2)
        { return {}; }
    };

    template <>
    struct one_impl<ext::std::Ratio> {
        static constexpr ::std::ratio<1> apply()
        { return {}; }
    };

    //////////////////////////////////////////////////////////////////////////
    // IntegralDomain
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct quot_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr ::std::ratio_divide<R1, R2> apply(R1, R2)
        { return {}; }
    };

    template <>
    struct mod_impl<ext::std::Ratio, ext::std::Ratio> {
        template <typename R1, typename R2>
        static constexpr ::std::ratio<0> apply(R1, R2)
        { return {}; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_RATIO_HPP
