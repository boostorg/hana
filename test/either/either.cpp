/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either.hpp>

#include <boost/hana/detail/assert.hpp>

#include <test/injection.hpp>
#include <type_traits>
using namespace boost::hana;


int main() {
    auto f = test::injection([]{});
    auto g = test::injection([]{});
    auto x = test::x<0>;
    constexpr struct { } undefined{};

    // left
    {
        auto e = left(undefined);
        static_assert(std::is_same<
            datatype_t<decltype(e)>, Either
        >::value, "");
    }

    // right
    {
        auto e = right(undefined);
        static_assert(std::is_same<
            datatype_t<decltype(e)>, Either
        >::value, "");
    }

    // either
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            either(f, g, left(x)),
            f(x)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            either(f, g, right(x)),
            g(x)
        ));
    }
}
