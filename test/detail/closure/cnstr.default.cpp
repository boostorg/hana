/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/closure.hpp>

#include <string>
#include <type_traits>
using namespace boost::hana;


struct DefaultOnly {
    int data_;
    DefaultOnly(DefaultOnly const&) = delete;
    DefaultOnly& operator=(DefaultOnly const&) = delete;

    static int count;

    DefaultOnly() : data_(-1) { ++count; }
    ~DefaultOnly() { data_ = 0; --count; }

    friend bool operator==(DefaultOnly const& x, DefaultOnly const& y)
    { return x.data_ == y.data_; }

    friend bool operator< (DefaultOnly const& x, DefaultOnly const& y)
    { return x.data_ < y.data_; }
};

int DefaultOnly::count = 0;

struct NoDefault {
    NoDefault() = delete;
    explicit NoDefault(int) { }
};

struct IllFormedDefault {
    IllFormedDefault(int x) : value(x) {}
    template <bool Pred = false>
    constexpr IllFormedDefault() {
        static_assert(Pred,
            "The default constructor should not be instantiated");
    }
    int value;
};

int main() {
    {
        detail::closure<> t; (void)t;
    }
    {
        detail::closure<int> t;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
    }
    {
        detail::closure<int, char*> t;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == nullptr);
    }
    {
        detail::closure<int, char*, std::string> t;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == nullptr);
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == "");
    }
    {
        detail::closure<int, char*, std::string, DefaultOnly> t;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == nullptr);
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == "");
        BOOST_HANA_RUNTIME_CHECK(detail::get<3>(t) == DefaultOnly());
    }
    {
        // See LLVM bug #21157.
        static_assert(!std::is_default_constructible<
            detail::closure<NoDefault>
        >(), "");
        static_assert(!std::is_default_constructible<
            detail::closure<DefaultOnly, NoDefault>
        >(), "");
        static_assert(!std::is_default_constructible<
            detail::closure<NoDefault, DefaultOnly, NoDefault>
        >(), "");
    }
    {
        struct T { };
        struct U { };
        struct V { };

        constexpr detail::closure<> z0;        (void)z0;
        constexpr detail::closure<T> z1;       (void)z1;
        constexpr detail::closure<T, U> z2;    (void)z2;
        constexpr detail::closure<T, U, V> z3; (void)z3;
    }
    {
        constexpr detail::closure<int> t;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
    }
    {
        constexpr detail::closure<int, char*> t;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == nullptr);
    }

    // Make sure we can hold non default-constructible elements, and that
    // it does not trigger an error in the default constructor.
    {
        {
            IllFormedDefault v(0);
            detail::closure<IllFormedDefault> t1(v);
            detail::closure<IllFormedDefault> t2{v};
            detail::closure<IllFormedDefault> t3 = {v};
        }
        {
            detail::closure<NoDefault> t1(0);
            detail::closure<NoDefault> t2{0};
            detail::closure<NoDefault> t3 = {0};
        }
        {
            NoDefault v(0);
            detail::closure<NoDefault> t1(v);
            detail::closure<NoDefault> t2{v};
            detail::closure<NoDefault> t3 = {v};
        }
    }
}
