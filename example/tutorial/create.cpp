/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <string>
using namespace boost::hana;


int main() {

//! [make<Tuple>]
auto xs = make<Tuple>(1, 2.2, 'a', std::string{"bcde"});
//! [make<Tuple>]

//! [make<Range>]
constexpr auto r = make<Range>(int_<3>, int_<10>);
static_assert(r == make_range(int_<3>, int_<10>), "");
//! [make<Range>]

}
