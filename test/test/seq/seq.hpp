/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_SEQ_SEQ_HPP
#define BOOST_HANA_TEST_TEST_SEQ_SEQ_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/iterable.hpp>


namespace test {
    //! @todo Find a way not to enable operators here, but to only enable them
    //! in the unit tests where they are needed.
    struct Seq {
        struct hana_enabled_operators : boost::hana::Iterable { };
    };

    template <typename Storage>
    struct seq_type : boost::hana::operators::Iterable_ops<seq_type<Storage>> {
        explicit constexpr seq_type(Storage s) : storage(s) { }
        Storage storage;
        using hana_datatype = Seq;
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto seq = [](auto ...xs) {
        auto storage = [=](auto f) { return f(xs...); };
        return seq_type<decltype(storage)>(storage);
    };
}

#endif //! BOOST_HANA_TEST_TEST_SEQ_SEQ_HPP
