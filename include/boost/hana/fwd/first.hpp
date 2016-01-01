/*!
@file
Forward declares `boost::hana::first`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_FIRST_HPP
#define BOOST_HANA_FWD_FIRST_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/core/when.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! Returns the first element of a pair.
    //! @relates Product
    //!
    //!
    //! Example
    //! -------
    //! @include example/first.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto first = [](auto&& product) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename P, typename = void>
    struct first_impl : first_impl<P, when<true>> { };

    struct first_t {
        template <typename Pair>
        constexpr decltype(auto) operator()(Pair&& pair) const;
    };

    constexpr first_t first{};
#endif
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_FWD_FIRST_HPP
