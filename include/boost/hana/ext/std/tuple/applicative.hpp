/*!
@file
Defines the instance of `boost::hana::Applicative` for `std::tuple`s.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_TUPLE_APPLICATIVE_HPP
#define BOOST_HANA_EXT_STD_TUPLE_APPLICATIVE_HPP

#include <boost/hana/applicative/mcd.hpp>
#include <boost/hana/ext/std/tuple/functor.hpp>
#include <boost/hana/ext/std/tuple/monad.hpp>

#include <tuple>


namespace boost { namespace hana {
    //! @todo
    //! Get rid of this redundant instance once
    //! http://llvm.org/bugs/show_bug.cgi?id=19616
    //! is fixed.
    template <>
    struct Applicative::instance<StdTuple> : Applicative::mcd {
        template <typename X>
        static constexpr auto lift_impl(X x)
        { return std::tuple<X>{x}; }

        template <typename Fs, typename Xs>
        static constexpr auto ap_impl(Fs fs, Xs xs)
        { return bind(fs, [=](auto f) { return fmap(f, xs); }); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_TUPLE_APPLICATIVE_HPP
