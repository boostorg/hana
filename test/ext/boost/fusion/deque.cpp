/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion/deque.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/sequence.hpp>
#include <test/cnumeric.hpp>
#include <test/injection.hpp>

#include <boost/fusion/container/generation/make_deque.hpp>
#include <boost/fusion/container/deque.hpp>
#include <type_traits>
using namespace boost::hana;


template <int i>
constexpr auto ord = test::cnumeric<int, i>;

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<ext::boost::fusion::Deque> = make<Tuple>(
        type<Sequence>
    );

    template <>
    auto objects<ext::boost::fusion::Deque> = make<Tuple>(
        ::boost::fusion::make_deque(),
        ::boost::fusion::make_deque(ord<0>),
        ::boost::fusion::make_deque(ord<0>, ord<1>),
        ::boost::fusion::make_deque(ord<0>, ord<1>, ord<2>),
        ::boost::fusion::make_deque(ord<0>, ord<1>, ord<2>, ord<3>)
    );
}}}


int main() {
    test::check_datatype<ext::boost::fusion::Deque>();
}
