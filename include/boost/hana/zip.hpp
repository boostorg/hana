/*!
@file
Defines `boost::hana::zip`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ZIP_HPP
#define BOOST_HANA_ZIP_HPP

#include <boost/hana/fwd/zip.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>

#include <boost/hana/all.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    //! @cond
    template <typename Xs, typename ...Ys>
    constexpr auto zip_t::operator()(Xs&& xs, Ys&& ...ys) const {
    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        constexpr bool models_of_Sequence[] = {
            _models<Sequence, Xs>{}, _models<Sequence, Ys>{}...
        };
        static_assert(hana::all(models_of_Sequence),
        "hana::zip(xs, ys...) requires 'xs' and 'ys...' to be Sequences");
    #endif

        return zip_impl<typename datatype<Xs>::type>::apply(
            static_cast<Xs&&>(xs),
            static_cast<Ys&&>(ys)...
        );
    }
    //! @endcond

    template <typename S, bool condition>
    struct zip_impl<S, when<condition>> : default_ {
        template <typename ...Xs>
        static constexpr decltype(auto) apply(Xs&& ...xs) {
            return hana::zip_with(hana::make_tuple, static_cast<Xs&&>(xs)...);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ZIP_HPP
