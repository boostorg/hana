/*!
@file
Defines `boost::hana::Searchable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_SEARCHABLE_HPP
#define BOOST_HANA_CONCEPT_SEARCHABLE_HPP

#include <boost/hana/fwd/concept/searchable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/concept/logical.hpp>
#include <boost/hana/optional.hpp>

#include <boost/hana/all.hpp>
#include <boost/hana/all_of.hpp>
#include <boost/hana/any.hpp>
#include <boost/hana/any_of.hpp>
#include <boost/hana/at_key.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/find_if.hpp>
#include <boost/hana/is_subset.hpp>
#include <boost/hana/none.hpp>
#include <boost/hana/none_of.hpp>

#include <cstddef>
#include <type_traits>


namespace boost { namespace hana {
    template <typename S>
    struct models_impl<Searchable, S>
        : _integral_constant<bool,
            !is_default<any_of_impl<S>>::value &&
            !is_default<find_if_impl<S>>::value
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_SEARCHABLE_HPP
