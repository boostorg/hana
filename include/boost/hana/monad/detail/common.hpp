/*!
@file
Defines a common part of the mcd for `Monad`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_DETAIL_COMMON_HPP
#define BOOST_HANA_MONAD_DETAIL_COMMON_HPP

#include <boost/hana/applicative/applicative.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/monad/monad.hpp>


namespace boost { namespace hana { namespace monad_detail {
    template <typename M>
    struct common {
        template <typename M1, typename M2>
        static constexpr decltype(auto) then_impl(M1&& m1, M2&& m2) {
            return bind(
                detail::std::forward<M1>(m1),
                always(detail::std::forward<M2>(m2))
            );
        }

        template <typename F>
        static constexpr auto tap_impl(F&& f) {
            return [f(detail::std::forward<F>(f))](auto&& x) -> decltype(auto) {
                f(x);
                return lift<M>(detail::std::forward<decltype(x)>(x));
            };
        }
    };
}}} // end namespace boost::hana::monad_detail

#endif // !BOOST_HANA_MONAD_DETAIL_COMMON_HPP
