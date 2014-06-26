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
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    BOOST_HANA_TYPECLASS_BOILERPLATE(struct Monad)

    /*!
    @ingroup typeclasses
    `Monad`s are `Applicative`s with the ability to flatten values that were
    lifted more than once.

    --------------------------------------------------------------------------

    ## Minimal complete definition
    `join` or `bind`

    --------------------------------------------------------------------------

    ## Laws
    Instances of `Monad` must satisfy the following laws:
    @code
        bind(lift<M>(x), f) == f(x)
        bind(m, lift<M>) == m
        bind(m, [](auto x){ return bind(f(x), g); }) == bind(bind(m, f), g)
    @endcode
     */
    struct Monad : typeclass<Monad> {
        struct bind_mcd;
        struct join_mcd;
    };

    //! Apply a function returning a monad to the value(s) inside a monad.
    //! @method{Monad}
    BOOST_HANA_CONSTEXPR_LAMBDA auto bind = [](auto monad, auto f) {
        return Monad::instance<datatype_t<decltype(monad)>>::bind_impl(monad, f);
    };

    //! Flatten two levels of monadic wrapping into a single level.
    //! @method{Monad}
    BOOST_HANA_CONSTEXPR_LAMBDA auto join = [](auto monad) {
        return Monad::instance<datatype_t<decltype(monad)>>::join_impl(monad);
    };

    struct Monad::bind_mcd {
        template <typename M>
        static constexpr auto join_impl(M monad)
        { return bind(monad, [](auto x) { return x; }); }
    };

    struct Monad::join_mcd {
        template <typename M, typename F>
        static constexpr auto bind_impl(M monad, F f)
        { return join(fmap(f, monad)); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_HPP
