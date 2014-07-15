/*!
@file
Defines `boost::hana::MonadZero`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_ZERO_HPP
#define BOOST_HANA_MONAD_ZERO_HPP

#include <boost/hana/applicative/applicative.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! `Monad`s with a neutral element.
    //!
    //! ## Minimal complete definition
    //! `zero`
    //!
    //! ## Laws
    //! ...
    struct MonadZero {
        BOOST_HANA_TYPECLASS(MonadZero);
        template <typename M>
        struct mcd;
    };

    //! ...
    //! @method{MonadZero}
    template <typename M>
    constexpr auto zero = MonadZero<M>::zero_impl();

    //! ...
    //! @method{MonadZero}
    //!
    //! ### Example
    //! ...
    BOOST_HANA_CONSTEXPR_LAMBDA auto filter = [](auto predicate, auto monad) {
        return MonadZero::instance<datatype_t<decltype(monad)>>::filter_impl(predicate, monad);
    };

    template <typename M>
    struct MonadZero::mcd {
        template <typename Pred, typename Monad>
        static constexpr auto filter_impl(Pred pred, Monad m) {
            return bind(m, [=](auto x) {
                return eval_if(pred(x),
                    [=](auto _) { return _(lift<M>)(x); },
                    [=](auto) { return zero<M>; }
                );
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_ZERO_HPP
