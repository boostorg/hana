/*!
@file
Defines `boost::hana::Metafunction`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_METAFUNCTION_HPP
#define BOOST_HANA_CONCEPT_METAFUNCTION_HPP

#include <boost/hana/fwd/concept/metafunction.hpp>

#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    template <typename F>
    struct models_impl<Metafunction, F> {
        static constexpr bool value = false;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_METAFUNCTION_HPP
