/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/lambda_tuple.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/sequence.hpp>
#include <test/cnumeric.hpp>
using namespace boost::hana;


template <int i>
constexpr auto ord = test::cnumeric<int, i>;

namespace boost { namespace hana { namespace test {
    template <>
    auto objects<sandbox::LambdaTuple> = tuple(
            sandbox::lambda_tuple()
          , sandbox::lambda_tuple(ord<0>)
          , sandbox::lambda_tuple(ord<0>, ord<1>)
          , sandbox::lambda_tuple(ord<0>, ord<1>, ord<2>)
          , sandbox::lambda_tuple(ord<0>, ord<1>, ord<2>, ord<3>)
          , sandbox::lambda_tuple(ord<0>, ord<1>, ord<2>, ord<3>, ord<4>)
          , sandbox::lambda_tuple(ord<0>, ord<1>, ord<2>, ord<3>, ord<4>, ord<5>)
    );

    template <>
    auto instances<sandbox::LambdaTuple> = tuple(
        type<Sequence>
    );
}}}


int main() {
    test::check_datatype<sandbox::LambdaTuple>();
}
