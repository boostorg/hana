/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_APPLICATIVE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_APPLICATIVE_HPP

#include <boost/hana/applicative.hpp>

#include <boost/hana/core/models.hpp>
#include <boost/hana/functor.hpp>

#include <test/auto/base.hpp>
#include <test/auto/functor.hpp>


namespace boost { namespace hana { namespace test {
    template <typename A>
    auto laws<Applicative, A> = [] {
        laws<Functor, A>();

        static_assert(_models<Applicative, A>{}, "");

        //! @todo Write Applicative laws
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_APPLICATIVE_HPP
