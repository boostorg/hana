/*!
@file
Forward declares `boost::hana::erase_key`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_ERASE_KEY_HPP
#define BOOST_HANA_FWD_ERASE_KEY_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Erase a key in a structure.
    //!
    //! This function is not associated to a concept right now, so its
    //! behavior can't be described generally. `Map` and `hana::set` are
    //! the only containers implementing this function right now, and
    //! you should refer to the documentation of `erase_key` for these
    //! containers instead.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto erase_key = [](auto&& xs, auto&& ...args) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct erase_key_impl : erase_key_impl<T, when<true>> { };

    struct erase_key_t {
        template <typename Set, typename ...Args>
        constexpr decltype(auto) operator()(Set&& set, Args&& ...args) const;
    };

    constexpr erase_key_t erase_key{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_ERASE_KEY_HPP
