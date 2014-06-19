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
    /*!
    @ingroup typeclasses
    `Monad`s are `Functor`s with the ability to wrap values into them
    and flatten values that were wrapped more than once.

    --------------------------------------------------------------------------

    ## Minimal complete definition
    `unit` and (`join` or `bind`)

    --------------------------------------------------------------------------

    ## Laws
    Instances of `Monad` must satisfy the following laws:
    @code
        bind(unit<M>(x), f) == f(x)
        bind(m, unit<M>) == m
        bind(m, [](auto x){ return bind(f(x), g); }) == bind(bind(m, f), g)
    @endcode
     */
    template <typename M, typename Enable = void>
    struct Monad;

    //! Wrap a value into a `Monad`.
    //! @method{Monad}
    template <typename M>
    BOOST_HANA_CONSTEXPR_LAMBDA auto unit = [](auto x) {
        return Monad<M>::unit_impl(x);
    };

    //! Apply a function returning a monad to the value(s) inside a monad.
    //! @method{Monad}
    BOOST_HANA_CONSTEXPR_LAMBDA auto bind = [](auto monad, auto f) {
        return Monad<datatype_t<decltype(monad)>>::bind_impl(monad, f);
    };

    //! Flatten two levels of monadic wrapping into a single level.
    //! @method{Monad}
    BOOST_HANA_CONSTEXPR_LAMBDA auto join = [](auto monad) {
        return Monad<datatype_t<decltype(monad)>>::join_impl(monad);
    };

    template <>
    struct instance<Monad> {
        template <typename M, typename Enable = void>
        struct with { };
    };

    template <>
    struct defaults<Monad> {
        template <typename M, typename Enable = void>
        struct with : defaults<> {
            template <typename Monad_>
            static constexpr auto join_impl(Monad_ monad)
            { return bind(monad, id); }

            template <typename Monad_, typename F>
            static constexpr auto bind_impl(Monad_ monad, F f)
            { return join(fmap(f, monad)); }
        };
    };

    template <typename M, typename Enable>
    struct Monad : instance<Monad>::template with<M> { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_HPP
