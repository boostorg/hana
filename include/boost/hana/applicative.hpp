/*!
@file
Defines `boost::hana::Applicative`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_APPLICATIVE_HPP
#define BOOST_HANA_APPLICATIVE_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup typeclasses
    `Applicative`s are `Functor`s with the ability to lift values and combine
    computations.

    --------------------------------------------------------------------------

    ## Minimal complete definition
    `lift` and `ap`

    --------------------------------------------------------------------------

    ## Laws
    For any `Applicative` `A`, the following laws must be satisfied:
    @code
        ap(lift<A>(id), u) == u                                  // identity
        ap(ap(ap(lift<A>(compose), u), v), w) == ap(u, ap(v, w)) // composition
        ap(lift<A>(f), lift<A>(x)) == lift<A>(f(x))              // homomorphism
        ap(u, lift<A>(y)) == ap(lift<A>(apply(y)), u)            // interchange
    @endcode

    As a consequence of these laws, the `Functor` instance for `A` will
    satisfy
    @code
        fmap(f, x) == ap(lift<A>(f), x)
    @endcode
     */
    struct Applicative : typeclass<Applicative> {
        struct mcd { };
    };

    //! Lifted application.
    //! @method{Applicative}
    //!
    //! ### Example
    //! @snippet example/list/applicative/overview.cpp main
    //!
    //! @todo
    //! Should this be a variadic function?
    BOOST_HANA_CONSTEXPR_LAMBDA auto ap = [](auto f, auto x) {
        return Applicative::instance<datatype_t<decltype(f)>>::ap_impl(f, x);
    };

    namespace applicative_detail {
        template <typename A>
        struct lift {
            template <typename X>
            constexpr auto operator()(X x) const
            { return Applicative::instance<A>::lift_impl(x); }
        };
    }

    //! Lift a value into the functor.
    //! @method{Applicative}
    template <typename A>
    constexpr applicative_detail::lift<A> lift{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_APPLICATIVE_HPP
