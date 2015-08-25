/*!
@file
Defines `boost::hana::repeat`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_REPEAT_HPP
#define BOOST_HANA_REPEAT_HPP

#include <boost/hana/fwd/repeat.hpp>

#include <boost/hana/concept/monad_plus.hpp>
#include <boost/hana/core/dispatch.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/cycle.hpp>
#include <boost/hana/lift.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    template <typename M>
    struct repeat_t {
    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<MonadPlus, M>::value,
        "hana::repeat<M>(x, n) requires 'M' to be a MonadPlus");
    #endif

        template <typename X, typename N>
        constexpr auto operator()(X&& x, N&& n) const {
            using Repeat = BOOST_HANA_DISPATCH_IF(repeat_impl<M>,
                _models<MonadPlus, M>::value
            );

            return Repeat::apply(static_cast<X&&>(x), static_cast<N&&>(n));
        }
    };

    template <typename M, bool condition>
    struct repeat_impl<M, when<condition>> : default_ {
        template <typename X, typename N>
        static constexpr auto apply(X&& x, N&& n) {
            return hana::cycle(hana::lift<M>(static_cast<X&&>(x)),
                               static_cast<N&&>(n));
        }
    };

    template <typename S>
    struct repeat_impl<S, when<_models<Sequence, S>::value>> {
        template <typename X, std::size_t ...i>
        static constexpr auto repeat_helper(X&& x, std::index_sequence<i...>)
        { return hana::make<S>(((void)i, x)...); }

        template <typename X, typename N>
        static constexpr auto apply(X&& x, N const&) {
            constexpr std::size_t n = hana::value<N>();
            return repeat_helper(static_cast<X&&>(x),
                                 std::make_index_sequence<n>{});
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_REPEAT_HPP
