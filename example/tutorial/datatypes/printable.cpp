/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>

#include <ostream>
using namespace boost::hana;


struct Printable {
    BOOST_HANA_TYPECLASS(Printable);
    struct print_mcd { };
    struct to_string_mcd { };
};

//! [methods]
auto print = [](std::ostream& os, auto x) {
    return Printable::instance<
        datatype_t<decltype(x)> // <- notice datatype_t here
    >::print_impl(os, x);
};

auto to_string = [](auto x) {
    return Printable::instance<
        datatype_t<decltype(x)> // <- same here
    >::to_string_impl(x);
};
//! [methods]

//! [my_datatype]
struct MyDatatype;
//! [my_datatype]

//! [nested]
struct MyUserDefinedType {
    using hana_datatype = MyDatatype;
};
//! [nested]

//! [ad_hoc]
struct i_cant_modify_this;

namespace boost { namespace hana {
    template <>
    struct datatype<i_cant_modify_this> {
        using type = MyDatatype;
    };
}}
//! [ad_hoc]

template <typename T>
struct is_a_boost_fusion_vector; // ...

//! [boost_fusion_vector]
struct BoostFusionVector;

namespace boost { namespace hana {
    template <typename T>
    struct datatype<T, when<is_a_boost_fusion_vector<T>::value>> {
        using type = BoostFusionVector;
    };
}}
//! [boost_fusion_vector]

//! [boost_fusion_vector_instance]
template <>
struct Printable::instance<BoostFusionVector> : Printable::print_mcd {
    // definition omitted
};
//! [boost_fusion_vector_instance]


int main() {
    (void)print;
    (void)to_string;
}
