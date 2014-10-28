/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/tuple/multiple.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>

// instances
#include <test/auto/foldable.hpp>
#include <test/auto/functor.hpp>
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<detail::multiple::Tuple> = tuple(
        type<List>,
        type<Functor>,
        type<Foldable>,
        type<Iterable>
    );

    template <>
    auto objects<detail::multiple::Tuple> = tuple(
        detail::multiple::make_tuple(),
        detail::multiple::make_tuple(x<0>),
        detail::multiple::make_tuple(x<0>, x<1>),
        detail::multiple::make_tuple(x<0>, x<1>, x<2>),
        detail::multiple::make_tuple(x<0>, x<1>, x<2>, x<3>),
        detail::multiple::make_tuple(x<0>, x<1>, x<2>, x<3>, x<4>),
        detail::multiple::make_tuple(x<0>, x<1>, x<2>, x<3>, x<4>, x<5>)
    );
}}}


int main() {
    test::check_datatype<detail::multiple::Tuple>();
}
