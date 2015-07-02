/*!
@file
Defines `boost::hana::unpack`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_UNPACK_HPP
#define BOOST_HANA_UNPACK_HPP

#include <boost/hana/fwd/unpack.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/fwd/fold_left.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    struct Foldable; //! @todo include the forward declaration instead

    //! @cond
    template <typename Xs, typename F>
    constexpr decltype(auto) unpack_t::operator()(Xs&& xs, F&& f) const {
        using S = typename datatype<Xs>::type;
        using Unpack = BOOST_HANA_DISPATCH_IF(unpack_impl<S>,
            _models<Foldable, S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Foldable, S>::value,
        "hana::unpack(xs, f) requires 'xs' to be Foldable");
    #endif

        return Unpack::apply(static_cast<Xs&&>(xs), static_cast<F&&>(f));
    }
    //! @endcond

    template <typename T, bool condition>
    struct unpack_impl<T, when<condition>> : default_ {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::fold_left(static_cast<Xs&&>(xs),
                                   static_cast<F&&>(f),
                                   hana::partial)();
        }
    };

    template <typename T, std::size_t N>
    struct unpack_impl<T[N]> {
        template <typename Xs, typename F, std::size_t ...i>
        static constexpr decltype(auto)
        unpack_helper(Xs&& xs, F&& f, std::index_sequence<i...>) {
            return static_cast<F&&>(f)(static_cast<Xs&&>(xs)[i]...);
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return unpack_impl::unpack_helper(static_cast<Xs&&>(xs),
                                              static_cast<F&&>(f),
                                              std::make_index_sequence<N>{});
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_UNPACK_HPP
