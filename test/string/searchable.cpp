/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/string.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/maybe.hpp>
using namespace boost::hana;


int main() {
    // any
    {
        BOOST_HANA_CONSTANT_ASSERT(
            any(BOOST_HANA_STRING("abcd"), [](auto c) {
                return equal(c, char_<'b'>);
            })
        );

        BOOST_HANA_CONSTANT_ASSERT(
            not_(any(BOOST_HANA_STRING(""), [](auto c) {
                return true_;
            }))
        );

        BOOST_HANA_CONSTANT_ASSERT(
            not_(any(BOOST_HANA_STRING("abcd"), [](auto c) {
                return equal(c, char_<'z'>);
            }))
        );
    }

    // find
    {
        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(BOOST_HANA_STRING(""), [](auto c) { return true_; }),
            nothing
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(BOOST_HANA_STRING("abcd"), [](auto c) {
                return equal(c, char_<'a'>);
            }),
            just(char_<'a'>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(BOOST_HANA_STRING("abcd"), [](auto c) {
                return equal(c, char_<'c'>);
            }),
            just(char_<'c'>)
        ));

        BOOST_HANA_CONSTANT_ASSERT(equal(
            find(BOOST_HANA_STRING("abcd"), [](auto c) {
                return equal(c, char_<'d'>);
            }),
            just(char_<'d'>)
        ));
    }
}
