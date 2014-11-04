/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

// instances
#include <test/auto/functor.hpp>
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>

#include <test/injection.hpp>
#include <tuple>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::std::Tuple> = tuple(
        type<List>,
        type<Iterable>,
        type<Functor>
    );

    template <>
    auto objects<ext::std::Tuple> = tuple(
        ::std::make_tuple(),
        ::std::make_tuple(x<0>),
        ::std::make_tuple(x<0>, x<1>),
        ::std::make_tuple(x<0>, x<1>, x<2>),
        ::std::make_tuple(x<0>, x<1>, x<2>, x<3>)
    );
}}}


int main() {
    test::check_datatype<ext::std::Tuple>();
}
