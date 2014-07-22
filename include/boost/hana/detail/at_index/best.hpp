/*!
@file
Defines `boost::hana::detail::at_index::best`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_AT_INDEX_BEST_HPP
#define BOOST_HANA_DETAIL_AT_INDEX_BEST_HPP

#include <boost/hana/detail/at_index/overload_resolution.hpp>
#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana { namespace detail { namespace at_index {

#if 0 // triggers an ICE on Clang 3.5
    template <std::size_t n>
    BOOST_HANA_CONSTEXPR_LAMBDA auto best = overload_resolution<n>;
#else
    template <std::size_t n>
    struct _best {
        template <typename ...Xs>
        constexpr auto operator()(Xs ...xs) const
        { return overload_resolution<n>(xs...); }
    };

    template <std::size_t n>
    constexpr _best<n> best{};
#endif
}}}}

#endif // !BOOST_HANA_DETAIL_AT_INDEX_BEST_HPP
