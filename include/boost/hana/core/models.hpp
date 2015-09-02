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

#include <boost/hana/bool.hpp>
#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename Concept, typename DataType, bool condition>
    struct models_impl<Concept, DataType, when<condition>>
        : integral_constant<bool, false>
    { };

    template <typename Concept, typename T>
    struct _models<Concept, T>
        : integral_constant<bool,
            models_impl<Concept, typename hana::tag_of<T>::type>::value
        >
    { };

    template <typename Concept>
    struct _models<Concept> {
        template <typename T>
        constexpr auto operator()(T const&) const
        { return models<Concept, T>; }
    };
    //! @endcond
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_MODELS_HPP
