/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/closure.hpp>

#include <string>
using namespace boost::hana;


struct Empty { };

int main() {
    {
        using T = detail::closure<>;
        T t0;
        T t_implicit = t0;
        T t_explicit(t0);

        (void)t_explicit;
        (void)t_implicit;
    }
    {
        using T = detail::closure<int>;
        T t0(2);
        T t = t0;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2);
    }
    {
        using T = detail::closure<int, char>;
        T t0(2, 'a');
        T t = t0;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 'a');
    }
    {
        using T = detail::closure<int, char, std::string>;
        const T t0(2, 'a', "some text");
        T t = t0;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 'a');
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == "some text");
    }
    {
        using T = detail::closure<int>;
        constexpr T t0(2);
        constexpr T t = t0;
        static_assert(detail::get<0>(t) == 2, "");
    }
    {
        using T = detail::closure<Empty>;
        constexpr T t0;
        constexpr T t = t0;
        constexpr Empty e = detail::get<0>(t); (void)e;
    }
    {
        struct T { };
        struct U { };

        constexpr detail::closure<T, U> binary{};
        constexpr detail::closure<T, U> copy_implicit = binary;
        constexpr detail::closure<T, U> copy_explicit(binary);

        (void)copy_implicit;
        (void)copy_explicit;
    }
}
