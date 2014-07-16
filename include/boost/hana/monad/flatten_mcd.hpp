/*!
@file
Defines `boost::hana::Monad::flatten_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_FLATTEN_MCD_HPP
#define BOOST_HANA_MONAD_FLATTEN_MCD_HPP

#include <boost/hana/monad/monad.hpp>

#include <boost/hana/functor/functor.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `flatten`
    struct Monad::flatten_mcd {
        template <typename M, typename F>
        static constexpr auto bind_impl(M monad, F f)
        { return flatten(fmap(f, monad)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_FLATTEN_MCD_HPP
