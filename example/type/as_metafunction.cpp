/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#include <type_traits>
using namespace boost::hana;


//! [main]
template <typename ...T>
using largest = decltype(maximum_by(less /on/ sizeof_, type_list<T...>));

template <int size>
struct storage { char s[size]; };

static_assert(std::is_same<
    largest<storage<1>, storage<2>, storage<3>>::type,
    storage<3>
>{}, "");
//! [main]

int main() { }
