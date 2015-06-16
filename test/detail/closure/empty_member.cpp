/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/closure.hpp>
using namespace boost::hana;


struct A { };
struct B { };

int main() {
    {
        using T = detail::closure<int, A>;
        static_assert((sizeof(T) == sizeof(int)), "");
    }
    {
        using T = detail::closure<A, int>;
        static_assert((sizeof(T) == sizeof(int)), "");
    }
    {
        using T = detail::closure<A, int, B>;
        static_assert((sizeof(T) == sizeof(int)), "");
    }
    {
        using T = detail::closure<A, B, int>;
        static_assert((sizeof(T) == sizeof(int)), "");
    }
    {
        using T = detail::closure<int, A, B>;
        static_assert((sizeof(T) == sizeof(int)), "");
    }
}
