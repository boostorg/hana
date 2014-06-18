/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>

#include <type_traits>
namespace hana = boost::hana;


template <typename T, typename Enable = void>
struct Incrementable;

BOOST_HANA_CONSTEXPR_LAMBDA auto next = [](auto x) {
    return Incrementable<hana::datatype_t<decltype(x)>>::next_impl(x);
};

namespace boost { namespace hana {
    template <>
    struct instance<Incrementable> {
        template <typename T, typename Enable = void>
        struct with { };
    };

    template <>
    struct defaults<Incrementable> {
        template <typename T, typename Enable = void>
        struct with { };
    };
}}

template <typename T, typename Enable>
struct Incrementable
    : hana::instance<Incrementable>::template with<T>
{ };

namespace boost { namespace hana {
    template <typename T>
    struct instance<Incrementable>::with<T,
        std::enable_if_t<std::is_integral<T>{} || std::is_floating_point<T>{}>
    >
        : defaults<Incrementable>::template with<T>
    {
        template <typename X>
        static constexpr auto next_impl(X x)
        { return x + 1; }
    };
}}


int main() {
    BOOST_HANA_STATIC_ASSERT(next(1) == 2);
    BOOST_HANA_STATIC_ASSERT(next(3.3) == 4.3);
    BOOST_HANA_STATIC_ASSERT(next('a') == 'b');
}
