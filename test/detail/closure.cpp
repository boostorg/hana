/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/closure.hpp>

#include <type_traits>
#include <utility>
using namespace boost::hana;


struct nocopy {
    nocopy() = default;
    nocopy(nocopy const&) = delete;
    nocopy(nocopy&&) = default;
};

struct T { };
struct U { };
struct V { };

int main() {
    // construction
    {
        constexpr T t{};
        constexpr U u{};
        constexpr V v{};

        constexpr detail::closure<> x0{}; (void)x0;
        constexpr detail::closure<T> x1{t}; (void)x1;
        constexpr detail::closure<T, U> x2{t, u}; (void)x2;
        constexpr detail::closure<T, U, V> x3{t, u, v}; (void)x3;

        constexpr detail::closure<> copy1(x0); (void) copy1;
        constexpr detail::closure<T, U> copy2(x2); (void)copy2;
    }

    // non-copyable friendliness
    {
        detail::closure<nocopy, nocopy, nocopy> xs{};
        auto ys = std::move(xs); (void)ys;
    }

    // SFINAE-friendliness of the constructor
    {
        static_assert(!std::is_copy_constructible<
            detail::closure<nocopy>
        >{}, "");

        static_assert(!std::is_constructible<
            detail::closure<T, U>, T
        >{}, "");

        static_assert(!std::is_constructible<
            detail::closure<T, U>, U, T
        >{}, "");

        static_assert(!std::is_constructible<
            detail::closure<T, U>, T, U, V
        >{}, "");

        static_assert(!std::is_constructible<
            detail::closure<float>, double
        >{}, "");
    }
}
