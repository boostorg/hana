/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_AUTO_SEARCHABLE_HPP
#define BOOST_HANA_TEST_TEST_AUTO_SEARCHABLE_HPP

#include <boost/hana/searchable.hpp>

#include <test/auto/base.hpp>


namespace boost { namespace hana { namespace test {
    template <typename S>
    auto laws<Searchable, S> = [] {
        //! @todo Write laws for Searchables
    };
}}} // end namespace boost::hana::test

#endif // !BOOST_HANA_TEST_TEST_AUTO_SEARCHABLE_HPP
