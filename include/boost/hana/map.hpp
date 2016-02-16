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
#include <boost/hana/remove_at.hpp>
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

        template <typename ...Pair>
        struct build_map_hash_table
        {
            using type = typename build_hash_table<hana::first_t, hana::tuple<Pair...>>::type;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // map
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <>
    struct map<>
        : detail::build_map_hash_table<>::type
        , detail::searchable_operators<map<>>
    {
        hana::tuple<> storage;
        using hana_tag = map_tag;
        static constexpr std::size_t size = 0;

        constexpr map() { }
    };

    template <typename ...Pairs>
    struct map
        : detail::build_map_hash_table<Pairs...>::type
        , detail::searchable_operators<map<Pairs...>>
        , detail::operators::adl<map<Pairs...>>
    {
        hana::tuple<Pairs...> storage;
        using hana_tag = map_tag;
        static constexpr std::size_t size = sizeof...(Pairs);

        constexpr map()
            : storage(Pairs{}...)
        { }

        explicit constexpr map(Pairs const& ...xn)
            : storage(xn...)
        { }

        explicit constexpr map(Pairs&& ...xn)
            : storage(static_cast<Pairs&&>(xn)...)
        { }

        constexpr map() = default;
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
        static constexpr decltype(auto) apply(Map&& map) {
            return hana::transform(static_cast<Map&&>(map).storage, hana::first);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // values
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename Map>
    constexpr decltype(auto) values_t::operator()(Map&& map) const {
        return hana::transform(static_cast<Map&&>(map).storage, hana::second);
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct insert_impl<map_tag> {
        template <typename Xs, typename Pair, typename Indices>
        static constexpr auto
        insert_helper(Xs&& xs, Pair&&, hana::true_, Indices) {
            return static_cast<Xs&&>(xs);
        }

        template <typename Xs, typename Pair, std::size_t ...n>
        static constexpr auto
        insert_helper(Xs&& xs, Pair&& pair, hana::false_, std::index_sequence<n...>) {
            return hana::make_map(
                hana::at_c<n>(static_cast<Xs&&>(xs).storage)..., static_cast<Pair&&>(pair)
            );
        }

        template <typename Xs, typename Pair>
        static constexpr auto apply(Xs&& xs, Pair&& pair) {
            constexpr bool contains = hana::value<decltype(
                hana::contains(xs, hana::first(pair))
            )>();
            constexpr std::size_t size = std::remove_reference<Xs>::type::size;
            return insert_helper(static_cast<Xs&&>(xs), static_cast<Pair&&>(pair),
                                 hana::bool_c<contains>, std::make_index_sequence<size>{});
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // erase_key
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct erase_key_impl<map_tag> {
        template <typename M>
        struct lazy_helper {
            M const& m;
            template <typename Index>
            constexpr auto operator()(Index index) {
                return hana::unpack(hana::remove_at(m.storage, index), hana::make_map);
            }
        };

        template <typename M, typename Key>
        static constexpr auto apply(M const& m, Key&& key) {
            auto maybe_i = detail::hash_table_find_i<hana::first_t>(m, static_cast<Key&&>(key));
            return hana::maybe(m, lazy_helper<M>{m}, maybe_i);
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
    struct find_impl<map_tag> {
        template <typename M>
        struct lazy_helper {
            M const& m;
            template <typename Index>
            constexpr auto operator()(Index index) {
                return hana::second(hana::at(m.storage, index));
            }
        };

        template <typename M, typename Key>
        static constexpr auto apply(M&& m, Key&& key) {
            auto maybe_i = detail::hash_table_find_i<hana::first_t>(m, static_cast<Key&&>(key));
            return hana::transform(maybe_i, lazy_helper<M>{static_cast<M&&>(m)});
        }
    };

    template <>
    struct find_if_impl<map_tag> {
        template <typename M, typename Pred>
        static constexpr auto apply(M&& map, Pred&& pred) {
            return hana::transform(
                hana::find_if(static_cast<M&&>(map).storage,
                    hana::compose(static_cast<Pred&&>(pred), hana::first)),
                hana::second
            );
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
                detail::hash_table_at_key<hana::first_t>(
                    static_cast<Map&&>(m)
                    , static_cast<Key&&>(key)
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<map_tag> {
        template <typename M, typename F>
        static constexpr decltype(auto) apply(M&& map, F&& f) {
            return hana::unpack(static_cast<M&&>(map).storage,
                                static_cast<F&&>(f));
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
