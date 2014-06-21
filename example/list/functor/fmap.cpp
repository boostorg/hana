/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list.hpp>
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
        fmap(to_string, list(1, '2', "345", std::string{"67"})) ==
        list("1", "2", "345", "67")
    );
    //! [fusion]

    //! [mpl]
    BOOST_HANA_CONSTEXPR_LAMBDA auto types = list_t<void, int(), char[10]>;
    BOOST_HANA_CONSTEXPR_LAMBDA auto pointers = fmap(template_<std::add_pointer_t>, types);
    BOOST_HANA_STATIC_ASSERT(pointers == list_t<void*, int(*)(), char(*)[10]>);
    BOOST_HANA_STATIC_ASSERT(head(pointers) == type<void*>);
    //! [mpl]
}
