/*!
@file
Defines `boost::hana::Monad`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_MONAD_HPP
#define BOOST_HANA_CONCEPT_MONAD_HPP

#include <boost/hana/fwd/concept/monad.hpp>

#include <boost/hana/chain.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/flatten.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    template <typename M>
    struct models_impl<Monad, M> {
        static constexpr bool value = !is_default<flatten_impl<M>>::value ||
                                      !is_default<chain_impl<M>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_MONAD_HPP
