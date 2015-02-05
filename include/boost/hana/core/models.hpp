/*!
@file
Defines `boost::hana::models`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_MODELS_HPP
#define BOOST_HANA_CORE_MODELS_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //! Temporary helper to help split the methods.
    //!
    //! Basically, the plan is that `is_a` will also check whether this
    //! helper is specialized. If it is, then `is_a` will be true.
    //!
    //! This way, we can start splitting methods without breaking the world.
    template <typename Concept, typename = void>
    struct models : models<Concept, when<true>> { };

    template <typename Concept, bool condition>
    struct models<Concept, when<condition>> : detail::std::false_type { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_MODELS_HPP
