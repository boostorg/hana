/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_EXT_BOOST_FUSION_HELPER_HPP
#define BOOST_HANA_TEST_EXT_BOOST_FUSION_HELPER_HPP

#include <boost/hana/list/instance.hpp>

#include <boost/fusion/container/generation/make_cons.hpp>
#include <boost/fusion/container/generation/make_deque.hpp>
#include <boost/fusion/container/generation/make_list.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/container/list/cons.hpp>
#include <boost/fusion/tuple/make_tuple.hpp>


auto with_nonassociative_forward_sequences = [](auto test) {
    test([](auto ...xs) { return boost::fusion::make_vector(xs...); });
    test([](auto ...xs) { return boost::fusion::make_list(xs...); });
    test([](auto ...xs) { return boost::fusion::make_deque(xs...); });
    test([](auto ...xs) { return boost::fusion::make_tuple(xs...); });
    test([](auto ...xs) {
        return boost::hana::foldr(
            [](auto x, auto xs) { return boost::fusion::make_cons(x, xs); },
            boost::fusion::nil{},
            boost::hana::list(xs...)
        );
    });
};

#endif // !BOOST_HANA_TEST_EXT_BOOST_FUSION_HELPER_HPP
