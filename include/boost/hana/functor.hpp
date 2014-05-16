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

namespace boost { namespace hana {
    //! @ingroup typeclasses
    template <typename T>
    struct Functor;

    constexpr struct _fmap {
        template <typename F, typename Functor_>
        constexpr auto operator()(F f, Functor_ functor) const
        { return Functor<Functor_>::fmap_impl(f, functor); }
    } fmap{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_HPP
