/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/foldable/mcd.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>
namespace hana = boost::hana;


template <typename ...>
struct types;

template <typename T, T v>
struct static_constant {
    static constexpr T value = v;
    using type = T;
};

// Note:
// Would it make sense to allow arbitrary lambdas to access the members
// of the UDT? That would allow handling accessors and similar stuff.
//
// How could we implement this if we are to accept arbitrary lambdas? Would
// that require dynamic type classes?
template <typename T, typename Members = types<>, typename Instances = types<>>
struct udt;

template <typename T, typename ...Members, typename ...Instances>
struct udt<T, types<Members...>, types<Instances...>> {
    template <typename M, M T::* p>
    using member = udt<
        T,
        types<Members..., static_constant<M T::*, p>>,
        types<Instances...>
    >;

    template <typename ...I>
    using instance_of = udt<
        T,
        types<Members...>,
        types<Instances..., I...>
    >;

    static constexpr auto instances = hana::type_list<Instances...>;
    static constexpr auto members = hana::type_list<Members...>;
};

namespace boost { namespace hana {
    template <typename T>
    struct Comparable::instance<T, T, when<
        decltype(hana::elem(hana::type<Comparable>, T::instances)){}
    >>
        : Comparable::equal_mcd
    {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y) {
            return all([=](auto memptr) {
                // Since we're using a type_list, our memptr's are actually
                // `type<static_constant<...>>`, hence the contrivedness of
                // the call to `equal`.
                using StaticConstant = typename decltype(memptr)::type;
                constexpr auto ptr = StaticConstant::value;
                return equal(x.*ptr, y.*ptr);
            }, T::members);
        }
    };

    template <typename T>
    struct Foldable::instance<T, when<
        decltype(hana::elem(hana::type<Foldable>, T::instances)){}
    >>
        : Foldable::mcd
    {
        template <typename F, typename S, typename X>
        static constexpr auto foldl_impl(F f, S s, X x) {
            auto g = [=](auto s, auto memptr) {
                using StaticConstant = typename decltype(memptr)::type;
                constexpr auto ptr = StaticConstant::value;
                return f(s, x.*ptr);
            };
            return foldl(g, s, T::members);
        }

        template <typename F, typename S, typename X>
        static constexpr auto foldr_impl(F f, S s, X x) {
            auto g = [=](auto memptr, auto s) {
                using StaticConstant = typename decltype(memptr)::type;
                constexpr auto ptr = StaticConstant::value;
                return f(x.*ptr, s);
            };
            return foldr(g, s, T::members);
        }
    };
}}


//////////////////////////////////////////////////////////////////////////////
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>


enum class Gender { male, female };

struct Person {
    Gender gender;
    int age;
};

namespace boost { namespace hana {
    template <>
    struct datatype<Person> {
        using type = udt<Person>
            ::member<Gender, &Person::gender>
            ::member<int, &Person::age>
            ::instance_of<Comparable, Foldable>;
    };
}}

int main() {
    Person louis{Gender::male, 22}, rose{Gender::female, 21};
    BOOST_HANA_STATIC_ASSERT(hana::not_equal(louis, rose));
    BOOST_HANA_STATIC_ASSERT(hana::foldr(hana::cons, hana::list(), louis) == hana::list(Gender::male, 22));
}
