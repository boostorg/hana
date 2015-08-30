/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/extract.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/succ.hpp>
namespace hana = boost::hana;


static_assert(hana::extract(hana::make_lazy(1)) == 1, "");
static_assert(hana::extract(hana::make_lazy(hana::succ)(3)) == 4, "");

int main() { }
