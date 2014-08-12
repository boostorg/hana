/*!
@file
Defines the instance of `boost::hana::Functor` for `boost::hana::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_FUNCTOR_HPP
#define BOOST_HANA_TUPLE_FUNCTOR_HPP

#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/tuple/tuple.hpp>


namespace boost { namespace hana {
    template <>
    struct Functor::instance<Tuple> : Functor::fmap_mcd {
        template <typename F, typename Xs>
        static constexpr auto fmap_impl(F f, Xs xs) {
            return xs.storage([=](auto ...xs) {
                return tuple(f(xs)...);
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_FUNCTOR_HPP
