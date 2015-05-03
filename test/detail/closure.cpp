/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/closure.hpp>

#include <laws/base.hpp>

#include <type_traits>
#include <utility>
using namespace boost::hana;


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

        // default-constructibility
        constexpr detail::closure<T, U, V> z3{}; (void)z3;
    }

    //////////////////////////////////////////////////////////////////////////
    // Make sure we do not try to instantiate invalid constructors when
    // doing copies.
    //////////////////////////////////////////////////////////////////////////
    {
        detail::closure<test::trap_construct> expr{};
        auto implicit_copy = expr;          (void)implicit_copy;
        decltype(expr) explicit_copy(expr); (void)explicit_copy;
    }

    // non-copyable friendliness
    {
        detail::closure<test::no_copy, test::no_copy, test::no_copy> xs{};
        auto ys = std::move(xs); (void)ys;
    }

    // SFINAE-friendliness of the constructor
    {
        static_assert(!std::is_copy_constructible<
            detail::closure<test::no_copy>
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
    }

    // get
    {
        {
            T t{};
            U u{};
            V v{};

            detail::closure<T, U, V> xs{t, u, v};
            (void)static_cast<T>(detail::get<0>(xs));
            (void)static_cast<U>(detail::get<1>(xs));
            (void)static_cast<V>(detail::get<2>(xs));
        }

        // make sure we don't double-move and do other weird stuff
        {
            detail::closure<test::Tracked, test::Tracked, test::Tracked> xs{
                test::Tracked{1}, test::Tracked{2}, test::Tracked{3}
            };

            test::Tracked a = detail::get<0>(std::move(xs)); (void)a;
            test::Tracked b = detail::get<1>(std::move(xs)); (void)b;
            test::Tracked c = detail::get<2>(std::move(xs)); (void)c;
        }

        // test with nested closures
        {
            using Inner = detail::closure<test::Tracked, test::Tracked>;
            detail::closure<Inner> xs{Inner{test::Tracked{1}, test::Tracked{2}}};

            test::Tracked a = detail::get<0>(detail::get<0>(std::move(xs))); (void)a;
            test::Tracked b = detail::get<1>(detail::get<0>(std::move(xs))); (void)b;
        }
    }
}
