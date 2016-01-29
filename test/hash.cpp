/*
@copyright Jason Rice 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/all.hpp>
#include <boost/hana/and.hpp>
#include <boost/hana/assert.hpp>
#include <boost/hana/chain.hpp>
#include <boost/hana/detail/decay.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/fuse.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/hash.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/or.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/tuple.hpp>

#include<type_traits>

namespace hana = boost::hana;

// specializations of hash need to prove that
// values with a common hash value are Comparable
// with each other or buckets will explode

template <typename T>
struct combine {
    T t;

    template <typename U>
    constexpr auto operator()(U u) const {
        return hana::transform(t, hana::partial(hana::make_pair, u));
    }
};

struct check_t {
    template <typename T_, typename U_>
    constexpr auto operator()(T_ t, U_ u) const {
        using T = hana::tag_of_t<T_>;
        using U = hana::tag_of_t<U_>;
        constexpr auto can_be_compared = hana::bool_c<
            (hana::detail::has_nontrivial_common_embedding<hana::Comparable, T, U>::value
                || hana::detail::EqualityComparable<T, U>::value)
            || std::is_same<T_, U_>::value
        >;
        constexpr auto hashes_are_equal = hana::equal(hana::hash(t), hana::hash(u));
        return (hashes_are_equal && can_be_compared) || !hashes_are_equal;
    }
};
constexpr auto check = hana::fuse(check_t{});

struct foo {
    int bar;
};

int main() {
    {
        constexpr auto xs = hana::make_tuple(
            hana::integral_constant<char, 42>{}
            , hana::integral_constant<signed char, 42>{}
            , hana::integral_constant<signed short, 42>{}
            , hana::integral_constant<signed int, 42>{}
            , hana::integral_constant<signed long, 42>{}
            , hana::integral_constant<unsigned char, 42>{}
            , hana::integral_constant<unsigned short, 42>{}
            , hana::integral_constant<unsigned int, 42>{}
            , hana::integral_constant<unsigned long, 42>{}
        );
        BOOST_HANA_CONSTANT_ASSERT(hana::all(hana::transform(xs | combine<decltype(xs)>{xs},  check)));
        hana::transform(xs | combine<decltype(xs)>{xs},  check);
    }

    // pointers to members should always hash to their identity
    {
        constexpr auto x = hana::integral_constant<int foo::*, &foo::bar>{};
        BOOST_HANA_CONSTANT_ASSERT(hana::equal(hana::hash(x), hana::decltype_(x)));
    }
}
