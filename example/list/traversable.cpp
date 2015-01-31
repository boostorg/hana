/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
using namespace boost::hana;


int main() {
    //! [main]
    using namespace std::string_literals;

    BOOST_HANA_RUNTIME_CHECK(
        sequence<Tuple>(
            tuple(tuple("a1"s, "a2"s), tuple("b1"s), tuple("c1"s, "c2"s, "c3"s))
        )
        ==
        tuple(
            tuple("a1"s, "b1"s, "c1"s),
            tuple("a1"s, "b1"s, "c2"s),
            tuple("a1"s, "b1"s, "c3"s),

            tuple("a2"s, "b1"s, "c1"s),
            tuple("a2"s, "b1"s, "c2"s),
            tuple("a2"s, "b1"s, "c3"s)
        )
    );

    BOOST_HANA_CONSTEXPR_LAMBDA auto half = [](auto x) {
        return if_(x % int_<2> == int_<0>,
            just(x / int_<2>),
            nothing
        );
    };

    BOOST_HANA_CONSTANT_CHECK(
        traverse<Maybe>(tuple(int_<2>, int_<4>, int_<6>), half)
        ==
        just(tuple(int_<1>, int_<2>, int_<3>))
    );

    BOOST_HANA_CONSTANT_CHECK(
        traverse<Maybe>(tuple(int_<2>, int_<3>, int_<6>), half)
        ==
        nothing
    );
    //! [main]
}
