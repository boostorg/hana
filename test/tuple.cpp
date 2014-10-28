/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>
#include <test/injection.hpp>
#include <utility>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto objects<Tuple> = tuple(
            tuple()
          , tuple(x<0>)
          , tuple(x<0>, x<1>)
          , tuple(x<0>, x<1>, x<2>)
    );

    template <>
    auto instances<Tuple> = tuple(
        type<Iterable>,
        type<List>,
        type<Foldable>
    );
}}}


int main() {
    test::check_datatype<Tuple>();
    using test::x;

    // move-only friendlyness
    {
        struct movable {
            movable() = default;
            movable(movable const&) = delete;
            movable(movable&&) = default;
        };

        // disabled until c++17; see the comment on the definition of
        // hana::tuple for details
    #if 0
        auto x = tuple(movable{});
        auto z = std::move(x); (void)z;
    #endif
    }

    // Foldable
    {
        // unpack
        {
            auto f = test::injection([]{});

            BOOST_HANA_CONSTANT_ASSERT(equal(
                unpack(tuple(), f),
                f()
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unpack(tuple(x<0>), f),
                f(x<0>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unpack(tuple(x<0>, x<1>), f),
                f(x<0>, x<1>)
            ));
            BOOST_HANA_CONSTANT_ASSERT(equal(
                unpack(tuple(x<0>, x<1>, x<2>), f),
                f(x<0>, x<1>, x<2>)
            ));
        }
    }

    // Iterable
    {
        // operators
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(
                tuple(x<0>, x<1>)[int_<0>],
                x<0>
            ));
        }
    }
}
