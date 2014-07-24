/*!
@file
Forward declares `boost::hana::Monad`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_MONAD_HPP
#define BOOST_HANA_MONAD_MONAD_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! `Monad`s are `Applicative`s with the ability to flatten values that
    //! were lifted more than once.
    //!
    //! @todo
    //! Consider adding a function to tap inside a monadic chain.
    struct Monad {
        BOOST_HANA_TYPECLASS(Monad);
        struct bind_mcd;
        struct flatten_mcd;
        struct laws;
    };

    //! Apply a function returning a monad to the value(s) inside a monad.
    //! @method{Monad}
    BOOST_HANA_CONSTEXPR_LAMBDA auto bind = [](auto monad, auto f) {
        return Monad::instance<
            datatype_t<decltype(monad)>
        >::bind_impl(monad, f);
    };

    //! Sequentially compose two monadic actions, discarding any value
    //! produced by the first.
    //! @method{Monad}
    //!
    //! This is equivalent to Haskell's `>>`.
    BOOST_HANA_CONSTEXPR_LAMBDA auto then = [](auto monadf, auto monadg) {
        return Monad::instance<
            datatype_t<decltype(monadf)>
        >::then_impl(monadf, monadg);
    };

    //! Flatten two levels of monadic wrapping into a single level.
    //! @method{Monad}
    BOOST_HANA_CONSTEXPR_LAMBDA auto flatten = [](auto monad) {
        return Monad::instance<
            datatype_t<decltype(monad)>
        >::flatten_impl(monad);
    };

    namespace operators {
        //! Equivalent to `bind`.
        //! @method{boost::hana::Monad}
        //!
        //! @note
        //! This was preferred over `>>=` because `>>=` is right associative,
        //! which makes it impossible to chain computations.
        template <typename M, typename F>
        constexpr auto operator|(M m, F f)
        { return bind(m, f); }
    }

    namespace monad_detail {
        struct common {
            template <typename M1, typename M2>
            static constexpr auto then_impl(M1 m1, M2 m2)
            { return bind(m1, [=](auto) { return m2; }); }
        };
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_MONAD_HPP
