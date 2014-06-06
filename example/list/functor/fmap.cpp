/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/trait.hpp>
#include <boost/hana/type.hpp>

#include <cassert>
#include <sstream>
#include <string>
using namespace boost::hana;


int main() {
    //! [fusion]
    BOOST_HANA_CONSTEXPR_LAMBDA auto to_string = [](auto x) {
        return (std::ostringstream{} << x).str();
    };

    assert(
        fmap(to_string, list(1, '2', "3", std::string{"456"})) ==
        list("1", "2", "3", "456")
    );
    //! [fusion]

    //! [mpl]
    BOOST_HANA_CONSTEXPR_LAMBDA auto types = list_t<void, int(), char[10]>;
    BOOST_HANA_STATIC_ASSERT(
        fmap(trait::add_pointer, types) == list_t<void*, int(*)(), char(*)[10]>
    );
    BOOST_HANA_STATIC_ASSERT(
        head(fmap(trait::add_pointer, types)) == type<void*>
    );
    //! [mpl]
}
