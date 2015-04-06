/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>
using namespace boost::hana;


template <typename ...> struct F { struct type; };
struct M { template <typename ...> struct apply { struct type; }; };
struct T;

int main() {
    // See http://llvm.org/bugs/show_bug.cgi?id=20046
    [](auto) { return trait<F>; }(1);
    [](auto) { return type<T>; }(1);
    [](auto) { return template_<F>; }(1);
    [](auto) { return metafunction<F>; }(1);
    [](auto) { return metafunction_class<M>; }(1);
}
