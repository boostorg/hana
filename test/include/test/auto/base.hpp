/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_BASE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_BASE_HPP

#include <boost/hana/constant.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana { namespace test {
    template <typename Datatype>
    auto objects = tuple();

    template <typename Datatype>
    auto instances = tuple();

    template <typename Typeclass, typename ...Datatypes>
    auto laws = [] { };

    template <typename Datatype>
    auto check_datatype = [] {
        for_each(instances<Datatype>, [](auto instance) {
            using Instance = typename decltype(instance)::type;
            laws<Instance, Datatype>();
        });
    };


    auto implies = infix([](auto p, auto q) {
        return or_(not_(p), q);
    });

    auto iff = infix([](auto p, auto q) {
        return and_(implies(p, q), implies(q, p));
    });
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_BASE_HPP
