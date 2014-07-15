/*!
@file
Defines `boost::hana::detail::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_LAWS_HPP
#define BOOST_HANA_DETAIL_LAWS_HPP

#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana { namespace detail {
    template <typename Typeclass>
    BOOST_HANA_CONSTEXPR_LAMBDA auto laws = [] { };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto laws<Functor> = [](auto functor, auto f, auto g) {
        return and_(
            equal(fmap(id, functor), functor),
            equal(fmap(compose(f, g), functor), fmap(f, fmap(g, functor)))
        );
    };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto laws<Monad> = [](auto monad, auto a, auto f, auto g) {
        auto lift_ = lift<datatype_t<decltype(monad)>>;
        return and_(
            equal(bind(lift_(a), f), f(a)),
            equal(bind(monad, lift_), monad),
            equal(bind(monad, [=](auto x) { return bind(f(x), g); }), bind(bind(monad, f), g)),
            equal(fmap(f, monad), bind(monad, compose(lift_, f)))
        );
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_LAWS_HPP
