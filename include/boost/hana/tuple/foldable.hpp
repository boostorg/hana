/*!
@file
Defines the instance of `boost::hana::Foldable` for `boost::hana::Tuple`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TUPLE_FOLDABLE_HPP
#define BOOST_HANA_TUPLE_FOLDABLE_HPP

#include <boost/hana/foldable/unpack_mcd.hpp>
#include <boost/hana/tuple/tuple.hpp>


namespace boost { namespace hana {
    template <>
    struct Foldable::instance<Tuple> : Foldable::unpack_mcd {
        template <typename Xs, typename F>
        static constexpr auto unpack_impl(Xs xs, F f) {
            return xs.storage(f);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TUPLE_FOLDABLE_HPP
