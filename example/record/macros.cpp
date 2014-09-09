/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/record/macros.hpp>
#include <boost/hana/type.hpp>

#include <string>
using namespace boost::hana;


namespace intrusive {
//! [intrusive]
struct name_tag;
struct age_tag;

struct Person {
    BOOST_HANA_DEFINE_RECORD_INTRUSIVE(Person,
        ((type<name_tag>)(std::string)(name))
        ((type<age_tag>)(int)(age))
    );
};
//! [intrusive]
}

//! [adhoc]
struct name_tag;
struct age_tag;

namespace ns {
    struct Person {
        std::string name;
        int age;
    };
}

BOOST_HANA_DEFINE_RECORD(ns::Person,
    ((type<name_tag>)(std::string)(name))
    ((type<age_tag>)(int)(age))
);
//! [adhoc]

int main() {

}
