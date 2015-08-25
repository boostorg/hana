/*!
@file
Defines `boost::hana::slice` and `boost::hana::slice_c`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SLICE_HPP
#define BOOST_HANA_SLICE_HPP

#include <boost/hana/fwd/slice.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename From, typename To>
    constexpr auto slice_t::operator()(Xs&& xs, From&& from, To&& to) const {
        using S = typename datatype<Xs>::type;
        using Slice = BOOST_HANA_DISPATCH_IF(slice_impl<S>,
            _models<Sequence, S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Sequence, S>::value,
        "hana::slice(xs, from, to) requires 'xs' to be a Sequence");
    #endif

        return Slice::apply(static_cast<Xs&&>(xs),
                            static_cast<From&&>(from),
                            static_cast<To&&>(to));
    }
    //! @endcond

    template <typename S, bool condition>
    struct slice_impl<S, when<condition>> : default_ {
        template <std::size_t from, typename Xs, std::size_t ...i>
        static constexpr auto slice_helper(Xs&& xs, std::index_sequence<i...>) {
            return hana::make<S>(
                hana::at_c<from + i>(static_cast<Xs&&>(xs))...
            );
        }

        template <typename Xs, typename From, typename To>
        static constexpr auto apply(Xs&& xs, From const&, To const&) {
            constexpr std::size_t from = hana::value<From>();
            constexpr std::size_t to = hana::value<To>();
            return slice_helper<from>(static_cast<Xs&&>(xs),
                                      std::make_index_sequence<to - from>{});
        }
    };

    template <std::size_t from, std::size_t to>
    struct slice_c_t {
        template <typename Xs>
        constexpr auto operator()(Xs&& xs) const {
            return hana::slice(static_cast<Xs&&>(xs),
                               hana::size_t<from>, hana::size_t<to>);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SLICE_HPP
