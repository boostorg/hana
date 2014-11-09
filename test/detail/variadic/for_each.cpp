/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/for_each.hpp>

#include <boost/hana/assert.hpp>

#include <test/injection.hpp>

#include <vector>
using namespace boost::hana;
namespace vd = detail::variadic;


int main() {
    // Make sure the function is applied in left-to-right order.
    {
        auto check_with = [=](auto ...xs) {
            std::vector<int> seen{};
            vd::for_each([&](int x) { seen.push_back(x); }, xs...);
            BOOST_HANA_RUNTIME_CHECK(seen == std::vector<int>{xs...});
        };
        check_with();
        check_with(0);
        check_with(0, 1);
        check_with(0, 1, 2);
        check_with(0, 1, 2, 3);
        check_with(0, 1, 2, 3, 4);
    }

    // Make sure it works with heterogeneous sequences.
    {
        using test::x;
        vd::for_each([](auto) { }, x<0>);
        vd::for_each([](auto) { }, x<0>, x<1>);
        vd::for_each([](auto) { }, x<0>, x<1>, x<2>);
        vd::for_each([](auto) { }, x<0>, x<1>, x<2>, x<3>);
    }
}
