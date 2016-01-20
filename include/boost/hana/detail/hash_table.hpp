/*!
@file
Defines `boost::hana::detail::hash_table`.

@copyright Jason Rice 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_HASH_TABLE_HPP
#define BOOST_HANA_DETAIL_HASH_TABLE_HPP

#include <boost/hana/hash.hpp>

#include <utility>


BOOST_HANA_NAMESPACE_BEGIN namespace detail {
    template <typename Element, typename TypeHash>
    struct bucket { Element element; };

    template<typename ...Element>
    struct hash_table
        : bucket<Element, decltype(hana::hash(std::declval<Element>()))>...
    {
        template <typename ...Xn>
        explicit constexpr hash_table(Xn&& ...xn)
            : bucket<Element,
                decltype(hana::hash(std::declval<Element>()))>{static_cast<Xn&&>(xn)}...
        { }
    };

    template <typename TypeHash, typename Element>
    static constexpr Element&& bucket_get_helper(detail::bucket<Element, TypeHash>&& m) {
        return static_cast<Element&&>(m.element);
    }

    template <typename TypeHash, typename Element>
    static constexpr Element& bucket_get_helper(detail::bucket<Element, TypeHash>& m) {
        return m.element;
    }

    template <typename TypeHash, typename Element>
    static constexpr Element const& bucket_get_helper(detail::bucket<Element, TypeHash> const& m) {
        return m.element;
    }

    template<typename M, typename TypeHash>
    static constexpr decltype(auto) bucket_get(M&& m, TypeHash const&) {
        return bucket_get_helper<TypeHash>(static_cast<M&&>(m));
    }
} BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_DETAIL_HASH_TABLE_HPP
