/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/sequence.hpp>
#include <test/cnumeric.hpp>
#include <test/injection.hpp>

#include <tuple>
using namespace boost::hana;


template <int i>
constexpr auto ord = test::cnumeric<int, i>;

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::std::Tuple> = make<Tuple>(
        type<Sequence>
    );

    template <>
    auto objects<ext::std::Tuple> = make<Tuple>(
        ::std::make_tuple(),
        ::std::make_tuple(ord<0>),
        ::std::make_tuple(ord<0>, ord<1>),
        ::std::make_tuple(ord<0>, ord<1>, ord<2>),
        ::std::make_tuple(ord<0>, ord<1>, ord<2>, ord<3>)
    );
}}}


int main() {
    test::check_datatype<ext::std::Tuple>();
}
