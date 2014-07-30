/*!
@file
Defines `boost::hana::Monad::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_LAWS_HPP
#define BOOST_HANA_MONAD_LAWS_HPP

#include <boost/hana/monad/monad.hpp>

#include <boost/hana/applicative/applicative.hpp>
#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/logical/logical.hpp>


namespace boost { namespace hana {
    struct Monad::laws {
        template <typename Monad_, typename A, typename F, typename G>
        static constexpr auto check(Monad_ monad, A a, F f, G g) {
            auto lift_ = lift<datatype_t<decltype(monad)>>;
            return and_(
                equal(bind(lift_(a), f), f(a)),
                equal(bind(monad, lift_), monad),
                equal(bind(monad, [=](auto x) { return bind(f(x), g); }), bind(bind(monad, f), g)),
                equal(fmap(f, monad), bind(monad, compose(lift_, f)))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_LAWS_HPP
