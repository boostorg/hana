/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/either.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    using test::ct_eq;
    struct undefined { };

    //////////////////////////////////////////////////////////////////////////
    // Interface
    //////////////////////////////////////////////////////////////////////////
    {
        test::_injection<0> f{};
        test::_injection<0> g{};

        // left
        {
            auto e = left(undefined{});
            static_assert(std::is_same<
                datatype_t<decltype(e)>, Either
            >::value, "");
        }

        // right
        {
            auto e = right(undefined{});
            static_assert(std::is_same<
                datatype_t<decltype(e)>, Either
            >::value, "");
        }

        // either
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                either(f, g, left(ct_eq<0>{})),
                f(ct_eq<0>{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                either(f, g, right(ct_eq<0>{})),
                g(ct_eq<0>{})
            ));
        }
    }
}
