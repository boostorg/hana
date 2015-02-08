/*!
@file
Defines `boost::hana::detail::has_common_embedding`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_HAS_COMMON_EMBEDDING_HPP
#define BOOST_HANA_DETAIL_HAS_COMMON_EMBEDDING_HPP

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>


namespace boost { namespace hana { namespace detail {
    //! @ingroup group-details
    //! Returns whether `T` and `U` are distinct and both have an embedding
    //! into their common type.
    //!
    //! If `T` and `U` do not have a common-type, a SFINAE-friendly failure
    //! happens when instantiating this alias.
    template <typename Concept, typename T, typename U>
    using has_common_embedding = detail::std::integral_constant<bool,
        !detail::std::is_same<T, U>{} &&
        models<Concept(T)>{} &&
        models<Concept(U)>{} &&
        models<Concept(typename common<T, U>::type)>{} &&
        is_embedded<T, typename common<T, U>::type>{} &&
        is_embedded<U, typename common<T, U>::type>{}
    >;
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_HAS_COMMON_EMBEDDING_HPP
