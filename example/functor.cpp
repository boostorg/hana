/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#include <sstream>
#include <string>
#include <type_traits>
using namespace boost::hana;


int main() {
    {
        //! [adjust]
        BOOST_HANA_CONSTEXPR_LAMBDA auto negative = [](auto x) {
            return x < 0;
        };

        BOOST_HANA_CONSTEXPR_LAMBDA auto negate = [](auto x) {
            return -x;
        };

        BOOST_HANA_CONSTEXPR_ASSERT(
            adjust(tuple(-3, -2, -1, 0, 1, 2, 3), negative, negate)
            ==
            tuple(3, 2, 1, 0, 1, 2, 3)
        );
        //! [adjust]
    }

    {
        //! [fill]
        BOOST_HANA_CONSTEXPR_ASSERT(
            fill(tuple(1, '2', 3.3, nullptr), 'x') == tuple('x', 'x', 'x', 'x')
        );

        BOOST_HANA_CONSTANT_ASSERT(fill(nothing, 'x') == nothing);
        BOOST_HANA_CONSTEXPR_ASSERT(fill(just('y'), 'x') == just('x'));
        //! [fill]
    }

    {
        //! [fmap]
        auto to_string = [](auto x) {
            return (std::ostringstream{} << x).str();
        };

        BOOST_HANA_RUNTIME_ASSERT(
            fmap(tuple(1, '2', "345", std::string{"67"}), to_string)
            ==
            tuple("1", "2", "345", "67")
        );

        BOOST_HANA_CONSTANT_ASSERT(fmap(nothing, to_string) == nothing);
        BOOST_HANA_RUNTIME_ASSERT(fmap(just(123), to_string) == just("123"));

        BOOST_HANA_CONSTANT_ASSERT(
            fmap(type_list<void, int(), char[10]>, template_<std::add_pointer_t>)
                    ==
            type_list<void*, int(*)(), char(*)[10]>
        );
        //! [fmap]
    }

    {
        //! [replace]
        BOOST_HANA_CONSTEXPR_LAMBDA auto negative = [](auto x) {
            return x < 0;
        };

        BOOST_HANA_CONSTEXPR_ASSERT(
            replace(tuple(-3, -2, -1, 0, 1, 2, 3), negative, 0)
            ==
            tuple(0, 0, 0, 0, 1, 2, 3)
        );
        //! [replace]
    }
}
