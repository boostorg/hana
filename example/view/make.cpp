// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/at.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/view.hpp>

#include <string>
namespace hana = boost::hana;


constexpr auto tuple = hana::make_tuple(1, 2.2, '3');
constexpr auto view = hana::make_view(tuple);

// Until an algorithm is applied to the view, it is basically a reference
// to the original object, unchanged.
static_assert(hana::at_c<0>(view) == 1, "");
static_assert(hana::at_c<1>(view) == 2.2, "");
static_assert(hana::at_c<2>(view) == '3', "");

int main() { }
