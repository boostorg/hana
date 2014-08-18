/*!
@file
Defines the instance of `boost::hana::Iterable` for `boost::hana::BoostMplVector`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_VECTOR_ITERABLE_HPP
#define BOOST_HANA_EXT_BOOST_MPL_VECTOR_ITERABLE_HPP

#include <boost/hana/ext/boost/mpl/integral_c.hpp>
#include <boost/hana/ext/boost/mpl/vector/vector.hpp>
#include <boost/hana/iterable/mcd.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/empty.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>


namespace boost { namespace hana {
    //! `Iterable` instance for Boost.MPL vectors.
    //!
    //! ### Example
    //! @include example/ext/boost/mpl/vector/iterable.cpp
    template <>
    struct Iterable::instance<BoostMplVector> : Iterable::mcd {
        template <typename xs>
        static constexpr auto head_impl(xs)
        { return type<typename boost::mpl::front<xs>::type>; }

        template <typename xs>
        static constexpr auto tail_impl(xs)
        { return typename boost::mpl::pop_front<xs>::type{}; }

        template <typename xs>
        static constexpr auto is_empty_impl(xs)
        { return typename boost::mpl::empty<xs>::type{}; }
    };
}}

#endif // !BOOST_HANA_EXT_BOOST_MPL_VECTOR_ITERABLE_HPP
