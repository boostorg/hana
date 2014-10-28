/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/tuple/multiple.hpp>

#include <test/iso/list.hpp>


int main() {
    List_isomorphism<boost::hana::detail::multiple::Tuple>();
}
