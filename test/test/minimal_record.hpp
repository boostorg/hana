/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_MINIMAL_RECORD_HPP
#define BOOST_HANA_TEST_TEST_MINIMAL_RECORD_HPP

#include <boost/hana/record.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/tuple.hpp>

#include <test/injection.hpp>


namespace test {
    template <typename Mcd>
    struct MinimalRecord { };

    template <typename X, typename Y, typename Mcd>
    struct record_type {
        X member1;
        Y member2;
        struct hana { using datatype = MinimalRecord<Mcd>; };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto member1 = injection([]{})();
    BOOST_HANA_CONSTEXPR_LAMBDA auto member2 = injection([]{})();

    template <typename Mcd = boost::hana::Record::mcd>
    BOOST_HANA_CONSTEXPR_LAMBDA auto minimal_record = [](auto x, auto y) {
        return record_type<decltype(x), decltype(y), Mcd>{x, y};
    };
}

namespace boost { namespace hana {
    template <>
    struct Record::instance<test::MinimalRecord<Record::mcd>>
        : Record::mcd
    {
        static BOOST_HANA_CONSTEXPR_LAMBDA auto members_impl() {
            return tuple(
                pair(test::member1, [](auto u) { return u.member1; }),
                pair(test::member2, [](auto u) { return u.member2; })
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_MINIMAL_RECORD_HPP
