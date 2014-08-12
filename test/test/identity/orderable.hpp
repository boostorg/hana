/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_IDENTITY_ORDERABLE_HPP
#define BOOST_HANA_TEST_TEST_IDENTITY_ORDERABLE_HPP

#include <boost/hana/orderable/less_mcd.hpp>
#include <test/identity/identity.hpp>


namespace boost { namespace hana {
    template <>
    struct Orderable::instance<test::Identity, test::Identity>
        : Orderable::less_mcd
    {
        template <typename Id1, typename Id2>
        static constexpr auto less_impl(Id1 x, Id2 y) {
            return less(x.value, y.value);
        }
    };
}}

#endif //! BOOST_HANA_TEST_TEST_IDENTITY_ORDERABLE_HPP
