/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <type_traits>
namespace hana = boost::hana;


struct Incrementable {
    BOOST_HANA_TYPECLASS(Incrementable);
    struct next_mcd;
    struct next_n_mcd;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto next = [](auto x) {
    return Incrementable::instance<
        hana::datatype_t<decltype(x)>
    >::next_impl(x);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto next_n = [](auto x, unsigned int n) {
    return Incrementable::instance<
        hana::datatype_t<decltype(x)>
    >::next_n_impl(x, n);
};

struct Incrementable::next_mcd {
    template <typename X>
    static constexpr auto next_n_impl(X x, unsigned int n) {
        if (n == 0) return x;
        else        return next_n_impl(next(x), n - 1);
    }
};

struct Incrementable::next_n_mcd {
    template <typename X>
    static constexpr auto next_impl(X x)
    { return next_n(x, 1); }
};

template <>
struct Incrementable::instance<int> : Incrementable::next_mcd {
    static constexpr auto next_impl(int x)
    { return x + 1; }
};

template <>
struct Incrementable::instance<long> : Incrementable::next_n_mcd {
    static constexpr auto next_n_impl(long x, unsigned int n)
    { return x + n; }
};

template <typename T>
struct Incrementable::instance<T, hana::when<std::is_floating_point<T>{}>>
    : Incrementable::next_mcd
{
    template <typename X>
    static constexpr auto next_impl(X x)
    { return x + 1; }
};

int main() {
    BOOST_HANA_CONSTEXPR_ASSERT(next(1) == 2);
    BOOST_HANA_CONSTEXPR_ASSERT(next_n(1, 3) == 4);

    BOOST_HANA_CONSTEXPR_ASSERT(next(1l) == 2l);
    BOOST_HANA_CONSTEXPR_ASSERT(next_n(1l, 3) == 4l);

    BOOST_HANA_CONSTEXPR_ASSERT(next(1.0) == 2.0);
    BOOST_HANA_CONSTEXPR_ASSERT(next_n(1.0, 3) == 4.0);
}
