/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/type.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/ext/std/utility.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>
#include <laws/comparable.hpp>

#include <type_traits>
using namespace boost::hana;


struct T { }; struct U;

using Function = void();
void function() { }
struct undefined { };

struct static_nested_member { static const int member = 1; };
struct static_nested_member_array { static int member[3]; };
struct nested_template_struct { template <typename ...> struct nested; };

int main() {
using boost::hana::size_t; // disambiguate with std::size_t on GCC

//////////////////////////////////////////////////////////////////////////
// Type interface and helper functions
//////////////////////////////////////////////////////////////////////////
{
    // make<type_tag> and make_type
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            make<type_tag>(T{}),
            decltype_(T{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make<type_tag>(type_c<T>),
            decltype_(type_c<T>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make_type(T{}),
            make<type_tag>(T{})
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            make_type(type_c<T>),
            make<type_tag>(type_c<T>)
        ));

        // make sure we don't read from non-constexpr variables
        {
            auto t = type_c<T>;
            auto x = 1;
            constexpr auto r1 = make<type_tag>(t); (void)r1;
            constexpr auto r2 = make<type_tag>(x); (void)r2;
        }
    }

    // decltype_
    {
        T t;

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(T{}),
            type_c<T>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(t),
            type_c<T>
        ));

        // [cv-qualified] reference types
        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(static_cast<T&>(t)),
            type_c<T>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(static_cast<T const&>(t)),
            type_c<T const>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(static_cast<T volatile&>(t)),
            type_c<T volatile>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(static_cast<T const volatile&>(t)),
            type_c<T const volatile>
        ));


        // [cv-qualified] rvalue reference types
        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(static_cast<T&&>(t)),
            type_c<T>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(static_cast<T const &&>(t)),
            type_c<T const>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(static_cast<T volatile&&>(t)),
            type_c<T volatile>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(static_cast<T const volatile&&>(t)),
            type_c<T const volatile>
        ));

        // decltype_(type_c<T>) is the identity function
        auto const type_const = type_c<T>;
        auto const& type_const_ref = type_c<T>;
        auto& type_ref = type_c<T>;
        auto&& type_ref_ref = static_cast<decltype(type_ref)&&>(type_ref);

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(type_c<T>),
            type_c<T>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(type_const),
            type_c<T>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(type_const_ref),
            type_c<T>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(type_ref),
            type_c<T>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            decltype_(type_ref_ref),
            type_c<T>
        ));

        // make sure we don't read from non-constexpr variables
        {
            auto t = type_c<T>;
            auto x = 1;
            constexpr auto r1 = decltype_(t); (void)r1;
            constexpr auto r2 = decltype_(x); (void)r2;
        }

        // decltype_ with builtin arrays, function pointers and other weirdos
        {
            using A = T[3];
            A a;
            A& a_ref = a;
            A const& a_const_ref = a;

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(a),
                type_c<A>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(a_ref),
                type_c<A>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(a_const_ref),
                type_c<A const>
            ));
        }
        {
            using Fptr = int(*)();
            Fptr f;
            Fptr& f_ref = f;
            Fptr const& f_const_ref = f;

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(f),
                type_c<Fptr>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(f_ref),
                type_c<Fptr>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(f_const_ref),
                type_c<Fptr const>
            ));
        }
        {
            Function& function_ref = function;

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(function),
                type_c<Function>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(function_ref),
                type_c<Function>
            ));
        }
    }

    // sizeof_
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            sizeof_(T{}),
            size_c<sizeof(T)>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            sizeof_(type_c<T>),
            size_c<sizeof(T)>
        ));

        // make sure we don't read from non-constexpr variables
        {
            auto t = type_c<T>;
            auto x = 1;
            constexpr auto r1 = sizeof_(t); (void)r1;
            constexpr auto r2 = sizeof_(x); (void)r2;
        }
    }

    // alignof_
    {
        BOOST_HANA_CONSTANT_CHECK(equal(
            alignof_(T{}),
            size_c<alignof(T)>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            alignof_(type_c<T>),
            size_c<alignof(T)>
        ));

        // make sure we don't read from non-constexpr variables
        {
            auto t = type_c<T>;
            auto x = 1;
            constexpr auto r1 = alignof_(t); (void)r1;
            constexpr auto r2 = alignof_(x); (void)r2;
        }
    }

    // is_valid
    {
        // Check for a non-static member
        {
            struct yes { int member; };
            struct no { };

            auto from_type = is_valid([](auto t) -> decltype(
                traits::declval(t).member
            ) { });
            BOOST_HANA_CONSTANT_CHECK(from_type(type_c<yes>));
            BOOST_HANA_CONSTANT_CHECK(not_(from_type(type_c<no>)));

            auto from_object = is_valid([](auto&& t) -> decltype(
                t.member
            ) { });
            BOOST_HANA_CONSTANT_CHECK(from_object(yes{}));
            BOOST_HANA_CONSTANT_CHECK(not_(from_object(no{})));
        }

        // Check for a static member
        {
            using yes = static_nested_member;
            struct no { };

            auto from_type = is_valid([](auto t) -> decltype(
                decltype(t)::type::member
            ) { });
            BOOST_HANA_CONSTANT_CHECK(from_type(type_c<yes>));
            BOOST_HANA_CONSTANT_CHECK(not_(from_type(type_c<no>)));

            auto from_object = is_valid([](auto&& t) -> decltype(
                std::remove_reference_t<decltype(t)>::member
            ) { });
            BOOST_HANA_CONSTANT_CHECK(from_object(yes{}));
            BOOST_HANA_CONSTANT_CHECK(not_(from_object(no{})));
        }

        // Check for a nested type
        {
            struct yes { using nested = void; };
            struct no { };

            auto from_type = is_valid([](auto t) -> decltype(type_c<
                typename decltype(t)::type::nested
            >) { });
            BOOST_HANA_CONSTANT_CHECK(from_type(type_c<yes>));
            BOOST_HANA_CONSTANT_CHECK(not_(from_type(type_c<no>)));

            auto from_object = is_valid([](auto&& t) -> decltype(type_c<
                typename std::remove_reference_t<decltype(t)>::nested
            >) { });
            BOOST_HANA_CONSTANT_CHECK(from_object(yes{}));
            BOOST_HANA_CONSTANT_CHECK(not_(from_object(no{})));
        }

        // Check for a nested template
        {
            { // template struct
            using yes = nested_template_struct;
            struct no { };

            auto from_type = is_valid([](auto t) -> decltype(template_<
                decltype(t)::type::template nested
            >) { });
            BOOST_HANA_CONSTANT_CHECK(from_type(type_c<yes>));
            BOOST_HANA_CONSTANT_CHECK(not_(from_type(type_c<no>)));

            auto from_object = is_valid([](auto&& t) -> decltype(template_<
                std::remove_reference_t<decltype(t)>::template nested
            >) { });
            BOOST_HANA_CONSTANT_CHECK(from_object(yes{}));
            BOOST_HANA_CONSTANT_CHECK(not_(from_object(no{})));
            }

            { // template alias
            struct yes { template <typename ...> using nested = void; };
            struct no { };

            auto from_type = is_valid([](auto t) -> decltype(template_<
                decltype(t)::type::template nested
            >) { });
            BOOST_HANA_CONSTANT_CHECK(from_type(type_c<yes>));
            BOOST_HANA_CONSTANT_CHECK(not_(from_type(type_c<no>)));

            auto from_object = is_valid([](auto&& t) -> decltype(template_<
                std::remove_reference_t<decltype(t)>::template nested
            >) { });
            BOOST_HANA_CONSTANT_CHECK(from_object(yes{}));
            BOOST_HANA_CONSTANT_CHECK(not_(from_object(no{})));
            }
        }

        // Make sure that checking for a nested static or non-static member
        // works even when the type of that member is an array type or
        // something that can't be returned from a function.
        {
            { // non-static member
            struct yes { int member[3]; };
            struct no { };

            auto from_type = is_valid([](auto t) -> decltype(
                (void)traits::declval(t).member
            ) { });
            BOOST_HANA_CONSTANT_CHECK(from_type(type_c<yes>));
            BOOST_HANA_CONSTANT_CHECK(not_(from_type(type_c<no>)));

            auto from_object = is_valid([](auto&& t) -> decltype(
                (void)t.member
            ) { });
            BOOST_HANA_CONSTANT_CHECK(from_object(yes{}));
            BOOST_HANA_CONSTANT_CHECK(not_(from_object(no{})));
            }

            { // static member
            using yes = static_nested_member_array;
            struct no { };

            auto from_type = is_valid([](auto t) -> decltype(
                (void)decltype(t)::type::member
            ) { });
            BOOST_HANA_CONSTANT_CHECK(from_type(type_c<yes>));
            BOOST_HANA_CONSTANT_CHECK(not_(from_type(type_c<no>)));

            auto from_object = is_valid([](auto&& t) -> decltype(
                (void)std::remove_reference_t<decltype(t)>::member
            ) { });
            BOOST_HANA_CONSTANT_CHECK(from_object(yes{}));
            BOOST_HANA_CONSTANT_CHECK(not_(from_object(no{})));
            }
        }

        // Make sure the result of a `is_valid` function is constexpr
        // even when called on non-constexpr arguments.
        {
            int i;
            auto f = is_valid([](auto) { });
            constexpr auto result = f(i);
            (void)result;
        }

        // Make sure `is_valid` works with non-PODs.
        {
            is_valid(undefined{})(test::Tracked{1});
            is_valid([t = Tracked{1}](auto) { return 1; })(Tracked{1});
        }

        // Check `is_valid` with a nullary function.
        {
            auto f = [](auto ...x) { (void)sizeof...(x); /* -Wunused-param */ };
            auto g = [](auto ...x) -> char(*)[sizeof...(x)] { };
            BOOST_HANA_CONSTANT_CHECK(is_valid(f)());
            BOOST_HANA_CONSTANT_CHECK(not_(is_valid(g)()));
        }

        // Call `is_valid` in the non-curried form.
        {
            struct yes { int member; };
            struct no { };

            auto f = [](auto&& t) -> decltype(t.member) { };

            BOOST_HANA_CONSTANT_CHECK(is_valid(f, yes{}));
            BOOST_HANA_CONSTANT_CHECK(not_(is_valid(f, no{})));
        }
    }

    // nested ::type
    {
        static_assert(std::is_same<decltype(type_c<T>)::type, T>{}, "");
    }

    // unary +
    {
        auto& ref = type_c<T>;
        auto const& cref = type_c<T>;
        auto&& rref = type_c<T>;
        auto val = type_c<T>;

        BOOST_HANA_CONSTANT_CHECK(equal(val, +val));
        BOOST_HANA_CONSTANT_CHECK(equal(val, +ref));
        BOOST_HANA_CONSTANT_CHECK(equal(val, +cref));
        BOOST_HANA_CONSTANT_CHECK(equal(val, +rref));

        static_assert(!std::is_reference<decltype(+val)>{}, "");
        static_assert(!std::is_reference<decltype(+ref)>{}, "");
        static_assert(!std::is_reference<decltype(+cref)>{}, "");
        static_assert(!std::is_reference<decltype(+rref)>{}, "");

        using T1 = decltype(+val)::type;
        using T2 = decltype(+ref)::type;
        using T3 = decltype(+cref)::type;
        using T4 = decltype(+rref)::type;
    }

    // `decltype(type_c<T>)` should inherit `basic_type<T>`
    {
        static_assert(std::is_base_of<basic_type<T>, decltype(type_c<T>)>{}, "");
    }
}

