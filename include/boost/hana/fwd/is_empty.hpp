/*!
@file
Forward declares `boost::hana::is_empty`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_IS_EMPTY_HPP
#define BOOST_HANA_FWD_IS_EMPTY_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Returns whether the iterable is empty.
    //! @relates Iterable
    //!
    //! `is_empty` must return a compile-time `Logical` representing
    //! whether the iterable is empty.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/iterable.cpp is_empty
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto is_empty = [](auto&& xs) {
        return tag-dispatched;
    };
#else
    template <typename It, typename = void>
    struct is_empty_impl : is_empty_impl<It, when<true>> { };

    struct is_empty_t {
        template <typename Xs>
        constexpr auto operator()(Xs&& xs) const;
    };

    constexpr is_empty_t is_empty{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_IS_EMPTY_HPP
