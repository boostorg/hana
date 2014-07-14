/*!
@file
Defines `boost::hana::Monad`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_HPP
#define BOOST_HANA_MONAD_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup typeclasses
    `Monad`s are `Applicative`s with the ability to flatten values that were
    lifted more than once.

    --------------------------------------------------------------------------

    ## Laws
    Instances of `Monad` must satisfy the following laws:
    @code
        bind(lift<M>(x), f) == f(x)
        bind(m, lift<M>) == m
        bind(m, [](auto x){ return bind(f(x), g); }) == bind(bind(m, f), g)
    @endcode
     */
    struct Monad {
        BOOST_HANA_TYPECLASS(Monad);
        struct bind_mcd;
        struct flatten_mcd;
    };

    //! Apply a function returning a monad to the value(s) inside a monad.
    //! @method{Monad}
    BOOST_HANA_CONSTEXPR_LAMBDA auto bind = [](auto monad, auto f) {
        return Monad::instance<datatype_t<decltype(monad)>>::bind_impl(monad, f);
    };

    //! Flatten two levels of monadic wrapping into a single level.
    //! @method{Monad}
    BOOST_HANA_CONSTEXPR_LAMBDA auto flatten = [](auto monad) {
        return Monad::instance<datatype_t<decltype(monad)>>::flatten_impl(monad);
    };

    //! Minimal complete definition: `bind`
    struct Monad::bind_mcd {
        template <typename M>
        static constexpr auto flatten_impl(M monad)
        { return bind(monad, [](auto x) { return x; }); }
    };

    //! Minimal complete definition: `flatten`
    struct Monad::flatten_mcd {
        template <typename M, typename F>
        static constexpr auto bind_impl(M monad, F f)
        { return flatten(fmap(f, monad)); }
    };

    namespace operators {
        //! Equivalent to `bind`.
        //! @method{boost::hana::Monad}
        //!
        //! @note
        //! This was preferred over `>>=` because `>>=` is right associative,
        //! which makes it impossible to chain computations.
        template <typename M, typename F>
        constexpr auto operator>>(M m, F f)
        { return bind(m, f); }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_HPP
