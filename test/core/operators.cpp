/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/operators.hpp>
using namespace boost::hana;


struct Typeclass { };

struct NestedOperators {
    struct hana { struct enabled_operators : Typeclass { }; };
};

struct NoNested { };

static_assert(enable_operators<Typeclass, NestedOperators>::value, "");
static_assert(!enable_operators<Typeclass, NoNested>::value, "");
static_assert(!enable_operators<Typeclass, void>::value, "");


int main() { }
