/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion/list.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

// instances
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>

#include <boost/fusion/container/generation/make_list.hpp>
#include <boost/fusion/container/list.hpp>
#include <test/injection.hpp>
#include <type_traits>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::boost::fusion::List> = tuple(
        type<List>,
        type<Iterable>
    );

    template <>
    auto objects<ext::boost::fusion::List> = tuple(
        ::boost::fusion::make_list(),
        ::boost::fusion::make_list(x<0>),
        ::boost::fusion::make_list(x<0>, x<1>),
        ::boost::fusion::make_list(x<0>, x<1>, x<2>),
        ::boost::fusion::make_list(x<0>, x<1>, x<2>, x<3>)
    );
}}}


int main() {
    test::check_datatype<ext::boost::fusion::List>();
}
