/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LOGICAL_MINIMAL_LOGICAL_HPP
#define BOOST_HANA_TEST_LOGICAL_MINIMAL_LOGICAL_HPP

#include <boost/hana/logical.hpp>


struct MinimalLogical;

template <bool b>
struct _logical {
    using hana_datatype = MinimalLogical;
    explicit constexpr operator bool() const { return b; }
};

// required for the tests
template <bool b1, bool b2>
constexpr bool operator==(_logical<b1>, _logical<b2>)
{ return b1 == b2; }

template <bool b>
constexpr _logical<b> logical{};


namespace boost { namespace hana {
    template <>
    struct Logical::instance<MinimalLogical> : Logical::mcd {
        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(_logical<true>, Then t, Else)
        { return t([](auto x) { return x; }); }

        template <typename Then, typename Else>
        static constexpr auto eval_if_impl(_logical<false>, Then, Else e)
        { return e([](auto x) { return x; }); }

        template <bool b>
        static constexpr auto not_impl(_logical<b>)
        { return logical<!b>; }
    };
}}

#endif // !BOOST_HANA_TEST_LOGICAL_MINIMAL_LOGICAL_HPP
