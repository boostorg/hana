/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/type.hpp>

#include <string>
namespace hana = boost::hana;


int main() {
    static_assert(hana::from_just(hana::just('x')) == 'x', "");
    BOOST_HANA_CONSTANT_CHECK(*hana::just(hana::type_c<int>) == hana::type_c<int>);
    BOOST_HANA_RUNTIME_CHECK(hana::just(std::string{"abcd"})->size() == 4);

    // hana::from_just(hana::nothing); // compile-time error
}
