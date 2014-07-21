/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core.hpp> // for instantiates
#include <boost/hana/detail/enable_if.hpp>
#include <boost/hana/foldable/mcd.hpp>


namespace boost { namespace hana {
    struct UDT {
        BOOST_HANA_TYPECLASS(UDT);
        struct mcd { };
    };

    template <typename T>
    auto members = UDT::instance<T>::members_impl();

    template <typename T>
    struct Comparable::instance<T, T, when<instantiates<UDT, T>()>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y) {
            return all([=](auto accessor) {
                return equal(accessor(x), accessor(y));
            }, members<T>);
        }
    };

    template <typename T>
    struct Foldable::instance<T, when<instantiates<UDT, T>()>>
        : Foldable::mcd
    {
        template <typename F, typename S, typename X>
        static constexpr auto foldl_impl(F f, S s, X x) {
            auto g = [=](auto s, auto accessor) {
                return f(s, accessor(x));
            };
            return foldl(g, s, members<T>);
        }

        template <typename F, typename S, typename X>
        static constexpr auto foldr_impl(F f, S s, X x) {
            auto g = [=](auto accessor, auto s) {
                return f(accessor(x), s);
            };
            return foldr(g, s, members<T>);
        }
    };
}} // end namespace boost::hana


//////////////////////////////////////////////////////////////////////////////
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/list/instance.hpp>
using namespace boost::hana;


enum class Gender { male, female };

struct Person {
    Gender gender;
    int age;
};

namespace boost { namespace hana {
    template <>
    struct UDT::instance<Person> : UDT::mcd {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto members_impl() {
            return list(
                [=](auto p) { return p.gender; },
                [=](auto p) { return p.age; }
            );
        }
    };
}}

int main() {
    Person louis{Gender::male, 22}, rose{Gender::female, 21};
    BOOST_HANA_STATIC_ASSERT(not_equal(louis, rose));
    BOOST_HANA_STATIC_ASSERT(foldr(cons, list(), louis) == list(Gender::male, 22));
}

#if 0
// Makes it Orderable (lexicographical)
BOOST_HANA_ADAPT_STRUCT_ORDERABLE(
    (name)
    (member1)
    (member2)
);

// Makes it Comparable (memberwise)
BOOST_HANA_ADAPT_STRUCT_COMPARABLE(
    (name)
    (member1)
    (member2)
);

// Makes it Searchable
BOOST_HANA_ADAPT_STRUCT_SEARCHABLE(
    (name)
    (key1)(member1)
    (key2)(member2)
);

// Makes it convertible to and from a Map
BOOST_HANA_ADAPT_STRUCT_MAP(
    (name)
    (key1)(member1)
    (key2)(member2)
);

// Makes it Foldable
BOOST_HANA_ADAPT_STRUCT_FOLDABLE(
    (name)
    (member1)
    (member2)
);
#endif
