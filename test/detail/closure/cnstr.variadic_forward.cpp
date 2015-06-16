/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/closure.hpp>

#include <type_traits>
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

struct Empty { };
struct A {
    int id_;
    explicit constexpr A(int i) : id_(i) {}
};

struct NoDefault { NoDefault() = delete; };

int main() {
    {
        detail::closure<MoveOnly> t(MoveOnly(0));
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
    }
    {
        detail::closure<MoveOnly, MoveOnly> t(MoveOnly(0), MoveOnly(1));
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 1);
    }
    {
        detail::closure<MoveOnly, MoveOnly, MoveOnly> t(
            MoveOnly(0), MoveOnly(1), MoveOnly(2)
        );
        BOOST_HANA_RUNTIME_CHECK(detail::get<0>(t) == 0);
        BOOST_HANA_RUNTIME_CHECK(detail::get<1>(t) == 1);
        BOOST_HANA_RUNTIME_CHECK(detail::get<2>(t) == 2);
    }
    {
        constexpr detail::closure<Empty> t0{Empty()};
    }
    {
        constexpr detail::closure<A, A> t(3, 2);
        static_assert(detail::get<0>(t).id_ == 3, "");
    }
    {
        typedef detail::closure<MoveOnly, NoDefault> Tuple;

        static_assert(!std::is_constructible<
            Tuple,
            MoveOnly
        >::value, "");

        static_assert(std::is_constructible<
            Tuple,
            MoveOnly, NoDefault
        >::value, "");
    }
    {
        typedef detail::closure<MoveOnly, MoveOnly, NoDefault> Tuple;

        static_assert(!std::is_constructible<
            Tuple,
            MoveOnly, MoveOnly
        >::value, "");

        static_assert(std::is_constructible<
            Tuple,
            MoveOnly, MoveOnly, NoDefault
        >::value, "");
    }
    {
        typedef detail::closure<MoveOnly, NoDefault> Tuple;
        typedef detail::closure<MoveOnly, Tuple, MoveOnly, MoveOnly> NestedTuple;

        static_assert(!std::is_constructible<
            NestedTuple,
            MoveOnly, MoveOnly, MoveOnly, MoveOnly
        >::value, "");

        static_assert(std::is_constructible<
            NestedTuple,
            MoveOnly, Tuple, MoveOnly, MoveOnly
        >::value, "");
    }
    {
        typedef detail::closure<MoveOnly, int> Tuple;
        typedef detail::closure<MoveOnly, Tuple, MoveOnly, MoveOnly> NestedTuple;

        static_assert(!std::is_constructible<
            NestedTuple,
            MoveOnly, MoveOnly, MoveOnly, MoveOnly
        >::value, "");

        static_assert(std::is_constructible<
            NestedTuple,
            MoveOnly, Tuple, MoveOnly, MoveOnly
        >::value, "");
    }
}
