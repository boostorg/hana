/*!
@file
Forward declares `boost::hana::succ`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_SUCC_HPP
#define BOOST_HANA_FWD_SUCC_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! Returns the successor of a value.
    //! @relates Enumerable
    //!
    //!
    //! Example
    //! -------
    //! @include example/succ.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto succ = [](auto&& num) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename E, typename = void>
    struct succ_impl : succ_impl<E, when<true>> { };

    struct succ_t {
        template <typename N>
        constexpr decltype(auto) operator()(N&& n) const;
    };

    constexpr succ_t succ{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_SUCC_HPP
