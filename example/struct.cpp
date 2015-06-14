/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/struct.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
using namespace boost::hana;


int main() {

//////////////////////////////////////////////////////////////////////////////
// Models
//////////////////////////////////////////////////////////////////////////////
{

//! [Comparable]
struct Person {
    BOOST_HANA_DEFINE_STRUCT(Person,
        (std::string, name),
        (unsigned short, age)
    );
};

Person john{"John", 30}, kevin{"Kevin", 20};

BOOST_HANA_RUNTIME_CHECK(equal(john, john));
BOOST_HANA_RUNTIME_CHECK(not_equal(john, kevin));
//! [Comparable]

}{

//! [Foldable]
struct Classroom {
    BOOST_HANA_DEFINE_STRUCT(Classroom,
        (unsigned short, boys),
        (unsigned short, girls)
    );
};

constexpr Classroom compsci{20, 3}; /* lol */

BOOST_HANA_CONSTEXPR_CHECK(
    fold_left(compsci, 0, [](auto total, auto member) {
        // first(member) is the name of the member, here
        // "boys" or "girls", and second(member) is its value.
        return second(member) + total;
    }) == 23
);
//! [Foldable]

}{

//! [to<Map>]
struct Person {
    BOOST_HANA_DEFINE_STRUCT(Person,
        (std::string, name),
        (unsigned short, age)
    );
};

Person john{"John", 30u};
BOOST_HANA_RUNTIME_CHECK(to<Map>(john) == make<Map>(
    make<Pair>(BOOST_HANA_STRING("name"), "John"),
    make<Pair>(BOOST_HANA_STRING("age"), 30u)
));
//! [to<Map>]

}{

//! [Searchable]
struct Person {
    BOOST_HANA_DEFINE_STRUCT(Person,
        (std::string, name),
        (unsigned short, age)
    );
};

Person john{"John", 30};

BOOST_HANA_RUNTIME_CHECK(
    find(john, BOOST_HANA_STRING("name")) == just("John")
);

BOOST_HANA_CONSTANT_CHECK(
    find(john, BOOST_HANA_STRING("foobar")) == nothing
);
//! [Searchable]

}

//////////////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////////////
{

//! [accessors]
struct Person {
    BOOST_HANA_DEFINE_STRUCT(Person,
        (std::string, name),
        (unsigned short, age)
    );
};

BOOST_HANA_CONSTANT_CHECK(
    transform(accessors<Person>(), first) == make_tuple(
        BOOST_HANA_STRING("name"), BOOST_HANA_STRING("age")
    )
);

auto get_name = second(at_c<0>(accessors<Person>()));
auto get_age = second(at_c<1>(accessors<Person>()));

Person john{"John", 30};
BOOST_HANA_RUNTIME_CHECK(get_name(john) == "John");
BOOST_HANA_RUNTIME_CHECK(get_age(john) == 30);
//! [accessors]

}{

//! [members]
struct Person {
    BOOST_HANA_DEFINE_STRUCT(Person,
        (std::string, name),
        (unsigned short, age)
    );
};

Person john{"John", 30};
BOOST_HANA_RUNTIME_CHECK(members(john) == make_tuple("John", 30));
//! [members]

}{

//! [keys]
struct Person {
    BOOST_HANA_DEFINE_STRUCT(Person,
        (std::string, name),
        (unsigned short, age)
    );
};

Person john{"John", 30};
BOOST_HANA_CONSTANT_CHECK(
    keys(john) == make_tuple(BOOST_HANA_STRING("name"),
                             BOOST_HANA_STRING("age"))
);
//! [keys]

}

}
