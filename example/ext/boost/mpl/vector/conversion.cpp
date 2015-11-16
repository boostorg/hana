/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/convert.hpp>
#include <boost/hana/ext/boost/mpl/vector.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/vector.hpp>
#include <type_traits>
namespace hana = boost::hana;
namespace mpl = boost::mpl;


auto xs = hana::make_tuple(1, '2', 3.0);
static_assert(std::is_same<
    decltype(hana::to<hana::ext::boost::mpl::vector_tag>(xs)),
    mpl::vector<int, char, double>
>{}, "");

auto ys = hana::make_tuple(1, '2', hana::type_c<void>);
static_assert(std::is_same<
    decltype(hana::to<hana::ext::boost::mpl::vector_tag>(ys)),
    mpl::vector<int, char, void>
>{}, "");

int main() { }
