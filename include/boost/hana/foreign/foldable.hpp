/*!
@file
Defines the instance of `boost::hana::Foldable` for foreign types.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOREIGN_FOLDABLE_HPP
#define BOOST_HANA_FOREIGN_FOLDABLE_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/foldable/unpack_mcd.hpp>


namespace boost { namespace hana {
    //! Instance of `Foldable` for array types.
    //!
    //! Builtin arrays whose size is known can be folded as-if they were
    //! homogeneous tuples.
    //!
    //! @note
    //! Builtin arrays can't be made more than `Foldable` (e.g. `Iterable`)
    //! because they can't be empty and they also can't be returned from
    //! functions.
    template <typename T, detail::std::size_t N>
    struct Foldable::instance<T[N]>
        : Foldable::unpack_mcd
    {
        template <typename Xs, typename F, detail::std::size_t ...i>
        static constexpr decltype(auto)
        unpack_helper(Xs&& xs, F&& f, detail::std::index_sequence<i...>) {
            return detail::std::forward<F>(f)(
                detail::std::forward<T>(xs[i])...
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) unpack_impl(Xs&& xs, F&& f) {
            return unpack_helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f),
                detail::std::make_index_sequence<N>{}
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOREIGN_FOLDABLE_HPP
