/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/orderable/less_mcd.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/minimal/orderable.hpp>
using namespace boost::hana;


template <typename mcd>
void test() {
    constexpr auto ord = detail::minimal::orderable<mcd>;

    BOOST_HANA_CONSTEXPR_LAMBDA auto ord_ = [=](auto i) {
        return [=] { return ord(i); };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto less_ = ordering([](auto x) {
        return x();
    });

    BOOST_HANA_CONSTEXPR_ASSERT(less_(ord_(0), ord_(1)) == less(ord(0), ord(1)));
    BOOST_HANA_CONSTEXPR_ASSERT(less_(ord_(0), ord_(0)) == less(ord(0), ord(0)));
    BOOST_HANA_CONSTEXPR_ASSERT(less_(ord_(1), ord_(0)) == less(ord(1), ord(0)));
}

int main() {
    test<Orderable::less_mcd>();
}
