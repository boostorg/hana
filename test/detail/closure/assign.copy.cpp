/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/closure.hpp>

#include <string>
using namespace boost::hana;


int main() {
    {
        using T = detail::closure<>;
        T t0;
        T t;
        t = t0;
    }
    {
        using T = detail::closure<int>;
        T t0(2);
        T t;
        t = t0;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2);
    }
    {
        using T = detail::closure<int, char>;
        T t0(2, 'a');
        T t;
        t = t0;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 'a');
    }
    {
        using T = detail::closure<int, char, std::string>;
        const T t0(2, 'a', "some text");
        T t;
        t = t0;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 'a');
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == "some text");
    }
}
