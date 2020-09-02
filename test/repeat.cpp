// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/fwd/concept/integral_constant.hpp>
#include <boost/hana/repeat.hpp>
namespace hana = boost::hana;


//////////////////////////////////////////////////////////////////////////////
// Define a simple model of IntegralConstant for use below
struct constant_tag;
template <int i>
struct constant {
    static constexpr int value = i;
    using value_type = int;
    using hana_tag = constant_tag;
};

namespace boost { namespace hana {
    template <>
    struct IntegralConstant<constant_tag> {
        static constexpr bool value = true;
    };

    // definition of `to<>` omitted
}}
//////////////////////////////////////////////////////////////////////////////

void function() { }

int main() {
    int counter = 0;
    hana::repeat(constant<3>{}, [&] { ++counter; });
    BOOST_HANA_RUNTIME_CHECK(counter == 3);

    // Try with a normal function.
    hana::repeat(constant<3>{}, function);

    // Try with a function pointer.
    hana::repeat(constant<3>{}, static_cast<void(*)()>(function));

    // Make sure we don't read from a non-constexpr variable.
    constant<3> three{};
    hana::repeat(three, []{});
}
