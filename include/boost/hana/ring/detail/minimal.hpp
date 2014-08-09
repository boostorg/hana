/*!
@file
Defines `boost::hana::MinimalRing`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RING_DETAIL_MINIMAL_HPP
#define BOOST_HANA_RING_DETAIL_MINIMAL_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/group/negate_mcd.hpp>
#include <boost/hana/monoid/mcd.hpp>
#include <boost/hana/ring/mcd.hpp>


namespace boost { namespace hana {
    template <typename Mcd = Ring::mcd>
    struct MinimalRing;

    namespace ring_detail {
        template <typename Mcd, typename X>
        struct minimal_ring_impl {
            X value;
            using hana_datatype = MinimalRing<Mcd>;
        };

        template <typename Mcd>
        struct make_minimal_ring {
            template <typename X>
            constexpr auto operator()(X x) const
            { return minimal_ring_impl<Mcd, X>{x}; }
        };
    }

    template <typename Mcd = Ring::mcd>
    static constexpr ring_detail::make_minimal_ring<Mcd> minimal_ring{};

    // Provided for convenience in the unit tests.
    template <typename Mcd>
    struct Comparable::instance<MinimalRing<Mcd>, MinimalRing<Mcd>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return x.value == y.value; }
    };

    template <typename Mcd>
    struct Monoid::instance<MinimalRing<Mcd>, MinimalRing<Mcd>>
        : Monoid::mcd
    {
        template <typename X, typename Y>
        static constexpr auto plus_impl(X x, Y y)
        { return minimal_ring<Mcd>(x.value + y.value); }

        static constexpr auto zero_impl()
        { return minimal_ring<Mcd>(0); }
    };

    template <typename Mcd>
    struct Group::instance<MinimalRing<Mcd>, MinimalRing<Mcd>>
        : Group::negate_mcd<MinimalRing<Mcd>, MinimalRing<Mcd>>
    {
        template <typename X>
        static constexpr auto negate_impl(X x)
        { return minimal_ring<Mcd>(-x.value); }
    };

    template <>
    struct Ring::instance<MinimalRing<Ring::mcd>, MinimalRing<Ring::mcd>>
        : Ring::mcd
    {
        static constexpr auto one_impl()
        { return minimal_ring<Ring::mcd>(1); }

        template <typename X, typename Y>
        static constexpr auto mult_impl(X x, Y y)
        { return minimal_ring<Ring::mcd>(x.value * y.value); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RING_DETAIL_MINIMAL_HPP
