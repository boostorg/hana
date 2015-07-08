/*!
@file
Defines `boost::hana::ext::boost::fusion::Vector`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_FUSION_VECTOR_HPP
#define BOOST_HANA_EXT_BOOST_FUSION_VECTOR_HPP

#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/ext/boost/fusion/detail/common.hpp>

#include <boost/fusion/algorithm/transformation/pop_front.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/support/tag_of.hpp>

#include <type_traits>


namespace boost { namespace hana {
    namespace ext { namespace boost { namespace fusion {
        struct Vector;
    }}}

    template <typename T>
    struct datatype<T, when<
        std::is_same<
            typename ::boost::fusion::traits::tag_of<T>::type,
            ::boost::fusion::traits::tag_of<
                ::boost::fusion::vector<>
            >::type
        >::value
    >> {
        using type = ext::boost::fusion::Vector;
    };

    namespace detail {
        template <>
        struct is_fusion_sequence<ext::boost::fusion::Vector>
            : std::true_type
        { };
    }

    //////////////////////////////////////////////////////////////////////////
    // Iterable (the rest is in detail/common.hpp)
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct tail_impl<ext::boost::fusion::Vector> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return ::boost::fusion::as_vector(
                ::boost::fusion::pop_front(static_cast<Xs&&>(xs)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<ext::boost::fusion::Vector> {
        template <typename ...Xs>
        static constexpr auto apply(Xs&& ...xs) {
            return ::boost::fusion::make_vector(static_cast<Xs&&>(xs)...);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_FUSION_VECTOR_HPP
