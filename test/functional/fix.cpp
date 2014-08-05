/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/integral.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto fact = fix(
    [](auto fact, auto n) {
        return eval_if(n == int_<0>,
            always(int_<1>),
            [=](auto id) { return n * fact(n - id(int_<1>)); }
        );
    }
);

constexpr unsigned long long reference(unsigned long long n)
{ return n == 0 ? 1 : n * reference(n - 1); }

template <int n>
constexpr void test() {
    BOOST_HANA_CONSTANT_ASSERT(fact(ullong<n>) == ullong<reference(n)>);
    test<n - 1>();
}

template <> constexpr void test<-1>() { }

int main() {
    test<15>();
}
