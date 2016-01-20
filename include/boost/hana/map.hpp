/*!
@file
Defines `boost::hana::map`.

@copyright Louis Dionne 2013-2016
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAP_HPP
#define BOOST_HANA_MAP_HPP

#include <boost/hana/fwd/map.hpp>

#include <boost/hana/all_of.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/product.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/detail/decay.hpp>
#include <boost/hana/detail/fast_and.hpp>
#include <boost/hana/detail/has_duplicates.hpp>
#include <boost/hana/detail/hash_table.hpp>
#include <boost/hana/detail/index_if.hpp>
#include <boost/hana/detail/intrinsics.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/searchable.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/functional/demux.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/fwd/any_of.hpp>
#include <boost/hana/fwd/at_key.hpp>
#include <boost/hana/fwd/erase_key.hpp>
#include <boost/hana/fwd/is_subset.hpp>
#include <boost/hana/fwd/keys.hpp>
#include <boost/hana/hash.hpp>
#include <boost/hana/insert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/keys.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/remove.hpp>
#include <boost/hana/second.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/unpack.hpp>
#include <boost/hana/value.hpp>

#include <cstddef>
#include <utility>


BOOST_HANA_NAMESPACE_BEGIN
    //////////////////////////////////////////////////////////////////////////
    // operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<map_tag> {
            static constexpr bool value = true;
        };

        template<typename ...Pair>
        struct map_hash_table
            : detail::bucket<Pair, decltype(hana::hash(hana::first(std::declval<Pair>())))>...
        {
            template <typename ...Xn>
            explicit constexpr map_hash_table(Xn&& ...xn)
                : detail::bucket<Pair,
                    decltype(hana::hash(hana::first(std::declval<Pair>())))>{static_cast<Xn&&>(xn)}...
            { }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // map
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <>
    struct map<>
        : detail::map_hash_table<>
        , detail::searchable_operators<map<>>
    {
        constexpr map() { }
        using hana_tag = map_tag;
        static constexpr std::size_t size = 0;
    };

    template <typename ...Pairs>
    struct map
        : detail::map_hash_table<Pairs...>
        , detail::searchable_operators<map<Pairs...>>
        , detail::operators::adl<map<Pairs...>>
    {
        using hana_tag = map_tag;
        static constexpr std::size_t size = sizeof...(Pairs);

        constexpr map()
            : detail::map_hash_table<Pairs...>(Pairs{}...)
        { }

        explicit constexpr map(Pairs const& ...xn)
            : detail::map_hash_table<Pairs...>(xn...)
        { }

        explicit constexpr map(Pairs&& ...xn)
            : detail::map_hash_table<Pairs...>(
                    static_cast<Pairs&&>(xn)...)
        { }

        constexpr map(map const& other) = default;
        constexpr map(map&& other) = default;
    };
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // make<map_tag>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<map_tag> {
        template <typename ...Pairs>
        static constexpr auto apply(Pairs&& ...pairs) {
#if defined(BOOST_HANA_CONFIG_ENABLE_DEBUG_MODE)
            static_assert(detail::fast_and<hana::Product<Pairs>::value...>::value,
            "hana::make_map(pairs...) requires all the 'pairs' to be Products");

            static_assert(detail::fast_and<
                Comparable<decltype(hana::first(pairs))>::value...
            >::value,
            "hana::make_map(pairs...) requires all the keys to be Comparable");

            static_assert(detail::fast_and<
                Constant<
                    decltype(hana::equal(hana::first(pairs), hana::first(pairs)))
                >::value...
            >::value,
            "hana::make_map(pairs...) requires all the keys to be "
            "Comparable at compile-time");

            static_assert(!detail::has_duplicates<decltype(hana::hash(hana::first(pairs)))...>::value,
            "hana::make_map(pairs...) requires all the keys to be unique");
#endif

            return map<typename detail::decay<Pairs>::type...>(
                static_cast<Pairs&&>(pairs)...
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // keys
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct keys_impl<map_tag> {
        template <typename Map>
        static constexpr auto apply(Map&& m) {
            return hana::unpack(static_cast<Map&&>(m),
                                hana::on(hana::make_tuple, hana::first));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // values
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename Map>
    constexpr decltype(auto) values_t::operator()(Map&& m) const {
        return hana::unpack(static_cast<Map&&>(m),
                            hana::on(hana::make_tuple, hana::second));
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct insert_impl<map_tag> {
        template <typename ...Pairs, typename Pair>
        static constexpr auto
        insert_helper(hana::map<Pairs...> const& m,
                      Pair&& pair, hana::false_)
        {
            return hana::make_map(
                detail::bucket_get(m, decltype(hana::hash(hana::first(std::declval<Pairs>()))){})...,
                static_cast<Pair&&>(pair)
            );
        }

        template <typename ...Pairs, typename Pair>
        static constexpr auto
        insert_helper(hana::map<Pairs...>&& m,
                      Pair&& pair, hana::false_)
        {
            return hana::make_map(
                detail::bucket_get(m, decltype(hana::hash(hana::first(std::declval<Pairs>()))){})...,
                static_cast<Pair&&>(pair)
            );
        }

        template <typename Map, typename Pair>
        static constexpr auto
        insert_helper(Map&& m, Pair&&, hana::true_) {
            return static_cast<Map&&>(m);
        }

        template <typename Map, typename Pair>
        static constexpr auto apply(Map&& m, Pair&& pair) {
            constexpr bool contains = hana::value<decltype(
                hana::contains(m, hana::first(pair))
            )>();
            return insert_helper(static_cast<Map&&>(m), static_cast<Pair&&>(pair),
                                 hana::bool_c<contains>);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // erase_key
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct erase_key_impl<map_tag> {
        template<typename Map>
        struct copy_map_by_keys {
            Map const& old;
            template<typename ...Key>
            constexpr auto operator()(Key&& ...key) {
                return hana::make_map(detail::bucket_get(old, hana::hash(static_cast<Key&&>(key)))...);
            }
        };

        template <typename M, typename Key>
        static constexpr auto apply(M const& m, Key&& key) {
            return hana::unpack(
                hana::remove(hana::keys(m), static_cast<Key&&>(key)),
                copy_map_by_keys<M>{m}
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<map_tag, map_tag> {
        template <typename M1, typename M2>
        static constexpr auto equal_helper(M1 const&, M2 const&, hana::false_) {
            return hana::false_c;
        }

        template <typename M1, typename M2>
        static constexpr auto equal_helper(M1 const& m1, M2 const& m2, hana::true_) {
            return hana::all_of(hana::keys(m1), hana::demux(equal)(
                hana::partial(hana::find, m1),
                hana::partial(hana::find, m2)
            ));
        }

        template <typename M1, typename M2>
        static constexpr auto apply(M1 const& m1, M2 const& m2) {
            return equal_impl::equal_helper(m1, m2, hana::bool_c<
                decltype(hana::length(m1))::value ==
                decltype(hana::length(m2))::value
            >);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<map_tag> {
        template<typename Map>
        struct just_at_key {
            Map const& m;
            template<typename Key>
            constexpr auto operator()(Key&& key) {
                return hana::just(
                    hana::second( 
                        detail::bucket_get(m, hana::hash(static_cast<Key&&>(key)))
                    )
                );
            }
        };

        template <typename Map, typename Pred>
        static constexpr auto apply(Map const& m, Pred&& pred) {
            return hana::maybe(hana::nothing, just_at_key<Map>{m},
                hana::find_if(hana::keys(m), static_cast<Pred&&>(pred)));
        }
    };

    template <>
    struct any_of_impl<map_tag> {
        template <typename M, typename Pred>
        static constexpr auto apply(M const& map, Pred const& pred)
        { return hana::any_of(hana::keys(map), pred); }
    };

    template <>
    struct is_subset_impl<map_tag, map_tag> {
        template <typename Ys>
        struct all_contained {
            Ys const& ys;
            template <typename ...X>
            constexpr auto operator()(X const& ...x) const {
                return hana::bool_c<detail::fast_and<
                    hana::value<decltype(hana::contains(ys, x))>()...
                >::value>;
            }
        };

        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs const& xs, Ys const& ys) {
            auto ys_keys = hana::keys(ys);
            return hana::unpack(hana::keys(xs), all_contained<decltype(ys_keys)>{ys_keys});
        }
    };

    template <>
    struct at_key_impl<map_tag> {
        template <typename Map, typename Key>
        static constexpr decltype(auto) apply(Map&& m, Key&& key) {
            return hana::second(
                detail::bucket_get(static_cast<Map&&>(m), hana::hash(static_cast<Key&&>(key)))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<map_tag> {
        template <typename ...Pair, typename F>
        static constexpr decltype(auto)
        apply(hana::map<Pair...> const& m, F&& f) {
            return static_cast<F&&>(f)(
                detail::bucket_get(m, decltype(hana::hash(hana::first(std::declval<Pair>()))){})...
            );
        }

        template <typename ...Pair, typename F>
        static constexpr decltype(auto)
        apply(hana::map<Pair...>& m, F&& f) {
            return static_cast<F&&>(f)(
                detail::bucket_get(m, decltype(hana::hash(hana::first(std::declval<Pair>()))){})...
            );
        }

        template <typename ...Pair, typename F>
        static constexpr decltype(auto)
        apply(hana::map<Pair...>&& m, F&& f) {
            return static_cast<F&&>(f)(
                detail::bucket_get(
                    static_cast<hana::map<Pair...>&&>(m),
                    decltype(hana::hash(hana::first(std::declval<Pair>()))){}
                )...
            );
        }
    };

    template <>
    struct length_impl<map_tag> {
        template <typename ...Pairs>
        static constexpr auto apply(hana::map<Pairs...> const&) {
            return hana::size_c<sizeof...(Pairs)>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Construction from a Foldable
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct to_impl<map_tag, F, when<hana::Foldable<F>::value>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return hana::fold_left(
                static_cast<Xs&&>(xs), hana::make_map(), hana::insert
            );
        }
    };
BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_MAP_HPP
