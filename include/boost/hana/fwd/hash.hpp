/*!
@file
Forward declares `boost::hana::hash`.

@copyright Jason Rice 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_HASH_HPP
#define BOOST_HANA_FWD_HASH_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/core/when.hpp>


BOOST_HANA_NAMESPACE_BEGIN
    //! Creates a type-level hash for use in associative containers.
    //! @relates Hashable
    //!
    //! @param x
    //! The value to be hashed.
    //!
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto hash = [](auto&& x) {
        return tag-dispatched;
    };
#else
    template <typename T, typename = void>
    struct hash_impl : hash_impl<T, when<true>> { };

    struct hash_t {
        template <typename X>
        constexpr auto operator()(X&& x) const;
    };

    constexpr hash_t hash{};
#endif
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_FWD_HASH_HPP
