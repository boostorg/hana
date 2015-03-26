/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/for_each.hpp>
#include <boost/hana/assert.hpp>

#include <laws/base.hpp>

#include <vector>
using namespace boost::hana;
namespace vd = detail::variadic;
using test::ct_eq;


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
        vd::for_each([](auto) { }, ct_eq<0>{});
        vd::for_each([](auto) { }, ct_eq<0>{}, ct_eq<1>{});
        vd::for_each([](auto) { }, ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{});
        vd::for_each([](auto) { }, ct_eq<0>{}, ct_eq<1>{}, ct_eq<2>{}, ct_eq<3>{});
    }
}
