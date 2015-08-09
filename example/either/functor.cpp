/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/succ.hpp>
#include <boost/hana/transform.hpp>

#include <string>
namespace hana = boost::hana;
using namespace std::literals;


int main() {
    auto safe_div = hana::infix([](auto x, auto y) {
        return hana::eval_if(y == hana::int_<0>,
            [] { return hana::left("division by zero"s); },
            [=](auto _) { return hana::right(x / _(y)); }
        );
    });

    BOOST_HANA_CONSTANT_CHECK(
        hana::transform(hana::int_<6> ^safe_div^ hana::int_<3>, hana::succ) == hana::right(hana::int_<3>)
    );

    BOOST_HANA_RUNTIME_CHECK(
        hana::transform(hana::int_<6> ^safe_div^ hana::int_<0>, hana::succ) == hana::left("division by zero"s)
    );
}
