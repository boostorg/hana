/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/tuple/comparable.hpp>
#include <boost/hana/ext/std/tuple/list.hpp>

#include <test/iso/list/methods.hpp>
using namespace boost::hana;


int main() {
    //! @todo Re-enable the tests once this is fixed:
    //! http://llvm.org/bugs/show_bug.cgi?id=19793
#if 0
    List_methods<StdTuple>();
#endif
}
