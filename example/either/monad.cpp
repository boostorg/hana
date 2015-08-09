/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/eval_if.hpp>
#include <boost/hana/integral_constant.hpp>

#include <string>
namespace hana = boost::hana;
using namespace std::literals;


int main() {
    auto safe_div = [](auto x, auto y) {
        return hana::eval_if(y == hana::int_<0>,
            [] { return hana::left("division by zero"s); },
            [=](auto _) { return hana::right(x / _(y)); }
        );
    };

    auto safe_dec = [](auto x) {
        return hana::eval_if(x == hana::int_<0>,
            [] { return hana::left("negative value"s); },
            [=](auto _) { return hana::right(_(x) - hana::int_<1>); }
        );
    };

    BOOST_HANA_RUNTIME_CHECK(
        (safe_div(hana::int_<4>, hana::int_<0>) | safe_dec) == hana::left("division by zero"s)
    );

    BOOST_HANA_RUNTIME_CHECK(
        (safe_div(hana::int_<0>, hana::int_<2>) | safe_dec) == hana::left("negative value"s)
    );

    BOOST_HANA_CONSTANT_CHECK(
        (safe_div(hana::int_<4>, hana::int_<2>) | safe_dec) == hana::right(hana::int_<1>)
    );
}
