/*!
@file
Forward declares `boost::hana::Traversable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TRAVERSABLE_TRAVERSABLE_HPP
#define BOOST_HANA_TRAVERSABLE_TRAVERSABLE_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! Data structures that can be traversed from left to right.
    struct Traversable {
        BOOST_HANA_TYPECLASS(Traversable);
        struct traverse_mcd;
    };

    namespace traversable_detail {
        template <typename A>
        struct traverse {
            template <typename F, typename T>
            constexpr auto operator()(F f, T traversable) const {
                return Traversable::instance<
                    datatype_t<decltype(traversable)>
                >::template traverse_impl<A>(f, traversable);
            }
        };

        template <typename A>
        struct sequence {
            template <typename T>
            constexpr auto operator()(T traversable) const {
                return Traversable::instance<
                    datatype_t<decltype(traversable)>
                >::template sequence_impl<A>(traversable);
            }
        };
    }

    //! Map each element of a structure to an action, evaluate these actions
    //! from left to right, and collect the results.
    //! @method{Traversable}
    //!
    //! ### Example
    //! @snippet example/list/traversable/traverse.cpp main
    //!
    //! @note
    //! We must specify the target `Applicative` explicitly because the current
    //! data-type system is not powerful enough to express the return type of
    //! the function passed to `traverse`.
    template <typename A>
    constexpr traversable_detail::traverse<A> traverse{};

    //! Evaluate each action in the structure from left to right, and collect
    //! the results.
    //! @method{Traversable}
    //!
    //! ### Example
    //! @snippet example/list/traversable/sequence.cpp main
    template <typename A>
    constexpr traversable_detail::sequence<A> sequence{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_TRAVERSABLE_TRAVERSABLE_HPP
