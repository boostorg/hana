/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/closure.hpp>

#include <type_traits>
#include <utility>
using namespace boost::hana;


struct MoveOnly {
    int data_;
    MoveOnly(MoveOnly const&) = delete;
    MoveOnly& operator=(MoveOnly const&) = delete;
    MoveOnly(int data = 1) : data_(data) { }
    MoveOnly(MoveOnly&& x) : data_(x.data_) { x.data_ = 0; }

    MoveOnly& operator=(MoveOnly&& x)
    { data_ = x.data_; x.data_ = 0; return *this; }

    int get() const {return data_;}
    bool operator==(const MoveOnly& x) const { return data_ == x.data_; }
    bool operator< (const MoveOnly& x) const { return data_ <  x.data_; }
};

int main() {
    {
        using T = detail::closure<>;
        T t0;
        T t = std::move(t0); (void)t;
    }
    {
        using T = detail::closure<MoveOnly>;
        T t0(MoveOnly(0));
        T t = std::move(t0); (void)t;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
    }
    {
        using T = detail::closure<MoveOnly, MoveOnly>;
        T t0(MoveOnly(0), MoveOnly(1));
        T t = std::move(t0); (void)t;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 1);
    }
    {
        using T = detail::closure<MoveOnly, MoveOnly, MoveOnly>;
        T t0(MoveOnly(0), MoveOnly(1), MoveOnly(2));
        T t = std::move(t0); (void)t;
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 1);
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == 2);
    }
    {
        // Check for SFINAE-friendliness
        static_assert(!std::is_constructible<
            detail::closure<MoveOnly>, MoveOnly const&
        >{}, "");

        static_assert(!std::is_constructible<
            detail::closure<MoveOnly>, MoveOnly&
        >{}, "");

        static_assert(std::is_constructible<
            detail::closure<MoveOnly>, MoveOnly
        >{}, "");

        static_assert(std::is_constructible<
            detail::closure<MoveOnly>, MoveOnly&&
        >{}, "");
    }
}
