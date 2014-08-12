/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#include <sstream>
#include <string>
#include <type_traits>
using namespace boost::hana;


int main() {
    //! [to_string]
    auto to_string = [](auto x) {
        return (std::ostringstream{} << x).str();
    };

    BOOST_HANA_RUNTIME_ASSERT(
        fmap(to_string, list(1, '2', "345", std::string{"67"}))
        ==
        list("1", "2", "345", "67")
    );

    BOOST_HANA_CONSTANT_ASSERT(fmap(to_string, nothing) == nothing);
    BOOST_HANA_RUNTIME_ASSERT(fmap(to_string, just(123)) == just("123"));
    //! [to_string]

    //! [add_pointer]
    BOOST_HANA_CONSTANT_ASSERT(
        fmap(template_<std::add_pointer_t>, type_list<void, int(), char[10]>)
                ==
        type_list<void*, int(*)(), char(*)[10]>
    );
    //! [add_pointer]
}
