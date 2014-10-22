/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>

#include <test/laws/constant.hpp>
using namespace boost::hana;


int main() {
    // value
    {
        static_assert(value(true_) == true, "");
        static_assert(value(false_) == false, "");
    }

    // laws
    {
        Constant_laws(true_, false_);
    }
}
