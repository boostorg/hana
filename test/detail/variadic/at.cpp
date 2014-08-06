/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/at.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/minimal/comparable.hpp>
using namespace boost::hana;
namespace vd = detail::variadic;


struct non_pod { virtual ~non_pod() { } };

template <int i> struct invalid { };
template <int i> constexpr invalid<i> y{};

template <int i>
constexpr auto x = detail::minimal::comparable<>(i);

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<0>(x<0>) == x<0>);

    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<0>(x<0>, x<1>) == x<0>);
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<1>(y<0>, x<1>) == x<1>);

    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<0>(x<0>, y<1>, y<2>) == x<0>);
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<1>(y<0>, x<1>, y<2>) == x<1>);
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<2>(y<0>, y<1>, x<2>) == x<2>);

    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<0>(x<0>, y<1>, y<2>, y<3>) == x<0>);
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<1>(y<0>, x<1>, y<2>, y<3>) == x<1>);
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<2>(y<0>, y<1>, x<2>, y<3>) == x<2>);
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<3>(y<0>, y<1>, y<2>, x<3>) == x<3>);

    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<0>(x<0>, y<1>, y<2>, y<3>, y<4>) == x<0>);
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<1>(y<0>, x<1>, y<2>, y<3>, y<4>) == x<1>);
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<2>(y<0>, y<1>, x<2>, y<3>, y<4>) == x<2>);
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<3>(y<0>, y<1>, y<2>, x<3>, y<4>) == x<3>);
    BOOST_HANA_CONSTEXPR_ASSERT(vd::at<4>(y<0>, y<1>, y<2>, y<3>, x<4>) == x<4>);

    // make sure we can use non-pods on both side of the fetched object
    BOOST_HANA_RUNTIME_ASSERT(vd::at<0>(x<0>, non_pod{}) == x<0>);
    BOOST_HANA_RUNTIME_ASSERT(vd::at<1>(non_pod{}, x<1>) == x<1>);
}
