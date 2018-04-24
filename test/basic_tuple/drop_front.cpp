// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/drop_front.hpp>
#include <boost/hana/integral_constant.hpp>
namespace hana = boost::hana;


template <int i>
struct x { };

int main() {
    // Make sure drop_front works with nested tuples
    hana::basic_tuple<x<0>, hana::basic_tuple<x<1>, x<2>>> tuple;
    auto all = hana::drop_front(tuple, hana::size_c<0>); (void)all;
}
