/*!
@file
Defines the instance of `boost::hana::Applicative` for `boost::hana::Lazy`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LAZY_APPLICATIVE_HPP
#define BOOST_HANA_LAZY_APPLICATIVE_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/lazy/lazy.hpp>

// Mcd
#include <boost/hana/lazy/functor.hpp>


namespace boost { namespace hana {
    //! Instance of `Applicative` for `Lazy`.
    //!
    //! A normal value can be lifted into a lazy value by using `lift<Lazy>`.
    //! A lazy function can be lazily applied to a lazy value by using `ap`.
    template <>
    struct Applicative::instance<Lazy> : Applicative::mcd {
        template <typename X>
        static constexpr auto lift_impl(X x) {
            auto storage = [=](auto) { return x; };
            return lazy_detail::lazy<decltype(storage)>{storage};
        }

        template <typename LF, typename LX>
        static constexpr auto ap_impl(LF lf, LX lx) {
            auto storage = [=](auto _) { return _(eval)(lf)(_(eval)(lx)); };
            return lazy_detail::lazy<decltype(storage)>{storage};
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LAZY_APPLICATIVE_HPP
