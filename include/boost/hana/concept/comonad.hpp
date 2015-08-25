/*!
@file
Defines `boost::hana::Comonad`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_COMONAD_HPP
#define BOOST_HANA_CONCEPT_COMONAD_HPP

#include <boost/hana/fwd/concept/comonad.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/duplicate.hpp>
#include <boost/hana/extend.hpp>
#include <boost/hana/extract.hpp>
#include <boost/hana/fwd/core/models.hpp>


namespace boost { namespace hana {
    template <typename W>
    struct models_impl<Comonad, W> {
        static constexpr bool value =
            !is_default<extract_impl<W>>::value &&
                (!is_default<duplicate_impl<W>>::value ||
                 !is_default<extend_impl<W>>::value);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_COMONAD_HPP
