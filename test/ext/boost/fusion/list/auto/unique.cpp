/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/version.hpp>

// Appears to be broken on Boost 1.58.0
#if BOOST_VERSION != 105800
#   include "_specs.hpp"
#   include "../../../../../auto/unique.hpp"
#endif

int main() { }
