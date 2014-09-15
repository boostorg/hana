/*!
@file
Defines the instance of `boost::hana::Foldable` for `boost::hana::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_FOLDABLE_HPP
#define BOOST_HANA_TUPLE_FOLDABLE_HPP

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable/unpack_mcd.hpp>
#include <boost/hana/tuple/tuple.hpp>


namespace boost { namespace hana {
    template <>
    struct Foldable::instance<Tuple> : Foldable::unpack_mcd {
        template <typename Xs, typename F>
        static constexpr decltype(auto) unpack_impl(Xs&& xs, F&& f) {
            return detail::std::forward<Xs>(xs)
                    .storage(detail::std::forward<F>(f));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_FOLDABLE_HPP
