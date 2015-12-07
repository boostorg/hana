/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/experimental/printable.hpp>
#include <boost/hana/type.hpp>

#include <sstream>
namespace hana = boost::hana;


template <typename ...T>
struct Template { };

int main() {
    {
        std::ostringstream ss;
        ss << hana::experimental::print(
            hana::type_c<void>
        );
        BOOST_HANA_RUNTIME_CHECK(ss.str() == "type<void>");
    }

    {
        std::ostringstream ss;
        ss << hana::experimental::print(
            hana::type_c<int>
        );
        BOOST_HANA_RUNTIME_CHECK(ss.str() == "type<int>");
    }

    {
        std::ostringstream ss;
        ss << hana::experimental::print(
            hana::type_c<int const>
        );
        BOOST_HANA_RUNTIME_CHECK(ss.str() == "type<int const>");
    }

    {
        std::ostringstream ss;
        ss << hana::experimental::print(
            hana::type_c<Template<void, char const*>>
        );
        BOOST_HANA_RUNTIME_CHECK(ss.str() == "type<Template<void, char const*>>");
    }
}