//////////////////////////////////////////////////////////////////////////
// Comparable
//////////////////////////////////////////////////////////////////////////
{
    // equal
    {
        BOOST_HANA_CONSTANT_CHECK(type_c<T> == type_c<T>);
        BOOST_HANA_CONSTANT_CHECK(type_c<T> != type_c<U>);

        BOOST_HANA_CONSTANT_CHECK(equal(type_c<T>, type_c<T>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(type_c<T>, type_c<U>)));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(type_c<void>, type_c<U>)));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(type_c<T>, type_c<void>)));
        BOOST_HANA_CONSTANT_CHECK(equal(type_c<void>, type_c<void>));

        BOOST_HANA_CONSTANT_CHECK(equal(type_c<T&>, type_c<T&>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(type_c<T&>, type_c<T&&>)));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(type_c<T const>, type_c<T>)));
        BOOST_HANA_CONSTANT_CHECK(equal(type_c<T const>, type_c<T const>));

        // make sure we don't read from a non-constexpr variable in equal
        auto t = type_c<T>;
        static_assert(equal(t, type_c<T>), "");
    }

    // laws
    auto types = make_tuple(
        type_c<T>,
        type_c<T*>,
        type_c<T&>,
        type_c<T&&>,
        type_c<T const>,
        type_c<T volatile>,
        type_c<T const volatile>
    );
    test::TestComparable<type_tag>{types};
}
}
