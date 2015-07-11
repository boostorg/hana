/*!
@file
Defines `boost::hana::length`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LENGTH_HPP
#define BOOST_HANA_LENGTH_HPP

#include <boost/hana/fwd/length.hpp>

#include <boost/hana/config.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/unpack.hpp>


namespace boost { namespace hana {
    struct Foldable; //! @todo include the forward declaration instead

    //! @cond
    template <typename Xs>
    constexpr auto length_t::operator()(Xs&& xs) const {
        using S = typename datatype<Xs>::type;
        using Length = BOOST_HANA_DISPATCH_IF(length_impl<S>,
            _models<Foldable, S>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Foldable, S>::value,
        "hana::length(xs) requires 'xs' to be Foldable");
    #endif

        return Length::apply(static_cast<Xs&&>(xs));
    }
    //! @endcond

    namespace detail {
        struct argn {
            template <typename ...Xs>
            constexpr auto operator()(Xs const& ...) const
            { return hana::size_t<sizeof...(Xs)>; }
        };
    }

    template <typename T, bool condition>
    struct length_impl<T, when<condition>> : default_ {
        template <typename Xs>
        static constexpr auto apply(Xs const& xs) {
            return hana::unpack(xs, detail::argn{});
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LENGTH_HPP
