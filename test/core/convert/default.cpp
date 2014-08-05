/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/convert.hpp>

#include <boost/hana/detail/assert.hpp>

#include <string>
#include <type_traits>
using namespace boost::hana;


template <typename ToDatatype, typename From, typename To>
void check_convert(From from, To to_) {
    BOOST_HANA_RUNTIME_ASSERT(to<ToDatatype>(from) == to_);
    static_assert(std::is_same<decltype(to<To>(from)), To>{}, "");
}

struct Datatype {
    int value;
    using hana_datatype = Datatype;
    constexpr bool operator==(Datatype x)
    { return value == x.value; }
};

struct other_ctor {
    int value;
    using hana_datatype = Datatype;
    constexpr bool operator==(other_ctor x)
    { return value == x.value; }
};

int main() {
    check_convert<std::string>("abcdef", std::string{"abcdef"});
    check_convert<double>(int{1}, double{1});
    check_convert<int>(double{1}, int{1});
    check_convert<int>(std::true_type{}, int{1});
    check_convert<int>(std::false_type{}, int{0});
    check_convert<Datatype>(Datatype{1}, Datatype{1});
    check_convert<Datatype>(other_ctor{1}, other_ctor{1});
}
