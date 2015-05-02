/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/assert.hpp>
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

int main() {
    //////////////////////////////////////////////////////////////////////////
    // Type interface and helper functions
    //////////////////////////////////////////////////////////////////////////
    {
        // make<Type>
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Type>(T{}),
                decltype_(T{})
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                make<Type>(type<T>),
                decltype_(type<T>)
            ));

            // make sure we don't read from non-constexpr variables
            {
                auto t = type<T>;
                auto x = 1;
                constexpr auto r1 = make<Type>(t); (void)r1;
                constexpr auto r2 = make<Type>(x); (void)r2;
            }
        }

        // decltype_
        {
            T t;

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(T{}),
                type<T>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(t),
                type<T>
            ));

            // [cv-qualified] reference types
            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(static_cast<T&>(t)),
                type<T>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(static_cast<T const&>(t)),
                type<T const>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(static_cast<T volatile&>(t)),
                type<T volatile>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(static_cast<T const volatile&>(t)),
                type<T const volatile>
            ));


            // [cv-qualified] rvalue reference types
            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(static_cast<T&&>(t)),
                type<T>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(static_cast<T const &&>(t)),
                type<T const>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(static_cast<T volatile&&>(t)),
                type<T volatile>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(static_cast<T const volatile&&>(t)),
                type<T const volatile>
            ));

            // decltype_(type<T>) is the identity function
            auto const type_const = type<T>;
            auto const& type_const_ref = type<T>;
            auto& type_ref = type<T>;
            auto&& type_ref_ref = static_cast<decltype(type_ref)&&>(type_ref);

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(type<T>),
                type<T>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(type_const),
                type<T>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(type_const_ref),
                type<T>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(type_ref),
                type<T>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                decltype_(type_ref_ref),
                type<T>
            ));

            // make sure we don't read from non-constexpr variables
            {
                auto t = type<T>;
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
                    type<A>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    decltype_(a_ref),
                    type<A>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    decltype_(a_const_ref),
                    type<A const>
                ));
            }
            {
                using Fptr = int(*)();
                Fptr f;
                Fptr& f_ref = f;
                Fptr const& f_const_ref = f;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    decltype_(f),
                    type<Fptr>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    decltype_(f_ref),
                    type<Fptr>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    decltype_(f_const_ref),
                    type<Fptr const>
                ));
            }
            {
                Function& function_ref = function;

                BOOST_HANA_CONSTANT_CHECK(equal(
                    decltype_(function),
                    type<Function>
                ));

                BOOST_HANA_CONSTANT_CHECK(equal(
                    decltype_(function_ref),
                    type<Function>
                ));
            }
        }

        // sizeof_
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                sizeof_(T{}),
                size_t<sizeof(T)>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sizeof_(type<T>),
                size_t<sizeof(T)>
            ));

            // make sure we don't read from non-constexpr variables
            {
                auto t = type<T>;
                auto x = 1;
                constexpr auto r1 = sizeof_(t); (void)r1;
                constexpr auto r2 = sizeof_(x); (void)r2;
            }
        }

        // alignof_
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                alignof_(T{}),
                size_t<alignof(T)>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                alignof_(type<T>),
                size_t<alignof(T)>
            ));

            // make sure we don't read from non-constexpr variables
            {
                auto t = type<T>;
                auto x = 1;
                constexpr auto r1 = alignof_(t); (void)r1;
                constexpr auto r2 = alignof_(x); (void)r2;
            }
        }

        // is_valid
        {
            auto incrementable = is_valid([](auto&& x) -> decltype(++x) { });
            BOOST_HANA_CONSTANT_CHECK(incrementable(1));
            BOOST_HANA_CONSTANT_CHECK(not_(incrementable(undefined{})));

            BOOST_HANA_CONSTANT_CHECK(incrementable(type<int>));
            BOOST_HANA_CONSTANT_CHECK(not_(incrementable(type<void>)));

            // make sure this can be `static_assert`ed upon even though
            // `incrementable` and `i` are both non-constexpr.
            int i;
            static_assert(incrementable(i), "");

            // make sure we can use it with non-pods.
            is_valid(undefined{})(test::Tracked{1});
            is_valid([t = Tracked{1}](auto) { return 1; })(Tracked{1});

            // check is_valid with a nullary function
            auto f = [](auto ...x) { (void)sizeof...(x); /* -Wunused-param */ };
            auto g = [](auto ...x) -> char(*)[sizeof...(x)] { };
            BOOST_HANA_CONSTANT_CHECK(is_valid(f)());
            BOOST_HANA_CONSTANT_CHECK(not_(is_valid(g)()));
        }

        // nested ::type
        {
            static_assert(std::is_same<decltype(type<T>)::type, T>{}, "");
        }

        // unary +
        {
            auto& ref = type<T>;
            auto const& cref = type<T>;
            auto&& rref = type<T>;
            auto val = type<T>;

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

        // `decltype(type<T>)` should inherit `_type<T>`
        {
            static_assert(std::is_base_of<_type<T>, decltype(type<T>)>{}, "");
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal
        {
            BOOST_HANA_CONSTANT_CHECK(equal(type<T>, type<T>));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(type<T>, type<U>)));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(type<void>, type<U>)));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(type<T>, type<void>)));
            BOOST_HANA_CONSTANT_CHECK(equal(type<void>, type<void>));

            BOOST_HANA_CONSTANT_CHECK(equal(type<T&>, type<T&>));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(type<T&>, type<T&&>)));
            BOOST_HANA_CONSTANT_CHECK(not_(equal(type<T const>, type<T>)));
            BOOST_HANA_CONSTANT_CHECK(equal(type<T const>, type<T const>));

            // make sure we don't read from a non-constexpr variable in equal
            auto t = type<T>;
            static_assert(equal(t, type<T>), "");
        }

        // laws
        auto types = make<Tuple>(
            type<T>,
            type<T*>,
            type<T&>,
            type<T&&>,
            type<T const>,
            type<T volatile>,
            type<T const volatile>
        );
        test::TestComparable<Type>{types};
    }
}
