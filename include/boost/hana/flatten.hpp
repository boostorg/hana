/*!
@file
Defines `boost::hana::flatten`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FLATTEN_HPP
#define BOOST_HANA_FLATTEN_HPP

#include <boost/hana/fwd/flatten.hpp>

#include <boost/hana/at.hpp>
#include <boost/hana/concept/monad.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/detail/algorithm.hpp>
#include <boost/hana/detail/array.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/fwd/chain.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/unpack.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs>
    constexpr auto flatten_t::operator()(Xs&& xs) const {
        using M = typename hana::tag_of<Xs>::type;
        using Flatten = BOOST_HANA_DISPATCH_IF(flatten_impl<M>,
            Monad<M>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(Monad<M>::value,
        "hana::flatten(xs) requires 'xs' to be a Monad");
    #endif

        return Flatten::apply(static_cast<Xs&&>(xs));
    }
    //! @endcond

    template <typename M, bool condition>
    struct flatten_impl<M, when<condition>> : default_ {
        template <typename Xs>
        static constexpr auto apply(Xs&& xs)
        { return hana::chain(static_cast<Xs&&>(xs), hana::id); }
    };

    namespace detail {
        template <std::size_t ...Lengths>
        struct flatten_indices {
            // avoid empty arrays by appending 0 to `lengths`
            static constexpr std::size_t lengths[] = {Lengths..., 0};
            static constexpr auto flat_length =
                detail::accumulate(lengths, lengths + sizeof...(Lengths), 0);

            template <bool Inner>
            static constexpr auto compute() {
                detail::array<std::size_t, flat_length> indices{};
                for (std::size_t index = 0, i = 0; i < sizeof...(Lengths); ++i)
                    for (std::size_t j = 0; j < lengths[i]; ++j, ++index)
                        indices[index] = (Inner ? i : j);
                return indices;
            }

            static constexpr auto inner = compute<true>();
            static constexpr auto outer = compute<false>();

            template <typename S, typename Xs, std::size_t ...i>
            static constexpr auto apply(Xs&& xs, std::index_sequence<i...>) {
                return hana::make<S>(
                    hana::at_c<outer[i]>(hana::at_c<inner[i]>(
                        static_cast<Xs&&>(xs)
                    ))...
                );
            }
        };
    }

    template <typename S>
    struct flatten_impl<S, when<Sequence<S>::value>> {
        template <typename ...Xs>
        auto operator()(Xs const& ...xs) const -> detail::flatten_indices<
            decltype(hana::length(xs))::value...
        >;

        template <typename Xs>
        static constexpr auto apply(Xs&& xs) {
            using Indices = decltype(hana::unpack(xs, flatten_impl{}));
            return Indices::template apply<S>(static_cast<Xs&&>(xs),
                            std::make_index_sequence<Indices::flat_length>{});
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FLATTEN_HPP
