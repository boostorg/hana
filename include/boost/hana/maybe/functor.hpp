/*!
@file
Defines the instance of `boost::hana::Functor` for `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_FUNCTOR_HPP
#define BOOST_HANA_MAYBE_FUNCTOR_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/maybe/maybe.hpp>


namespace boost { namespace hana {
    //! Instance of `Functor` for `Maybe`s.
    //!
    //! A `Maybe` can be seen as a `List` containing either one element
    //! (`just(x)`) or no elements at all (`nothing`). As such, `fmap` for
    //! `Maybe`s returns `nothing` when applied to `nothing` and `just(f(x))`
    //! when applied to `just(x)`.
    //!
    //! ### Example
    //! @snippet example/maybe/functor.cpp main
    template <>
    struct Functor::instance<Maybe> : Functor::fmap_mcd {
        template <typename M, typename F>
        static constexpr decltype(auto) fmap_impl(M&& m, F&& f) {
            return maybe(
                nothing,
                compose(just, detail::std::forward<F>(f)),
                detail::std::forward<M>(m)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_FUNCTOR_HPP
