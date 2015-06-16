/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/closure.hpp>

#include <string>
using namespace boost::hana;


struct Empty { };

int main() {
    {
        using T = detail::closure<int>;
        const T t(3);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 3);
    }
    {
        using T = detail::closure<std::string, int>;
        const T t("high", 5);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == "high");
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 5);
    }
    {
        using T = detail::closure<double, int>;
        constexpr T t(2.718, 5);
        static_assert(detail::get<0>(t) == 2.718, "");
        static_assert(detail::get<1>(t) == 5, "");
    }
    {
        using T = detail::closure<Empty>;
        constexpr T t{Empty()};
        constexpr Empty e = detail::get<0>(t); (void)e;
    }
    {
        using T = detail::closure<double&, std::string, int>;
        double d = 1.5;
        const T t(d, "high", 5);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 1.5);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == "high");
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == 5);
        detail::get<0>(t) = 2.5;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2.5);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == "high");
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == 5);
        BOOST_HANA_RUNTIME_CHECK(d == 2.5);
    }
}
