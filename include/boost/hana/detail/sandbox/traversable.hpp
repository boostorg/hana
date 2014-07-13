/*!
@file
Defines `boost::hana::Traversable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TRAVERSABLE_HPP
#define BOOST_HANA_TRAVERSABLE_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup typeclasses
    `Traversable` represents types that can be folded in a
    structure-preserving manner.

    --------------------------------------------------------------------------

    ## Laws

     */
    struct Traversable {
        BOOST_HANA_TYPECLASS(Traversable);
        struct traverse_mcd;
    };

    //! Structure-preserving left-fold with an `Applicative`.
    //! @method{Traversable}
    //!
    //! ### Example
    //! @todo
    BOOST_HANA_CONSTEXPR_LAMBDA auto traverse = [](auto f, auto traversable) {
        return Traversable::instance<
            datatype_t<decltype(traversable)>
        >::traverse_impl(f, traversable);
    };

    //! Minimal complete definition: `traverse`
    struct Traversable::traverse_mcd {

    };

    template <>
    struct Traversable::instance<Maybe> : Traversable::traverse_mcd {
        template <typename X, template <typename> class A, typename Y>
        static constexpr auto traverse_impl(Function<X, A<Y>> f, Maybe<X> mx) {
            return maybe(
                lift<A>(nothing<Y>),
                [](auto x) { return ap(lift<A>(just<Y>), f(x)); },
                mx
            );
        }

        template <typename F, typename M>
        static constexpr auto traverse_impl(F f, M m) {
            using A = datatype_t<decltype(f(???))>;
            nothing -> lift<A>(nothing)
            just(x) -> ap(lift<A>(just), f(x))
        }
    };

    template <>
    struct Traversable::instance<List> : Traversable::traverse_mcd {
        template <typename X, template <typename> class A, typename Y>
        static constexpr auto traverse_impl(Function<X, A<Y>> f, List<X> xs) {
            auto cons_f = [=](auto x, auto ys) {
                return ap(ap(lift<A>(cons), f(x)), ys);
            };
            return foldr(cons_f, lift<A>(list<Y>()), xs);
        }

        template <typename F, typename Xs>
        static constexpr auto traverse_impl(F f, Xs xs) {
            using A = ???;
            auto cons_f = [=](auto x, auto ys) {
                return ap(ap(lift<A>(cons), f(x)), ys);
            };
            return foldr(cons_f, lift<A>(list()), xs);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TRAVERSABLE_HPP
