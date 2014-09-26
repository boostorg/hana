/*!
@file
Defines `boost::hana::detail::variadic::for_each`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_VARIADIC_FOR_EACH_HPP
#define BOOST_HANA_DETAIL_VARIADIC_FOR_EACH_HPP

#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana { namespace detail { namespace variadic {
    template <typename F, typename ...Xs>
    constexpr void for_each(F&& f, Xs&& ...xs) {
        using swallow = int[];
        (void)swallow{1,
            (f(detail::std::forward<Xs>(xs)), void(), 1)...
        };
    }
}}}} // end namespace boost::hana::detail::variadic

#endif // !BOOST_HANA_DETAIL_VARIADIC_FOR_EACH_HPP
