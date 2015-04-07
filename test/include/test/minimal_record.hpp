/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_MINIMAL_RECORD_HPP
#define BOOST_HANA_TEST_TEST_MINIMAL_RECORD_HPP

#include <boost/hana/record.hpp>

#include <boost/hana/config.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <laws/base.hpp>


namespace boost { namespace hana {
    namespace test {
        struct MinimalRecord;

        ct_eq<292138> member1{};
        ct_eq<292139> member2{};

        template <typename X, typename Y>
        struct _record {
            X member1;
            Y member2;
            struct hana { using datatype = MinimalRecord; };
        };

        constexpr detail::create<_record> minimal_record{};
    }

    template <>
    struct members_impl<test::MinimalRecord> {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto apply() {
            return make<Tuple>(
                make<Pair>(test::member1, [](auto&& u) {
                    return static_cast<decltype(u)&&>(u).member1;
                }),
                make<Pair>(test::member2, [](auto&& u) {
                    return static_cast<decltype(u)&&>(u).member2;
                })
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_MINIMAL_RECORD_HPP
