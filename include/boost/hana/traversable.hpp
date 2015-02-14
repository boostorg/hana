/*!
@file
Defines `boost::hana::Traversable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TRAVERSABLE_HPP
#define BOOST_HANA_TRAVERSABLE_HPP

#include <boost/hana/fwd/traversable.hpp>

#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dependent_on.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // sequence
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct sequence_impl : sequence_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct sequence_impl<T, when<condition>> : default_ {
        template <typename A, typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using Traversable = detail::dependent_on_t<sizeof(xs) == 1, T>;
            static_assert(!is_default<traverse_impl<Traversable>>{},
            "no definition of boost::hana::sequence for the given data type");

            return hana::traverse<A>(detail::std::forward<Xs>(xs), id);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // traverse
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct traverse_impl : traverse_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct traverse_impl<T, when<condition>> : default_ {
        template <typename A, typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            using Traversable = detail::dependent_on_t<sizeof(xs) == 1, T>;
            static_assert(!is_default<sequence_impl<Traversable>>{},
            "no definition of boost::hana::traverse for the given data type");

            return hana::sequence<A>(
                    hana::transform(detail::std::forward<Xs>(xs),
                                    detail::std::forward<F>(f)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Traversable(T)>
        : detail::std::integral_constant<bool,
            !is_default<sequence_impl<T>>{} ||
            !is_default<traverse_impl<T>>{}
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TRAVERSABLE_HPP
