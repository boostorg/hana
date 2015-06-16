/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/closure.hpp>

#include <string>
#include <memory>
using namespace boost::hana;


struct B {
    int id_;
    explicit B(int i) : id_(i) {}
    virtual ~B() {}
};

struct D : B {
    explicit D(int i) : B(i) {}
};

int main() {
    {
        using T0 = detail::closure<double>;
        using T1 = detail::closure<int>;
        T0 t0(2.5);
        T1 t1 = std::move(t0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t1) == 2);
    }
    {
        using T0 = detail::closure<double, char>;
        using T1 = detail::closure<int, int>;
        T0 t0(2.5, 'a');
        T1 t1 = std::move(t0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t1) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t1) == int('a'));
    }
    {
        using T0 = detail::closure<double, char, D>;
        using T1 = detail::closure<int, int, B>;
        T0 t0(2.5, 'a', D(3));
        T1 t1 = std::move(t0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t1) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t1) == int('a'));
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t1).id_ == 3);
    }
    {
        D d(3);
        using T0 = detail::closure<double, char, D&>;
        using T1 = detail::closure<int, int, B&>;
        T0 t0(2.5, 'a', d);
        T1 t1 = std::move(t0);
        d.id_ = 2;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t1) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t1) == int('a'));
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t1).id_ == 2);
    }
    {
        using T0 = detail::closure<double, char, std::unique_ptr<D>>;
        using T1 = detail::closure<int, int, std::unique_ptr<B>>;
        T0 t0(2.5, 'a', std::unique_ptr<D>(new D(3)));
        T1 t1 = std::move(t0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t1) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t1) == int('a'));
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t1)->id_ == 3);
    }
}
