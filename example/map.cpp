/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/type.hpp>

#include <string>
using namespace boost::hana;


int main() {
    {
        //! [comparable]
        BOOST_HANA_RUNTIME_CHECK(
            map(
                pair(char_<'a'>, std::string{"foobar"}),
                pair(type<int&&>, nullptr)
            )
            ==
            map(
                pair(type<int&&>, (void*)0),
                pair(char_<'a'>, "foobar")
            )
        );
        //! [comparable]
    }

    {
        //! [searchable]
        BOOST_HANA_CONSTEXPR_LAMBDA auto m = map(
            pair(type<int>, 'i'),
            pair(type<float>, 'f')
        );
        BOOST_HANA_CONSTEXPR_CHECK(lookup(m, type<int>) == just('i'));
        BOOST_HANA_CONSTEXPR_CHECK(lookup(m, type<float>) == just('f'));
        BOOST_HANA_CONSTANT_CHECK(lookup(m, type<void>) == nothing);
        BOOST_HANA_CONSTANT_CHECK(lookup(m, int_<3>) == nothing);
        //! [searchable]
    }
}
