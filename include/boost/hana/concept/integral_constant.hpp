/*!
@file
Defines `boost::hana::IntegralConstant`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_INTEGRAL_CONSTANT_HPP
#define BOOST_HANA_CONCEPT_INTEGRAL_CONSTANT_HPP

#include <boost/hana/fwd/concept/integral_constant.hpp>

#include <boost/hana/core/tag_of.hpp>


namespace boost { namespace hana {
    namespace detail {
        template <typename C, typename Tag = typename tag_of<C>::type>
        struct integral_constant_dispatch {
            static constexpr bool value = IntegralConstant<Tag>::value;
        };

        template <typename C>
        struct integral_constant_dispatch<C, C> {
            static constexpr bool value = false;
        };
    }

    //! @cond
    template <typename C>
    struct IntegralConstant
        : detail::integral_constant_dispatch<C>
    { };
    //! @endcond
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_INTEGRAL_CONSTANT_HPP
