/*!
 * @file
 * Defines `boost::hana::Functor`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_HPP
#define BOOST_HANA_FUNCTOR_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    template <typename T>
    struct Functor;

    BOOST_HANA_CONSTEXPR_LAMBDA auto fmap = [](auto f, auto functor) {
        return Functor<decltype(functor)>::fmap_impl(f, functor);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_HPP
