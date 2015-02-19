/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/operators.hpp>
using namespace boost::hana;


struct Concept { };

struct NestedOperators {
    struct hana { struct enabled_operators : Concept { }; };
};

struct NoNested { };

static_assert(enable_operators<Concept, NestedOperators>::value, "");
static_assert(!enable_operators<Concept, NoNested>::value, "");
static_assert(!enable_operators<Concept, void>::value, "");


int main() { }
