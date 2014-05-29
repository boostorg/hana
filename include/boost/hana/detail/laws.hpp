/*!
 * @file
 * Defines `boost::hana::detail::laws`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_LAWS_HPP
#define BOOST_HANA_DETAIL_LAWS_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana { namespace detail {
    //! @ingroup details
    template <template <typename ...> class Typeclass>
    BOOST_HANA_CONSTEXPR_LAMBDA auto laws = [] { };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto laws<Functor> = [](auto functor, auto f, auto g) {
        return fmap(id, functor) == functor &&
               fmap(compose(f, g), functor) == fmap(f, fmap(g, functor));
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_LAWS_HPP
