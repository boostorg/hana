/*!
@file
Forward declares `boost::hana::ext::boost::fusion::Vector`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_EXT_BOOST_FUSION_VECTOR_HPP
#define BOOST_HANA_FWD_EXT_BOOST_FUSION_VECTOR_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/is_same.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/support/tag_of.hpp>


namespace boost { namespace hana {
    namespace ext { namespace boost { namespace fusion {
        struct Vector;
    }}}

    template <typename T>
    struct datatype<T, when<
        detail::std::is_same<
            typename ::boost::fusion::traits::tag_of<T>::type,
            ::boost::fusion::traits::tag_of<
                ::boost::fusion::vector<>
            >::type
        >::value
    >> {
        using type = ext::boost::fusion::Vector;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_EXT_BOOST_FUSION_VECTOR_HPP
