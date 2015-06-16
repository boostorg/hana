/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/closure.hpp>

#include <string>
using namespace boost::hana;


struct Empty {};

struct S {
    constexpr S()
        : a{1, Empty{}},
          k(detail::get<0>(a)),
          e(detail::get<1>(a))
    { }

    detail::closure<int, Empty> a;
    int k;
    Empty e;
};

constexpr detail::closure<int, int> getP () { return { 3, 4 }; }

int main() {
    {
        using T = detail::closure<int>;
        T t(3);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 3);
        detail::get<0>(t) = 2;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2);
    }
    {
        using T = detail::closure<std::string, int>;
        T t("high", 5);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == "high");
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 5);
        detail::get<0>(t) = "four";
        detail::get<1>(t) = 4;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == "four");
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 4);
    }
    {
        using T = detail::closure<double&, std::string, int>;
        double d = 1.5;
        T t(d, "high", 5);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 1.5);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == "high");
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == 5);
        detail::get<0>(t) = 2.5;
        detail::get<1>(t) = "four";
        detail::get<2>(t) = 4;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2.5);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == "four");
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == 4);
        BOOST_HANA_RUNTIME_CHECK(d == 2.5);
    }
    // get on a rvalue tuple
    {
        static_assert(detail::get<0>(detail::closure<float, int, double, long>{0.0f, 1, 2.0, 3L}) == 0, "" );
        static_assert(detail::get<1>(detail::closure<float, int, double, long>{0.0f, 1, 2.0, 3L}) == 1, "" );
        static_assert(detail::get<2>(detail::closure<float, int, double, long>{0.0f, 1, 2.0, 3L}) == 2, "" );
        static_assert(detail::get<3>(detail::closure<float, int, double, long>{0.0f, 1, 2.0, 3L}) == 3, "" );
        static_assert(S().k == 1, "");
        static_assert(detail::get<1>(getP()) == 4, "");
    }
    {
        // make sure get<> returns the right types
        struct T { };
        struct U { };
        struct V { };

        detail::closure<T, U, V> xs{};
        (void)static_cast<T>(detail::get<0>(xs));
        (void)static_cast<U>(detail::get<1>(xs));
        (void)static_cast<V>(detail::get<2>(xs));
    }
    {

    }
}
