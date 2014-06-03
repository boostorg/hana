/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>

#include <type_traits>
using namespace boost::hana;


struct NestedDatatype;
struct Nested { using hana_datatype = NestedDatatype; };

struct NoNested { };


int main() {
    static_assert(std::is_same<datatype_t<Nested>, NestedDatatype>{}, "");
    static_assert(std::is_same<datatype_t<NoNested>, NoNested>{}, "");
}
