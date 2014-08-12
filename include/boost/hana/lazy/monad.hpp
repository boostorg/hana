/*!
@file
Defines the instance of `boost::hana::Monad` for `boost::hana::Lazy`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LAZY_MONAD_HPP
#define BOOST_HANA_LAZY_MONAD_HPP

#include <boost/hana/lazy/lazy.hpp>
#include <boost/hana/monad/flatten_mcd.hpp>

// Mcd
#include <boost/hana/lazy/applicative.hpp>


namespace boost { namespace hana {
    //! Instance of `Monad` for `Lazy`.
    //!
    //! The `Lazy` monad allows combining lazy computations into larger
    //! lazy computations.
    //!
    //! ### Example
    //! @snippet example/lazy/monad.cpp main
    template <>
    struct Monad::instance<Lazy> : Monad::flatten_mcd<Lazy> {
        template <typename LLX>
        static constexpr auto flatten_impl(LLX llx) {
            auto storage = [=](auto _) {
                return eval(_(eval)(llx));
            };
            return lazy_detail::lazy<decltype(storage)>{storage};
        }

        template <typename LX, typename F>
        static constexpr auto bind_impl(LX lx, F f) {
            auto storage = [=](auto _) {
                return eval(f(_(eval)(lx)));
            };
            return lazy_detail::lazy<decltype(storage)>{storage};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LAZY_MONAD_HPP
