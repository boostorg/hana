/*!
@file
Defines `boost::hana::Monad`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_HPP
#define BOOST_HANA_MONAD_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    template <typename M>
    struct Monad;

    template <typename M>
    BOOST_HANA_CONSTEXPR_LAMBDA auto unit = [](auto x) {
        return Monad<M>::unit_impl(x);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto join = [](auto monad) {
        return Monad<datatype_t<decltype(monad)>>::join_impl(monad);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto bind = [](auto monad, auto f) {
        return Monad<datatype_t<decltype(monad)>>::bind_impl(monad, f);
    };

    template <>
    struct defaults<Monad> {
        template <typename Monad_>
        static constexpr auto join_impl(Monad_ monad)
        { return bind(monad, id); }

        template <typename Monad_, typename F>
        static constexpr auto bind_impl(Monad_ monad, F f)
        { return join(fmap(f, monad)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_HPP
