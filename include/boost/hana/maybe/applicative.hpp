/*!
@file
Defines the instance of `boost::hana::Applicative` for `boost::hana::Maybe`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_APPLICATIVE_HPP
#define BOOST_HANA_MAYBE_APPLICATIVE_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functor/functor.hpp>
#include <boost/hana/maybe/maybe.hpp>

// Mcd
#include <boost/hana/maybe/functor.hpp>


namespace boost { namespace hana {
    //! Instance of `Applicative` for `Maybe`s.
    //!
    //! First, a value can be made optional with `lift<Maybe>`, which is
    //! equivalent to `just`. Second, one can feed an optional value to an
    //! optional function with `ap`, which will return `just(f(x))` if there
    //! is both a function _and_ a value, and `nothing` otherwise.
    //!
    //! ### Example
    //! @snippet example/maybe/applicative_simple.cpp main
    //!
    //! ### Example
    //! @include example/maybe/applicative.cpp
    template <>
    struct Applicative::instance<Maybe> : Applicative::mcd {
        template <typename X>
        static constexpr decltype(auto) lift_impl(X&& x)
        { return just(detail::std::forward<X>(x)); }

        template <typename Mf, typename Mx>
        static constexpr decltype(auto) ap_impl(Mf&& mf, Mx&& mx) {
            return maybe(
                nothing,
                [&mx](auto&& f) -> decltype(auto) {
                    return fmap(
                        detail::std::forward<Mx>(mx),
                        detail::std::forward<decltype(f)>(f)
                    );
                },
                detail::std::forward<Mf>(mf)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_APPLICATIVE_HPP
