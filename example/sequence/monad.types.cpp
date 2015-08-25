/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/chain.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/ext/std/type_traits.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
#include <utility>
namespace hana = boost::hana;


// Using the Tuple Monad, we generate all the possible combinations of
// cv-qualifiers and reference qualifiers. Then, we use the Optional Monad
// to make sure that our generic function can be called with arguments
// of any of those types.

// cv_qualifiers : Type -> Tuple(Type)
auto cv_qualifiers = [](auto t) {
    return hana::make_tuple(
        t,
        hana::traits::add_const(t),
        hana::traits::add_volatile(t),
        hana::traits::add_volatile(hana::traits::add_const(t))
    );
};

// ref_qualifiers : Type -> Tuple(Type)
auto ref_qualifiers = [](auto t) {
    return hana::make_tuple(
        hana::traits::add_lvalue_reference(t),
        hana::traits::add_rvalue_reference(t)
    );
};

auto possible_args = cv_qualifiers(hana::type<int>) | ref_qualifiers;

BOOST_HANA_CONSTANT_CHECK(
    possible_args == hana::make_tuple(
                        hana::type<int&>,
                        hana::type<int&&>,
                        hana::type<int const&>,
                        hana::type<int const&&>,
                        hana::type<int volatile&>,
                        hana::type<int volatile&&>,
                        hana::type<int const volatile&>,
                        hana::type<int const volatile&&>
                    )
);

struct some_function {
    template <typename T>
    void operator()(T&&) const { }
};

int main() {
    hana::for_each(possible_args, [](auto t) {
        using T = typename decltype(t)::type;
        static_assert(decltype(hana::is_valid(some_function{})(std::declval<T>())){},
        "some_function should be callable with any type of argument");
    });
}
