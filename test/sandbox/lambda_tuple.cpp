/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/sandbox/lambda_tuple.hpp>

#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/foldable.hpp>
#include <test/auto/iterable.hpp>
#include <test/auto/list.hpp>
#include <test/injection.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto objects<sandbox::LambdaTuple> = tuple(
            sandbox::lambda_tuple()
          , sandbox::lambda_tuple(x<0>)
          , sandbox::lambda_tuple(x<0>, x<1>)
          , sandbox::lambda_tuple(x<0>, x<1>, x<2>)
          , sandbox::lambda_tuple(x<0>, x<1>, x<2>, x<3>)
          , sandbox::lambda_tuple(x<0>, x<1>, x<2>, x<3>, x<4>)
          , sandbox::lambda_tuple(x<0>, x<1>, x<2>, x<3>, x<4>, x<5>)
    );

    template <>
    auto instances<sandbox::LambdaTuple> = tuple(
        type<Iterable>,
        type<List>,
        type<Foldable>
    );
}}}


int main() {
    test::check_datatype<sandbox::LambdaTuple>();
}
