/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core.hpp> // for instantiates
#include <boost/hana/detail/enable_if.hpp>
#include <boost/hana/foldable/mcd.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/searchable/find_mcd.hpp>


namespace boost { namespace hana {
    struct AssociativeUDT {
        BOOST_HANA_TYPECLASS(AssociativeUDT);
        struct mcd { };
    };

    template <typename T>
    auto member_map = AssociativeUDT::instance<T>::member_map_impl();

    template <typename T>
    struct Comparable::instance<T, T, when<instantiates<AssociativeUDT, T>()>>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y) {
            return all([=](auto accessor) {
                return equal(accessor(x), accessor(y));
            }, values(member_map<T>));
        }
    };

    template <typename T>
    struct Foldable::instance<T, when<instantiates<AssociativeUDT, T>()>>
        : Foldable::mcd
    {
        template <typename F, typename S, typename X>
        static constexpr auto foldl_impl(F f, S s, X x) {
            auto g = [=](auto s, auto accessor) {
                return f(s, accessor(x));
            };
            return foldl(g, s, values(member_map<T>));
        }

        template <typename F, typename S, typename X>
        static constexpr auto foldr_impl(F f, S s, X x) {
            auto g = [=](auto accessor, auto s) {
                return f(accessor(x), s);
            };
            return foldr(g, s, values(member_map<T>));
        }
    };

    template <typename T>
    struct Searchable::instance<T, when<instantiates<AssociativeUDT, T>()>>
        : Searchable::find_mcd
    {
        template <typename Pred, typename X>
        static constexpr auto find_impl(Pred pred, X x) {
            auto maybe_accessor = find(pred, member_map<T>);
            return fmap([=](auto accessor) { return accessor(x); }, maybe_accessor);
        }

        template <typename Pred, typename X>
        static constexpr auto any_impl(Pred pred, X x) {
            return any(pred, member_map<T>);
        }
    };

    template <typename T>
    struct convert<Map, T, detail::enable_if_t<instantiates<AssociativeUDT, T>()>> {
        template <typename X>
        static constexpr auto apply(X x) {

        }
    };

    template <typename T>
    struct convert<T, Map, detail::enable_if_t<instantiates<AssociativeUDT, T>()>> {
        template <typename M>
        static constexpr auto apply(M m) {

        }
    };
}} // end namespace boost::hana


//////////////////////////////////////////////////////////////////////////////
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair/instance.hpp>
using namespace boost::hana;


enum class Gender { male, female };

struct Person {
    Gender gender;
    int age;
};

template <typename T, T v>
struct _constant {
    static constexpr T value = v;
    constexpr operator T() const { return value; }

    template <typename X, X x, typename Y, Y y>
    static constexpr auto eq(_constant<X, x>, _constant<Y, y>)
        -> decltype(bool_<x == y>)
    { return bool_<x == y>; }

    static constexpr auto eq(...)
    { return false_; }

    template <typename X, X x, typename Y, Y y>
    friend constexpr auto operator==(_constant<X, x> a, _constant<Y, y> b)
    { return eq(a, b); }
};

template <typename T, T v>
constexpr _constant<T, v> constant{};


auto gender = constant<decltype(&Person::gender), &Person::gender>;
auto age = constant<decltype(&Person::age), &Person::age>;

namespace boost { namespace hana {
    template <>
    struct AssociativeUDT::instance<Person> : AssociativeUDT::mcd {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto member_map_impl() {
            return map(
                pair(gender, [](auto p) { return p.gender; }),
                pair(age, [](auto p) { return p.age; })
            );
        }
    };
}}

int main() {
    Person louis{Gender::male, 22}, rose{Gender::female, 21};
    BOOST_HANA_STATIC_ASSERT(not_equal(louis, rose));
    BOOST_HANA_STATIC_ASSERT(foldr(cons, list(), louis) == list(Gender::male, 22));

    BOOST_HANA_STATIC_ASSERT(lookup(gender, louis) == just(Gender::male));
    BOOST_HANA_STATIC_ASSERT(lookup(age, louis) == just(22));
}
