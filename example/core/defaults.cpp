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

BOOST_HANA_CONSTEXPR_LAMBDA auto next_n = [](auto x, unsigned int n) {
    return Incrementable<hana::datatype_t<decltype(x)>>::next_n_impl(x, n);
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
        struct with : defaults<> {
            template <typename X>
            static constexpr auto next_n_impl(X x, unsigned int n) {
                if (n == 0) return x;
                else        return next_n_impl(next(x), n - 1);
            }

            template <typename X>
            static constexpr auto next_impl(X x)
            { return next_n(x, 1); }
        };
    };
}}

template <typename T, typename Enable>
struct Incrementable
    : hana::instance<Incrementable>::template with<T>
{ };

template <>
struct Incrementable<int> : hana::defaults<Incrementable>::with<int> {
    static constexpr auto next_impl(int x)
    { return x + 1; }
};

template <>
struct Incrementable<long> : hana::defaults<Incrementable>::with<long> {
    static constexpr auto next_n_impl(long x, unsigned int n)
    { return x + n; }
};

int main() {
    BOOST_HANA_STATIC_ASSERT(next(1) == 2);
    BOOST_HANA_STATIC_ASSERT(next_n(1, 3) == 4); // default implementation

    BOOST_HANA_STATIC_ASSERT(next(1l) == 2l); // default implementation
    BOOST_HANA_STATIC_ASSERT(next_n(1l, 3) == 4l);
}
