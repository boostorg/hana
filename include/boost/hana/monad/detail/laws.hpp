/*!
@file
Defines `boost::hana::Monad::laws`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_DETAIL_LAWS_HPP
#define BOOST_HANA_MONAD_DETAIL_LAWS_HPP

#include <boost/hana/monad/monad.hpp>

#include <boost/hana/applicative/applicative.hpp>
#include <boost/hana/comparable/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/searchable/searchable.hpp>


namespace boost { namespace hana {
    struct Monad::laws {
        template <typename Monads, typename Xs, typename Fs, typename Gs>
        static constexpr auto check2(Monads ms, Xs xs, Fs fs, Gs gs) {
            return all(ms, [=](auto m) {
                return all(xs, [=](auto x) {
                    return all(fs, [=](auto f) {
                        return all(gs, [=](auto g) {
                            using M = datatype_t<decltype(m)>;
                            return and_(
                                equal(bind(lift<M>(x), f), f(x)),
                                equal(bind(m, lift<M>), m),
                                equal(
                                    bind(m, [=](auto x) { return bind(f(x), g); }),
                                    bind(bind(m, f), g)
                                ),
                                equal(fmap(f, m), bind(m, compose(lift<M>, f)))
                            );
                        });
                    });
                });
            });
        }

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

#endif // !BOOST_HANA_MONAD_DETAIL_LAWS_HPP
