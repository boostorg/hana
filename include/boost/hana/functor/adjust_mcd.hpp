/*!
@file
Defines `boost::hana::Functor::adjust_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_ADJUST_MCD_HPP
#define BOOST_HANA_FUNCTOR_ADJUST_MCD_HPP

#include <boost/hana/functor/functor.hpp>

#include <boost/hana/bool.hpp>


namespace boost { namespace hana {
    //! Minimal complete definition: `adjust`
    struct Functor::adjust_mcd : functor_detail::common {
        template <typename F, typename Functor_>
        static constexpr auto fmap_impl(F f, Functor_ functor)
        { return adjust([](auto) { return true_; }, f, functor); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_ADJUST_MCD_HPP
