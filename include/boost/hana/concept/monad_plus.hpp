/*!
@file
Defines `boost::hana::MonadPlus`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_MONAD_PLUS_HPP
#define BOOST_HANA_CONCEPT_MONAD_PLUS_HPP

#include <boost/hana/fwd/concept/monad_plus.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>

#include <boost/hana/append.hpp>
#include <boost/hana/concat.hpp>
#include <boost/hana/cycle.hpp>
#include <boost/hana/empty.hpp>
#include <boost/hana/filter.hpp>
#include <boost/hana/prefix.hpp>
#include <boost/hana/prepend.hpp>
#include <boost/hana/remove.hpp>
#include <boost/hana/remove_if.hpp>
#include <boost/hana/repeat.hpp>
#include <boost/hana/suffix.hpp>


namespace boost { namespace hana {
    template <typename M>
    struct models_impl<MonadPlus, M> {
        static constexpr bool value = !is_default<concat_impl<M>>::value &&
                                      !is_default<empty_impl<M>>::value;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_MONAD_PLUS_HPP
