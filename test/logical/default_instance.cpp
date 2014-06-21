/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/logical.hpp>

#include <boost/hana/detail/constexpr.hpp>

#include <cassert>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto test = [](auto true_, auto false_) {
    assert(eval_if(true_,
        [=](auto) { return true_; },
        [](auto id) { return 1 / id(0); }
    ) == true_);
    assert(eval_if(false_,
        [](auto id) { return 1 / id(0); },
        [=](auto) { return false_; }
    ) == false_);

    assert(if_(true_, true_, false_) == true_);
    assert(if_(false_, true_, false_) == false_);

    assert(not_(not_(true_)));
    assert(not_(false_));
};

int main() {
    test(true, false);
    test(1, 0);
    test(2ll, 0ll);
    test(2, false);
}
