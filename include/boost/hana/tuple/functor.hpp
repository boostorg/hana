/*!
@file
Defines the instance of `boost::hana::Functor` for `boost::hana::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_FUNCTOR_HPP
#define BOOST_HANA_TUPLE_FUNCTOR_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/tuple/tuple.hpp>


namespace boost { namespace hana {
    template <>
    struct Functor::instance<Tuple> : Functor::fmap_mcd {
        template <typename Xs, typename F>
        static constexpr decltype(auto) fmap_impl(Xs&& xs, F&& f) {
            return detail::std::forward<Xs>(xs).storage(
                [f(detail::std::forward<F>(f))](auto&& ...xs) -> decltype(auto) {
                    return tuple(f(detail::std::forward<decltype(xs)>(xs))...);
                }
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_FUNCTOR_HPP
