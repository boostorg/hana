/*!
@file
Defines the instance of `boost::hana::Foldable` for `boost::hana::Range`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RANGE_FOLDABLE_HPP
#define BOOST_HANA_RANGE_FOLDABLE_HPP

#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/foldable/iterable_mcd.hpp>
#include <boost/hana/group/group.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/range/range.hpp>


namespace boost { namespace hana {
    //! Instance of `Foldable` for `Range`s.
    //!
    //! @todo Use the `unpack_mcd`.
    template <>
    struct Foldable::instance<Range> : Foldable::iterable_mcd {
        template <typename F, typename From, typename T, T ...vs>
        static constexpr auto unpack_helper(F f, From from,
            detail::std::integer_sequence<T, vs...>)
        { return f(integral<T, from() + vs>...); }

        template <typename R, typename F>
        static constexpr auto unpack_impl(R r, F f) {
            auto size = minus(r.to, r.from);
            return unpack_helper(f, r.from,
                detail::std::make_integer_sequence<decltype(r.from()), size()>{});
        }

        template <typename R>
        static constexpr auto length_impl(R r)
        { return minus(r.to, r.from); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RANGE_FOLDABLE_HPP
