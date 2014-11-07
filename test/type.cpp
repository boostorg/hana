/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/tuple.hpp>

#include <test/auto/base.hpp>

// instances
#include <test/auto/comparable.hpp>

#include <type_traits>
using namespace boost::hana;


struct T { }; struct U;

namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Type> = tuple(type<Comparable>);

    template <>
    auto objects<Type> = tuple(
        type<void>,
        type<char>,
        type<T>,
        type<T*>,
        type<T&>,
        type<T&&>,
        type<T const>,
        type<T volatile>,
        type<T const volatile>
    );
}}}


int main() {
    test::check_datatype<Type>();

    // Type interface and helper functions
    {
        // decltype_
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(decltype_(1), type<int>));
            BOOST_HANA_CONSTANT_ASSERT(equal(decltype_('1'), type<char>));
            BOOST_HANA_CONSTANT_ASSERT(equal(decltype_(T{}), type<T>));
        }

        // sizeof_
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(sizeof_(type<int>), size_t<sizeof(int)>));
            BOOST_HANA_CONSTANT_ASSERT(equal(sizeof_(type<T>), size_t<sizeof(T)>));
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

            BOOST_HANA_CONSTANT_ASSERT(equal(val, +val));
            BOOST_HANA_CONSTANT_ASSERT(equal(val, +ref));
            BOOST_HANA_CONSTANT_ASSERT(equal(val, +cref));
            BOOST_HANA_CONSTANT_ASSERT(equal(val, +rref));

            static_assert(!std::is_reference<decltype(+val)>{}, "");
            static_assert(!std::is_reference<decltype(+ref)>{}, "");
            static_assert(!std::is_reference<decltype(+cref)>{}, "");
            static_assert(!std::is_reference<decltype(+rref)>{}, "");

            using T1 = decltype(+val)::type;
            using T2 = decltype(+ref)::type;
            using T3 = decltype(+cref)::type;
            using T4 = decltype(+rref)::type;
        }
    }

    // Comparable
    {
        // equal
        {
            BOOST_HANA_CONSTANT_ASSERT(equal(type<T>, type<T>));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(type<T>, type<U>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(type<void>, type<U>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(type<T>, type<void>)));
            BOOST_HANA_CONSTANT_ASSERT(equal(type<void>, type<void>));

            BOOST_HANA_CONSTANT_ASSERT(equal(type<T&>, type<T&>));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(type<T&>, type<T&&>)));
            BOOST_HANA_CONSTANT_ASSERT(not_(equal(type<T const>, type<T>)));
            BOOST_HANA_CONSTANT_ASSERT(equal(type<T const>, type<T const>));
        }
    }
}
