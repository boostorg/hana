/*!
@file
Defines the instance of `boost::hana::List` for `boost::hana::BoostMplVector`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_VECTOR_LIST_HPP
#define BOOST_HANA_EXT_BOOST_MPL_VECTOR_LIST_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/ext/boost/mpl/vector/vector.hpp>
#include <boost/hana/list/mcd.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/push_front.hpp>
#include <boost/mpl/vector/vector0.hpp>


namespace boost { namespace hana {
    //! `List` instance for Boost.MPL vectors.
    //!
    //! Note that since Boost.MPL vectors can only hold types,
    //! only `Type`s can be used with `cons`.
    //!
    //! ### Example
    //! @include example/ext/boost/mpl/vector/list.cpp
    template <>
    struct List::instance<BoostMplVector> : List::mcd<BoostMplVector> {
        template <typename x, typename xs>
        static constexpr auto cons_impl(x, xs) {
            static_assert(detail::std::is_same<datatype_t<x>, Type>::value,
            "Only Types may be prepended to a Boost.MPL vector.");

            return typename boost::mpl::push_front<
                xs, typename x::type
            >::type{};
        }

        static constexpr auto nil_impl()
        { return boost::mpl::vector0<>{}; }
    };
}}

#endif // !BOOST_HANA_EXT_BOOST_MPL_VECTOR_LIST_HPP
