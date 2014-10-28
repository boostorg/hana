/*!
@file
Forward declares `boost::hana::ext::boost::fusion::sequence`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_BOOST_FUSION_SEQUENCE_HPP
#define BOOST_HANA_FWD_EXT_BOOST_FUSION_SEQUENCE_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>

#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <type_traits>


namespace boost { namespace hana {
    namespace fusion_detail {
        template <typename T, bool = ::boost::fusion::traits::is_sequence<T>::value>
        struct is_nonassociative_forward_sequence {
            static constexpr bool value = false;
        };

        template <typename T>
        struct is_nonassociative_forward_sequence<T, true> {
            static constexpr bool value = ! ::std::is_base_of<
                ::boost::fusion::associative_tag,
                typename ::boost::fusion::traits::category_of<T>::type
            >::value;
        };
    }

    namespace ext { namespace boost { namespace fusion {
        struct Sequence;
    }}}

    template <typename T>
    struct datatype<T, when<
        fusion_detail::is_nonassociative_forward_sequence<T>::value
    >> {
        using type = ext::boost::fusion::Sequence;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_BOOST_FUSION_SEQUENCE_HPP
