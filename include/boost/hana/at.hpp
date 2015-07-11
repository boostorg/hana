/*!
@file
Defines `boost::hana::at` and `boost::hana::at_c`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_AT_HPP
#define BOOST_HANA_AT_HPP

#include <boost/hana/fwd/at.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/integral_constant.hpp>


namespace boost { namespace hana {
    struct Iterable; //! @todo include the forward declaration instead

    //! @cond
    template <typename Xs, typename N>
    constexpr decltype(auto) at_t::operator()(Xs&& xs, N&& n) const {
        using It = typename datatype<Xs>::type;
        using At = BOOST_HANA_DISPATCH_IF(at_impl<It>,
            _models<Iterable, It>::value
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Iterable, It>::value,
        "hana::at(xs, n) requires 'xs' to be an Iterable");
    #endif

        return At::apply(static_cast<Xs&&>(xs), static_cast<N&&>(n));
    }
    //! @endcond

    template <typename It, bool condition>
    struct at_impl<It, when<condition>> : default_ {
        template <typename ...Args>
        static constexpr auto apply(Args&& ...) = delete;
    };

    template <std::size_t n>
    struct at_c_t {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const
        { return hana::at(static_cast<Xs&&>(xs), hana::size_t<n>); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_AT_HPP
