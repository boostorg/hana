// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HANA_TEST_BASIC_TUPLE_COMPARABLE_HPP
#define BOOST_HANA_TEST_BASIC_TUPLE_COMPARABLE_HPP

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/fwd/equal.hpp>
#include <boost/hana/tuple.hpp>


// This file provides a model of Comparable for hana::basic_tuple. This model
// is required by the some unit tests, but it is not provided by default with
// basic_tuple because the implementation of the algorithm is a bit heavy.

namespace boost { namespace hana {
    template <>
    struct equal_impl<basic_tuple_tag, basic_tuple_tag> {
        template <typename Ts, typename Us>
        static constexpr auto apply(Ts const& ts, Us const& us) {
            return hana::equal(hana::to_tuple(ts), hana::to_tuple(us));
        }
    };
}}

#endif // !BOOST_HANA_TEST_BASIC_TUPLE_COMPARABLE_HPP
