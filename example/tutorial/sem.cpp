/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <sstream>
#include <string>
using namespace boost::hana;


int main() {

{

//! [copy_initialize]
std::string hello{"Hello"};
std::string world{"world"};
// copies hello and world
_tuple<std::string, std::string> xs{hello, world};
//! [copy_initialize]

}{

//! [move_initialize]
std::string hello{"Hello"};
std::string world{"world"};
// moves hello and world
_tuple<std::string, std::string> xs{std::move(hello), std::move(world)};
//! [move_initialize]

}

//! [reverse_transform]
auto to_str = [](auto const& x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
};

auto xs = make_tuple(1, 2.2, 'a', "bcde");
BOOST_HANA_RUNTIME_CHECK(
    reverse(transform(xs, to_str)) == make_tuple("bcde", "a", "2.2", "1")
);
//! [reverse_transform]

//! [reverse_transform_copy]
reverse(
    transform(xs, to_str) // <-- copy into reverse(...) here?
);
//! [reverse_transform_copy]

//! [reverse_transform_move]
reverse(
    transform(xs, to_str) // <-- nah, move from the temporary!
);
//! [reverse_transform_move]

}
