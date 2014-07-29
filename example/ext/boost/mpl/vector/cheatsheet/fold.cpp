/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/boost/mpl/vector.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/type_list.hpp>

#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/vector.hpp>

#include <type_traits>
namespace hana = boost::hana;
namespace mpl = boost::mpl;


namespace with_mpl {
//! [mpl]
using types = mpl::vector<long, float, short, float, long, long double>;
using number_of_floats = mpl::fold<
    types,
    mpl::int_<0>,
    mpl::if_<std::is_floating_point<mpl::_2>,
        mpl::next<mpl::_1>,
        mpl::_1
    >
>::type;
static_assert(number_of_floats::value == 3, "");
//! [mpl]
}

namespace with_hana {
//! [hana]
constexpr auto types = hana::type_list<long, float, short, float, long, long double>;
BOOST_HANA_CONSTEXPR_LAMBDA auto number_of_floats = hana::foldl(
    [](auto count, auto t) {
        return hana::if_(hana::trait<std::is_floating_point>(t),
            count + hana::int_<1>,
            count
        );
    },
    hana::int_<0>,
    types
);
static_assert(decltype(number_of_floats == hana::int_<3>){}, "");
//! [hana]
}

int main() { }
