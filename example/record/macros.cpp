/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#define BOOST_PP_VARIADICS 1
#include <boost/hana/record_macros.hpp>
#include <boost/hana/type.hpp>

#include <string>
using namespace boost::hana;


namespace intrusive {
//! [intrusive]
struct Person {
    BOOST_HANA_DEFINE_RECORD_INTRUSIVE(Person,
        (std::string, name),
        (int, age)
    );
};
//! [intrusive]
}

//! [adhoc]
namespace ns {
    struct Person {
        std::string name;
        int age;
    };
}

BOOST_HANA_DEFINE_RECORD(ns::Person,
    (std::string, name),
    (int, age)
);
//! [adhoc]

int main() {

}
