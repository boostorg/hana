/*!
@file
Defines `boost::hana::detail::has_[nontrivial_]common_embedding`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_HAS_COMMON_EMBEDDING_HPP
#define BOOST_HANA_DETAIL_HAS_COMMON_EMBEDDING_HPP

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/models.hpp>

#include <type_traits>


namespace boost { namespace hana { namespace detail {
    //! @ingroup group-details
    //! Returns whether `T` and `U` both have an embedding into a
    //! common type.
    //!
    //! If `T` and `U` do not have a common-type, a SFINAE-friendly failure
    //! happens when instantiating this alias.
    template <typename Concept, typename T, typename U>
    using has_common_embedding = std::integral_constant<bool,
        _models<Concept, T>{}() &&
        _models<Concept, U>{}() &&
        _models<Concept, typename common<T, U>::type>{}() &&
        is_embedded<T, typename common<T, U>::type>{} &&
        is_embedded<U, typename common<T, U>::type>{}
    >;

    //! @ingroup group-details
    //! Returns whether `T` and `U` are distinct and both have an embedding
    //! into a common type.
    //!
    //! If `T` and `U` do not have a common-type, a SFINAE-friendly failure
    //! happens when instantiating this alias.
    template <typename Concept, typename T, typename U>
    using has_nontrivial_common_embedding = std::integral_constant<bool,
        !std::is_same<T, U>{} &&
        has_common_embedding<Concept, T, U>{}
    >;
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_HAS_COMMON_EMBEDDING_HPP
