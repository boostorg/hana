/*!
@file
Defines `boost::hana::models`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_MODELS_HPP
#define BOOST_HANA_CORE_MODELS_HPP

#include <boost/hana/fwd/core/models.hpp>

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename Concept, typename>
    struct models : models<Concept, when<true>> { };

    template <typename Concept, bool condition>
    struct models<Concept, when<condition>>
        : detail::std::false_type
    { };
    //! @endcond
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_MODELS_HPP
