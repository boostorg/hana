/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/tuple.hpp>

#include <boost/hana/core/datatype.hpp>

#include <boost/tuple/tuple.hpp>
#include <type_traits>
using namespace boost::hana;


int main() {
    auto make_cons = [](auto x, auto xs) {
        return boost::tuples::cons<decltype(x), decltype(xs)>{x, xs};
    };

    static_assert(std::is_same<
        datatype_t<decltype(boost::make_tuple())>,
        BoostTuple
    >::value, "");

    static_assert(std::is_same<
        datatype_t<decltype(boost::make_tuple(1))>,
        BoostTuple
    >::value, "");

    static_assert(std::is_same<
        datatype_t<decltype(boost::make_tuple(1, '2'))>,
        BoostTuple
    >::value, "");

    static_assert(std::is_same<
        datatype_t<decltype(boost::make_tuple(1, '2', 3.3))>,
        BoostTuple
    >::value, "");

    static_assert(std::is_same<
        datatype_t<decltype(make_cons(1, boost::tuples::null_type{}))>,
        BoostTuple
    >::value, "");

    static_assert(std::is_same<
        datatype_t<decltype(make_cons(1, make_cons('2', boost::tuples::null_type{})))>,
        BoostTuple
    >::value, "");

    static_assert(std::is_same<
        datatype_t<decltype(make_cons(1, boost::make_tuple('2', 3.3)))>,
        BoostTuple
    >::value, "");

    static_assert(std::is_same<
        datatype_t<boost::tuples::null_type>,
        BoostTuple
    >::value, "");
}
