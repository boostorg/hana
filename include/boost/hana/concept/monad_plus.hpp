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

#include <boost/hana/concept/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/concept/logical.hpp>
#include <boost/hana/concept/monad.hpp>

#include <boost/hana/concat.hpp>
#include <boost/hana/filter.hpp>
#include <boost/hana/append.hpp>
#include <boost/hana/prepend.hpp>
#include <boost/hana/empty.hpp>
#include <boost/hana/cycle.hpp>
#include <boost/hana/repeat.hpp>
#include <boost/hana/remove_if.hpp>
#include <boost/hana/remove.hpp>
#include <boost/hana/prefix.hpp>
#include <boost/hana/suffix.hpp>

#include <cstddef>


namespace boost { namespace hana {
    template <typename M>
    struct models_impl<MonadPlus, M>
        : _integral_constant<bool,
            !is_default<concat_impl<M>>{}() &&
            !is_default<empty_impl<M>>{}()
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_MONAD_PLUS_HPP
