/*!
@file
Defines `boost::hana::Monad::flatten_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_FLATTEN_MCD_HPP
#define BOOST_HANA_MONAD_FLATTEN_MCD_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/monad/detail/common.hpp>
#include <boost/hana/monad/monad.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `flatten`
    template <typename M>
    struct Monad::flatten_mcd : monad_detail::common<M> {
        template <typename Mon, typename F>
        static constexpr decltype(auto) bind_impl(Mon&& monad, F&& f) {
            return flatten(
                fmap(
                    detail::std::forward<Mon>(monad),
                    detail::std::forward<F>(f)
                )
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_FLATTEN_MCD_HPP
