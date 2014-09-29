/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/n_times.hpp>

#include <boost/hana/detail/assert.hpp>
using namespace boost::hana;


template <unsigned long n>
void check() {
    unsigned long counter = 0;
    detail::n_times<n>([&] {
        ++counter;
    });
    BOOST_HANA_RUNTIME_ASSERT(counter == n);
}

int main() {
    check<0>();
    check<1>();
    check<2>();
    check<3>();
    check<4>();
    check<5>();
    check<6>();
}
