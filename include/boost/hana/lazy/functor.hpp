/*!
@file
Defines the instance of `boost::hana::Functor` for `boost::hana::Lazy`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LAZY_FUNCTOR_HPP
#define BOOST_HANA_LAZY_FUNCTOR_HPP

#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/lazy/lazy.hpp>


namespace boost { namespace hana {
    //! Instance of `Functor` for `Lazy`.
    //!
    //! `fmap`ing a function to a lazy value returns the result of applying
    //! the function as a lazy value.
    //!
    //! ### Example
    //! @snippet example/lazy/functor/fmap.cpp main
    template <>
    struct Functor::instance<Lazy> : Functor::fmap_mcd {
        template <typename F, typename LX>
        static constexpr auto fmap_impl(F f, LX lx) {
            auto storage = [=](auto _) { return f(_(eval)(lx)); };
            return lazy_detail::lazy<decltype(storage)>{storage};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LAZY_FUNCTOR_HPP
