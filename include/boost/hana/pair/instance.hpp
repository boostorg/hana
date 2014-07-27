/*!
@file
Defines the `boost::hana::Pair` instance.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PAIR_INSTANCE_HPP
#define BOOST_HANA_PAIR_INSTANCE_HPP

#include <boost/hana/pair/pair.hpp>

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/pair/mcd.hpp>


namespace boost { namespace hana {
    namespace pair_detail {
        template <typename First, typename Second, typename = operators::enable>
        struct pair {
            First first;
            Second second;
            using hana_datatype = Pair;
        };
    }

    //! Creates a `Pair` with the given elements.
    //! @relates Pair
    BOOST_HANA_CONSTEXPR_LAMBDA auto pair = [](auto first, auto second) {
        return pair_detail::pair<
            decltype(first), decltype(second)
        >{first, second};
    };

    template <>
    struct Pair::instance<Pair> : Pair::mcd {
        template <typename P>
        static constexpr auto first_impl(P p)
        { return p.first; }

        template <typename P>
        static constexpr auto second_impl(P p)
        { return p.second; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PAIR_INSTANCE_HPP
