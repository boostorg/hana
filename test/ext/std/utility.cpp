/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/utility.hpp>

#include <boost/hana/type.hpp>
namespace hana = boost::hana;


struct T { };

using Z = decltype(hana::traits::declval(hana::type_c<T>));

int main() { }
