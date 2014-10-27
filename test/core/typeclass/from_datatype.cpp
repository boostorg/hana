/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>

#include <type_traits>
using namespace boost::hana;


struct Typeclass {
    BOOST_HANA_TYPECLASS(Typeclass);
};

struct Datatype {
    struct hana { struct Typeclass { struct dispatch; }; };
};

static_assert(std::is_same<
    Typeclass::instance<Datatype>::dispatch,
    Datatype::hana::Typeclass::dispatch
>::value, "");

int main() { }
