/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LOGICAL_MINIMAL_LOGICAL_HPP
#define BOOST_HANA_TEST_LOGICAL_MINIMAL_LOGICAL_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/logical.hpp>


struct MinimalLogical;

template <bool b>
struct _logical {
    using hana_datatype = MinimalLogical;
    constexpr bool operator!() const { return !b; }
};

template <bool b>
constexpr _logical<b> logical{};


namespace boost { namespace hana {
    template <>
    struct Logical<MinimalLogical>
        : defaults<Logical>::template with<MinimalLogical>
    {
        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(_logical<true>, Then t, Else)
        { return t([](auto x) { return x; }); }

        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(_logical<false>, Then, Else e)
        { return e([](auto x) { return x; }); }
    };
}}

#endif // !BOOST_HANA_TEST_LOGICAL_MINIMAL_LOGICAL_HPP
