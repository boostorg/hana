/*!
@file
Defines the instance of `boost::hana::Monad` for `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_MONAD_HPP
#define BOOST_HANA_MAYBE_MONAD_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/maybe/maybe.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>

// Mcd
#include <boost/hana/maybe/applicative.hpp>


namespace boost { namespace hana {
    //! Instance of `Monad` for `Maybe`s.
    //!
    //! The `Maybe` `Monad` makes it easy to compose actions that might fail.
    //! One can feed an optional value if there is one into a function with
    //! `bind`, which will return `nothing` if there is no value. Finally,
    //! optional-optional values can have their redundant level of
    //! `Maybe`ness removed with `flatten`.
    //!
    //! ### Example
    //! @include example/maybe/monad.cpp
    template <>
    struct Monad::instance<Maybe> : Monad::flatten_mcd<Maybe> {
        template <typename MMX>
        static constexpr decltype(auto) flatten_impl(MMX&& mmx) {
            return maybe(nothing, id, detail::std::forward<MMX>(mmx));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_MONAD_HPP
