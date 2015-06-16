/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/closure.hpp>

#include <string>
using namespace boost::hana;


template <typename ...>
struct never { static constexpr bool value = false; };

struct NoValueCtor {
    NoValueCtor() : id(++count) {}
    NoValueCtor(NoValueCtor const & other) : id(other.id) { ++count; }

    // The constexpr is required to make is_constructible instantiate this
    // template. The explicit is needed to test-around a similar bug with
    // is_convertible.
    template <typename T>
    constexpr explicit NoValueCtor(T)
    { static_assert(never<T>::value, "This should not be instantiated"); }

    static int count;
    int id;
};

int NoValueCtor::count = 0;


struct NoValueCtorEmpty {
    NoValueCtorEmpty() {}
    NoValueCtorEmpty(NoValueCtorEmpty const &) {}

    template <typename T>
    constexpr explicit NoValueCtorEmpty(T)
    { static_assert(never<T>::value, "This should not be instantiated"); }
};

int main() {
    {
        detail::closure<int> t(2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2);
    }
    {
        constexpr detail::closure<int> t(2);
        static_assert(detail::get<0>(t) == 2, "");
    }
    {
        constexpr detail::closure<int> t;
        static_assert(detail::get<0>(t) == 0, "");
    }
    {
        constexpr detail::closure<int, char*> t(2, nullptr);
        static_assert(detail::get<0>(t) == 2, "");
        static_assert(detail::get<1>(t) == nullptr, "");
    }
    {
        detail::closure<int, char*> t(2, nullptr);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == nullptr);
    }
    {
        detail::closure<int, char*, std::string> t(2, nullptr, "text");
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == nullptr);
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == "text");
    }
    {
        detail::closure<int, NoValueCtor, int, int> t(1, NoValueCtor(), 2, 3);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 1);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t).id == 1);
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<3>(t) == 3);
    }
    {
        detail::closure<int, NoValueCtorEmpty, int, int> t(1, NoValueCtorEmpty(), 2, 3);
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 1);
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == 2);
        BOOST_HANA_RUNTIME_CHECK(detail::get<3>(t) == 3);
    }
    {
        struct T { };
        struct U { };
        struct V { };

        constexpr T t{};
        constexpr U u{};
        constexpr V v{};

        constexpr detail::closure<T> x1{t};             (void)x1;
        constexpr detail::closure<T, U> x2{t, u};       (void)x2;
        constexpr detail::closure<T, U, V> x3{t, u, v}; (void)x3;
    }
    {
        struct T { };
        struct U { };
        struct V { };

        // Check for SFINAE-friendliness
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

    // Make sure we can initialize elements with the brace-init syntax.
    {
        struct Member { };
        struct Element { Member member; };

        detail::closure<Element, Element> xs{{Member()}, {Member()}};
        detail::closure<Element, Element> ys = {{Member()}, {Member()}};
        (void)xs; (void)ys;
    }
}
