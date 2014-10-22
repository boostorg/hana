/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_ISO_LIST_HPP
#define BOOST_HANA_TEST_TEST_ISO_LIST_HPP

#include <test/iso/list/applicative.hpp>
#include <test/iso/list/functor.hpp>
#include <test/iso/list/iterable.hpp>
#include <test/iso/list/methods.hpp>
#include <test/iso/list/monad.hpp>


template <typename L>
void List_isomorphism() {
    List_methods<L>();

    List_functor<L>();
    List_applicative<L>();
    List_monad<L>();

    List_iterable<L>();
}

#endif // !BOOST_HANA_TEST_TEST_ISO_LIST_HPP
