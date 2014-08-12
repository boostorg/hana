/*!
@file
Defines `boost::hana::Monad::bind_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_BIND_MCD_HPP
#define BOOST_HANA_MONAD_BIND_MCD_HPP

#include <boost/hana/monad/detail/common.hpp>
#include <boost/hana/monad/monad.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `bind`
    template <typename M>
    struct Monad::bind_mcd : monad_detail::common<M> {
        template <typename MM>
        static constexpr auto flatten_impl(MM monad)
        { return bind(monad, [](auto x) { return x; }); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_BIND_MCD_HPP
