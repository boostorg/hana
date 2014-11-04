/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/wrap.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/datatype.hpp>

#include <type_traits>
using namespace boost::hana;


struct Datatype;

struct Anything;
struct anything { struct hana { using datatype = Anything; }; };

static_assert(std::is_same<
    datatype_t<decltype(detail::wrap<Datatype>(anything{}))>,
    Datatype
>::value, "");

static_assert(std::is_same<
    datatype_t<anything>,
    datatype_t<decltype(
        detail::unwrap(detail::wrap<Datatype>(anything{}))
    )>
>::value, "");

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(detail::unwrap(detail::wrap<Datatype>(2.2)) == 2.2);
}
