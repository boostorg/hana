/*!
@file
Defines `boost::hana::Applicative`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_APPLICATIVE_HPP
#define BOOST_HANA_APPLICATIVE_HPP

#include <boost/hana/detail/left_folds/variadic.hpp>
#include <boost/hana/detail/typeclasses.hpp>
#include <boost/hana/functional.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup typeclasses
    `Applicative`s are `Functor`s with the ability to lift values and combine
    computations.

    --------------------------------------------------------------------------

    ## Laws
    For any `Applicative` `A`, the following laws must be satisfied:
    @code
        ap(lift<A>(id), u) == u                          // identity
        ap(lift<A>(compose), u, v, w) == ap(u, ap(v, w)) // composition
        ap(lift<A>(f), lift<A>(x)) == lift<A>(f(x))      // homomorphism
        ap(u, lift<A>(y)) == ap(lift<A>(apply(y)), u)    // interchange
    @endcode

    As a consequence of these laws, the `Functor` instance for `A` will
    satisfy
    @code
        fmap(f, x) == ap(lift<A>(f), x)
    @endcode

    @todo Write checkable laws and enforce them.
     */
    struct Applicative {
        BOOST_HANA_TYPECLASS(Applicative);

        //! Minimal complete definition : `lift` and `ap`
        struct mcd { };
    };

    namespace applicative_detail {
        template <typename A>
        struct lift {
            template <typename X>
            constexpr auto operator()(X x) const
            { return Applicative::instance<A>::lift_impl(x); }
        };

        struct ap {
            template <typename F, typename X>
            constexpr auto operator()(F f, X x) const {
                return Applicative::instance<datatype_t<F>>::ap_impl(f, x);
            }

            template <typename F, typename ...Xs>
            constexpr auto operator()(F f, Xs ...xs) const {
                static_assert(sizeof...(xs) >= 1,
                "boost::hana::ap must be called with two arguments or more");
                return detail::left_folds::variadic(
                    *this,
                    fmap(curry<sizeof...(xs)>, f),
                    xs...
                );
            }
        };
    }

    //! Lifted application.
    //! @method{Applicative}
    //!
    //! `ap` can be called with two arguments or more. Specifically,
    //! `ap(f, x1, ..., xN)` is equivalent to
    //! @code
    //!     foldl(ap, fmap(curry<N>, f), list(x1, ..., xN));
    //! @endcode
    //! where `ap(f, x)` (called with two arguments only) dispatches to the
    //! implementation in the type class. This basically means that applying
    //! a `N`-ary function with `ap` is equivalent to applying a curried
    //! binary function to each argument starting from the left with the
    //! `ap` provided in the type class.
    //!
    //! ### Example
    //! @snippet example/list/applicative/overview.cpp main
    //!
    //! @todo
    //! Consider giving access to all the arguments to the type class
    //! implementation. This is for performance purposes.
    constexpr applicative_detail::ap ap{};

    //! Lift a value into the functor.
    //! @method{Applicative}
    template <typename A>
    constexpr applicative_detail::lift<A> lift{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_APPLICATIVE_HPP
