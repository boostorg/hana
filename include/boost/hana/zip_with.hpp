/*!
@file
Defines `boost::hana::zip_with`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ZIP_WITH_HPP
#define BOOST_HANA_ZIP_WITH_HPP

#include <boost/hana/fwd/zip_with.hpp>

#include <boost/hana/all.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <utility>


namespace boost { namespace hana {
    struct Foldable; //! @todo include the forward declaration instead
    struct Sequence;

    //! @cond
    template <typename F, typename Xs, typename ...Ys>
    constexpr auto zip_with_t::operator()(F&& f, Xs&& xs, Ys&& ...ys) const {
    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        constexpr bool models_of_Sequence[] = {
            _models<Sequence, Xs>::value, _models<Sequence, Ys>::value...
        };
        static_assert(hana::all(models_of_Sequence),
        "hana::zip_with(f, xs, ys...) requires 'xs' and 'ys...' to be Sequences");
    #endif

        return zip_with_impl<typename datatype<Xs>::type>::apply(
            static_cast<F&&>(f),
            static_cast<Xs&&>(xs),
            static_cast<Ys&&>(ys)...
        );
    }
    //! @endcond

    template <typename S>
    struct zip_with_impl<S, when<
        _models<Sequence, S>::value && _models<Foldable, S>::value
    >> {
        template <std::size_t N, typename F, typename ...Xs>
        static constexpr decltype(auto) transverse(F&& f, Xs&& ...xs) {
            return static_cast<F&&>(f)(hana::at_c<N>(static_cast<Xs&&>(xs))...);
        }

        template <std::size_t ...N, typename F, typename ...Xs>
        static constexpr auto
        zip_helper(std::index_sequence<N...>, F&& f, Xs&& ...xs) {
            return hana::make<S>(transverse<N>(f, xs...)...);
        }

        template <typename F, typename X, typename ...Xs>
        static constexpr auto
        apply(F&& f, X&& x, Xs&& ...xs) {
            constexpr std::size_t N = hana::value<decltype(hana::length(x))>();
            return zip_helper(std::make_index_sequence<N>{},
                              static_cast<F&&>(f),
                              static_cast<X&&>(x), static_cast<Xs&&>(xs)...);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ZIP_WITH_HPP
