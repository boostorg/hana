/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_MINIMAL_RECORD_HPP
#define BOOST_HANA_TEST_TEST_MINIMAL_RECORD_HPP

#include <boost/hana/record.hpp>

#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <test/injection.hpp>


namespace boost { namespace hana {
    namespace test {
        struct MinimalRecord;

        BOOST_HANA_CONSTEXPR_LAMBDA auto member1 = injection([]{})();
        BOOST_HANA_CONSTEXPR_LAMBDA auto member2 = injection([]{})();

        template <typename X, typename Y>
        struct _record {
            X member1;
            Y member2;
            struct hana { using datatype = MinimalRecord; };
        };

        constexpr detail::create<_record> minimal_record{};
    }

    template <>
    struct models<Record(test::MinimalRecord)>
        : detail::std::true_type
    { };

    template <>
    struct members_impl<test::MinimalRecord> {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto apply() {
            return tuple(
                pair(test::member1, [](auto u) { return u.member1; }),
                pair(test::member2, [](auto u) { return u.member2; })
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_MINIMAL_RECORD_HPP
