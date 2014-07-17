/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/list/instance.hpp>


int main() {
    using namespace boost::hana;
    using namespace boost::hana::operators;

    // This would be ambiguous if decltype(list()) is defined in the
    // `boost::hana::operators` namespace, like it was before.
    list();
}
