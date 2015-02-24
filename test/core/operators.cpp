/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/operators.hpp>
namespace hana = boost::hana;


struct Concept { };
struct _method { };
constexpr _method method{};

namespace boost { namespace hana { namespace operators {
    template <>
    struct of<Concept>
        : decltype(::method)
    { };
}}}

struct Nested {
    struct hana {
        struct operators
            : boost::hana::operators::of<Concept>
        { };
    };
};

static_assert(boost::hana::has_operator<Nested, decltype(method)>, "");

int main() { }
