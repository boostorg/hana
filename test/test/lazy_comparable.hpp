/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_LAZY_COMPARABLE_HPP
#define BOOST_HANA_TEST_TEST_LAZY_COMPARABLE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/lazy.hpp>


namespace boost { namespace hana {
    // We provide this instance for unit tests only because it is _so_ much
    // more convenient, but this instance is too dangerous for general usage.
    // See the documentation of `Lazy` for more info.
    template <>
    struct Comparable::instance<Lazy, Lazy> : Comparable::equal_mcd {
        template <typename X, typename Y>
        static constexpr auto equal_impl(X x, Y y)
        { return equal(eval(x), eval(y)); }
    };
}}

#endif // !BOOST_HANA_TEST_TEST_LAZY_COMPARABLE_HPP
