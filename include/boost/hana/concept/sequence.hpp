/*!
@file
Defines `boost::hana::Sequence`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_SEQUENCE_HPP
#define BOOST_HANA_CONCEPT_SEQUENCE_HPP

#include <boost/hana/fwd/concept/sequence.hpp>

#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    namespace detail {
        template <typename S, typename Tag = typename tag_of<S>::type>
        struct sequence_dispatch {
            static constexpr bool value = Sequence<Tag>::value;
        };

        template <typename S>
        struct sequence_dispatch<S, S> {
            static constexpr bool value = false;
        };
    }

    //! @cond
    template <typename S, bool condition>
    struct Sequence<S, when<condition>>
        : detail::sequence_dispatch<S>
    { };
    //! @endcond
}}

#endif // !BOOST_HANA_CONCEPT_SEQUENCE_HPP
