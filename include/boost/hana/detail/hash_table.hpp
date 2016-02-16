/*!
@file
Defines `boost::hana::detail::hash_table`.

@copyright Jason Rice 2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_HASH_TABLE_HPP
#define BOOST_HANA_DETAIL_HASH_TABLE_HPP

#include <boost/hana/detail/index_if.hpp>
#include <boost/hana/hash.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/remove_if.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana/tuple.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

template <typename T>
struct show { };


BOOST_HANA_NAMESPACE_BEGIN namespace detail {
    template <typename TypeHash>
    struct has_type_hash { };

    template <typename TypeHash_, std::size_t ...i>
    struct bucket : detail::has_type_hash<TypeHash_> { };

    struct non_bucket { };

    // hash_table
    template <typename ...Bucket>
    struct hash_table : Bucket... { };

    // filter_hash_table
    template <typename TypeHash>
    struct filter_hash_table_pred {
        template <typename Bucket>
        constexpr auto operator()(Bucket) const {
            return hana::bool_c<std::is_base_of<detail::has_type_hash<TypeHash>, Bucket>::value>;
        }
    };

    template <typename TypeHash, typename ...Buckets>
    constexpr auto filter_hash_table(detail::hash_table<Buckets...>) {
        return typename decltype(hana::unpack(
            hana::remove_if(hana::make_tuple(Buckets{}...), detail::filter_hash_table_pred<TypeHash>{})
            , hana::template_<detail::hash_table>
        ))::type{};
    }

    //get_bucket_type - used by hash_table_insert
    template <typename TypeHash, typename NoBucket,
        typename = typename std::enable_if<!std::is_base_of<
            detail::has_type_hash<TypeHash>, NoBucket>::value>::type>
    constexpr auto
    get_bucket_type(NoBucket const&) {
        return hana::type_c<non_bucket>;
    }
    template <typename TypeHash, std::size_t ...i>
    constexpr auto get_bucket_type(detail::bucket<TypeHash, i...>) {
       return hana::type_c<detail::bucket<TypeHash, i...>>;
    }

    //bucket_get - return index of matched element
    template <typename Indices, typename Index>
    constexpr auto bucket_get_helper(Indices indices, Index) {
        return hana::just(hana::get_impl<Index::value>(indices));
    }

    template <typename Indices>
    constexpr auto bucket_get_helper(Indices, hana::int_<-1>) {
        return hana::nothing;
    }

    template <typename GetKey, typename TypeHash, typename NoBucket, typename Storage, typename Key,
        typename = typename std::enable_if<!std::is_base_of<
            detail::has_type_hash<TypeHash>, NoBucket>::value>::type>
    constexpr auto
    bucket_get(NoBucket const&, Storage const&, Key const&) {
        return hana::nothing;
    }

    template <typename GetKey, typename TypeHash, typename Storage, typename Key>
    constexpr auto
    bucket_get(detail::bucket<TypeHash> const&, Storage const&, Key const&) {
        return hana::nothing;
    }

    template <typename GetKey, typename TypeHash, std::size_t i, typename Storage, typename Key>
    constexpr auto
    bucket_get(detail::bucket<TypeHash, i> const&, Storage const& storage, Key const& key) {
        return decltype(hana::if_(
            hana::equal(std::declval<GetKey>()(hana::at_c<i>(storage)), key)
            , hana::just(hana::size_c<i>)
            , hana::nothing
        )){};
    }

    template <typename GetKey, typename TypeHash, std::size_t ...i, typename Storage, typename Key>
    constexpr auto
    bucket_get(detail::bucket<TypeHash, i...> const&, Storage const& storage, Key const& key) {
        using Xs = decltype(hana::make_basic_tuple(hana::at(storage, hana::size_c<i>)...));
        using Indices = decltype(hana::make_basic_tuple(hana::size_c<i>...));
        using Pred = decltype(hana::compose(hana::equal.to(key), std::declval<GetKey>()));
        using Pack = typename detail::make_pack<Xs>::type;
        return detail::bucket_get_helper(Indices{}, detail::index_if<Pred, Pack>{});
    }


    // bucket_append
    template <typename GetKey, typename Storage, typename TypeHash, std::size_t ...i, typename Index>
    constexpr auto bucket_append(detail::bucket<TypeHash, i...>, Index) {
        static_assert(decltype(hana::is_nothing(
            detail::bucket_get<GetKey, TypeHash>(
                detail::bucket<TypeHash, i...>{}
                , std::declval<Storage>()
                , std::declval<GetKey>()(hana::at(std::declval<Storage>(), Index{}))
            )
        )){}, "Hash based containers may not have duplicate keys.");

        return detail::bucket<TypeHash, i..., Index::value>{};
    }

    // hash_table_insert
    template <typename GetKey
        , typename Storage
        , typename TypeHash
        , typename Bucket
        , typename ...OtherBuckets
        , typename Index
    >
    constexpr auto hash_table_insert_helper2(detail::hash_table<OtherBuckets...>, Index x) {
        return detail::hash_table<OtherBuckets..., decltype(
            detail::bucket_append<GetKey, Storage, TypeHash>(Bucket{}, x))>{};
    }

    template <typename GetKey
        , typename Storage
        , typename TypeHash
        , typename Bucket
        , typename ...NonMatchingBuckets
        , typename Index
        , typename = typename std::enable_if<std::is_same<non_bucket, Bucket>::value>::type
    >
    constexpr auto hash_table_insert_helper(detail::hash_table<NonMatchingBuckets...>, Index) {
        return detail::hash_table<NonMatchingBuckets..., detail::bucket<TypeHash, Index::value>>{};
    }

    template <typename GetKey
        , typename Storage
        , typename TypeHash
        , typename Bucket
        , typename HashTable
        , typename Index
        , typename = typename std::enable_if<!std::is_same<non_bucket, Bucket>::value>::type
    >
    constexpr auto hash_table_insert_helper(HashTable h, Index x) {
        return detail::hash_table_insert_helper2<GetKey, Storage, TypeHash, Bucket>(
            detail::filter_hash_table<TypeHash>(h), x);
    }

    template <typename GetKey, typename Storage>
    struct hash_table_insert {
        template <typename HashTable, typename Index>
        constexpr auto operator()(HashTable h, Index x) {
            using Element = decltype(hana::at(std::declval<Storage>(), x));
            using TypeHash = decltype(hana::hash(std::declval<GetKey>()(std::declval<Element>())));
            using Bucket = typename decltype(detail::get_bucket_type<TypeHash>(h))::type;
            return detail::hash_table_insert_helper<GetKey, Storage, TypeHash, Bucket>(h, x);
        }
    };

    // build_hash_table - metafunction
    template <typename GetKey, typename Storage>
    struct build_hash_table {
        using type = decltype(hana::fold_left(
            hana::make_range(hana::size_c<0>, hana::size(std::declval<Storage>()))
            , detail::hash_table<>{}
            , detail::hash_table_insert<GetKey, Storage>{}
        ));
    };

    // hash_table_find
    template <typename GetKey, typename Map, typename Key>
    constexpr decltype(auto) hash_table_find_i(Map&& m, Key&& key) {
        using TypeHash = decltype(hana::hash(key));
        constexpr auto maybe_i = decltype(
            bucket_get<GetKey, TypeHash>(m, m.storage, static_cast<Key&&>(key))
        ){};
        return maybe_i;
    }

    // hash_table_at_key - returns entire element
    template <typename GetKey, typename Map, typename Key>
    constexpr decltype(auto) hash_table_at_key(Map&& m, Key&& key) {
        using TypeHash = decltype(hana::hash(key));
        constexpr auto maybe_i = decltype(
            bucket_get<GetKey, TypeHash>(m, m.storage, key)
        ){};
        //if maybe_i == hana::nothing this will explode
        static_assert(decltype(hana::is_just(maybe_i))::value, "at_key requires key to be set");
        return hana::at(static_cast<Map&&>(m).storage, *maybe_i);
    }

} BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_DETAIL_HASH_TABLE_HPP
