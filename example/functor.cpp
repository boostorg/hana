/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <sstream>
#include <string>
#include <type_traits>
using namespace boost::hana;
using namespace std::literals;


int main() {

{

//! [adjust_if]
BOOST_HANA_CONSTEXPR_LAMBDA auto negative = [](auto x) {
    return x < 0;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto negate = [](auto x) {
    return -x;
};

BOOST_HANA_CONSTEXPR_CHECK(
    adjust_if(make<Tuple>(-3, -2, -1, 0, 1, 2, 3), negative, negate)
    ==
    make<Tuple>(3, 2, 1, 0, 1, 2, 3)
);
//! [adjust_if]

}{

//! [adjust]
BOOST_HANA_CONSTEXPR_LAMBDA auto negate = [](auto x) {
    return -x;
};

BOOST_HANA_CONSTEXPR_CHECK(
    adjust(make<Tuple>(1, 4, 9, 2, 3, 4), 4, negate)
    ==
    make<Tuple>(1, -4, 9, 2, 3, -4)
);
//! [adjust]

}{

//! [fill]
BOOST_HANA_CONSTEXPR_CHECK(
    fill(make<Tuple>(1, '2', 3.3, nullptr), 'x') == make<Tuple>('x', 'x', 'x', 'x')
);

BOOST_HANA_CONSTANT_CHECK(fill(nothing, 'x') == nothing);
BOOST_HANA_CONSTEXPR_CHECK(fill(just('y'), 'x') == just('x'));
//! [fill]

}{

//! [transform]
auto to_string = [](auto x) {
    return static_cast<std::ostringstream const&>(std::ostringstream{} << x).str();
};

BOOST_HANA_RUNTIME_CHECK(
    transform(make<Tuple>(1, '2', "345", std::string{"67"}), to_string)
    ==
    make<Tuple>("1", "2", "345", "67")
);

BOOST_HANA_CONSTANT_CHECK(transform(nothing, to_string) == nothing);
BOOST_HANA_RUNTIME_CHECK(transform(just(123), to_string) == just("123"s));

BOOST_HANA_CONSTANT_CHECK(
    transform(tuple_t<void, int(), char[10]>, template_<std::add_pointer_t>)
            ==
    tuple_t<void*, int(*)(), char(*)[10]>
);
//! [transform]

}{

//! [replace_if]
BOOST_HANA_CONSTEXPR_LAMBDA auto negative = [](auto x) {
    return x < 0;
};

BOOST_HANA_CONSTEXPR_CHECK(
    replace_if(make<Tuple>(-3, -2, -1, 0, 1, 2, 3), negative, 0)
    ==
    make<Tuple>(0, 0, 0, 0, 1, 2, 3)
);
//! [replace_if]

}{

//! [replace]
BOOST_HANA_CONSTEXPR_CHECK(
    replace(make<Tuple>(1, 1, 1, 2, 3, 1, 4, 5), 1, 0)
    ==
    make<Tuple>(0, 0, 0, 2, 3, 0, 4, 5)
);
//! [replace]

}

}
