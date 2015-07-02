/*!
@file
Defines `boost::hana::fuse`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUSE_HPP
#define BOOST_HANA_FUSE_HPP

#include <boost/hana/fwd/fuse.hpp>

#include <boost/hana/unpack.hpp>

#include <type_traits>


namespace boost { namespace hana {
    namespace detail {
        template <typename F>
        struct fused {
            F f;
            template <typename Xs>
            constexpr decltype(auto) operator()(Xs&& xs) const&
            { return hana::unpack(static_cast<Xs&&>(xs), f); }

            template <typename Xs>
            constexpr decltype(auto) operator()(Xs&& xs) &
            { return hana::unpack(static_cast<Xs&&>(xs), f); }

            template <typename Xs>
            constexpr decltype(auto) operator()(Xs&& xs) &&
            { return hana::unpack(static_cast<Xs&&>(xs), static_cast<F&&>(f)); }
        };
    }

    //! @cond
    template <typename F>
    constexpr auto fuse_t::operator()(F&& f) const {
        return detail::fused<typename std::decay<F>::type>{static_cast<F&&>(f)};
    }
    //! @endcond
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUSE_HPP
