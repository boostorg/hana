/*!
@file
Defines `boost::hana::Map`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAP_HPP
#define BOOST_HANA_MAP_HPP

#include <boost/hana/fwd/map.hpp>

#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/concept/functor.hpp>
#include <boost/hana/concept/logical.hpp>
#include <boost/hana/concept/product.hpp>
#include <boost/hana/concept/searchable.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/fast_and.hpp>
#include <boost/hana/detail/operators/adl.hpp>
#include <boost/hana/detail/operators/comparable.hpp>
#include <boost/hana/detail/operators/searchable.hpp>
#include <boost/hana/erase_key.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/demux.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/insert.hpp>
#include <boost/hana/keys.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/tuple.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // operators
    //////////////////////////////////////////////////////////////////////////
    namespace detail {
        template <>
        struct comparable_operators<Map> {
            static constexpr bool value = true;
        };
    }

    //////////////////////////////////////////////////////////////////////////
    // map
    //////////////////////////////////////////////////////////////////////////
    template <typename ...Pairs>
    struct map : detail::searchable_operators<map<Pairs...>>, operators::adl {
        tuple<Pairs...> storage;
        using hana = map;
        using datatype = Map;

        explicit constexpr map(tuple<Pairs...> const& ps)
            : storage(ps)
        { }

        explicit constexpr map(tuple<Pairs...>&& ps)
            : storage(static_cast<tuple<Pairs...>&&>(ps))
        { }
    };

    //////////////////////////////////////////////////////////////////////////
    // make<Map>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Map> {
        template <typename ...Pairs>
        static constexpr auto apply(Pairs&& ...pairs) {
        #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
            static_assert(detail::fast_and<_models<Product, Pairs>::value...>::value,
            "hana::make<Map>(pairs...) requires all the 'pairs' to be Products");

            static_assert(detail::fast_and<
                _models<Comparable, decltype(hana::first(pairs))>::value...
            >::value,
            "hana::make<Map>(pairs...) requires all the keys to be Comparable");

            static_assert(detail::fast_and<
                _models<
                    Constant,
                    decltype(hana::equal(hana::first(pairs), hana::first(pairs)))
                >::value...
            >::value,
            "hana::make<Map>(pairs...) requires all the keys to be "
            "Comparable at compile-time");
        #endif

            return map<typename std::decay<Pairs>::type...>{
                hana::make_tuple(static_cast<Pairs&&>(pairs)...)
            };
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // keys
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct keys_impl<Map> {
        template <typename Map>
        static constexpr decltype(auto) apply(Map&& map) {
            return hana::transform(static_cast<Map&&>(map).storage, first);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // values
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename Map>
    constexpr decltype(auto) values_t::operator()(Map&& map) const {
        return hana::transform(static_cast<Map&&>(map).storage, second);
    }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // insert
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct insert_impl<Map> {
        template <typename M, typename P>
        static constexpr typename std::decay<M>::type
        insert_helper(M&& map, P&&, decltype(hana::true_))
        { return static_cast<M&&>(map); }

        template <typename M, typename P>
        static constexpr decltype(auto)
        insert_helper(M&& map, P&& pair, decltype(hana::false_)) {
            return hana::unpack(
                hana::append(static_cast<M&&>(map).storage,
                             static_cast<P&&>(pair)),
                hana::make<Map>
            );
        }

        template <typename M, typename P>
        static constexpr decltype(auto) apply(M&& map, P&& pair) {
            constexpr bool contains = hana::value<decltype(
                hana::contains(map, hana::first(pair))
            )>();
            return insert_helper(static_cast<M&&>(map),
                                 static_cast<P&&>(pair),
                                 hana::bool_<contains>);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // erase_key
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct erase_key_impl<Map> {
        template <typename M, typename Key>
        static constexpr decltype(auto) apply(M&& map, Key&& key) {
            return hana::unpack(
                hana::remove_if(
                    static_cast<M&&>(map).storage,
                    hana::compose(
                        hana::equal.to(static_cast<Key&&>(key)),
                        hana::first
                    )
                ),
                hana::make<Map>
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Map, Map> {
        template <typename M1, typename M2>
        static constexpr auto equal_helper(M1 const&, M2 const&, decltype(hana::false_)) {
            return hana::false_;
        }

        template <typename M1, typename M2>
        static constexpr auto equal_helper(M1 const& m1, M2 const& m2, decltype(hana::true_)) {
            return hana::all_of(hana::keys(m1), hana::demux(equal)(
                hana::partial(find, m1),
                hana::partial(find, m2)
            ));
        }

        template <typename M1, typename M2>
        static constexpr auto apply(M1 const& m1, M2 const& m2) {
            constexpr bool same_length = hana::value<decltype(
                hana::equal(hana::length(m1.storage), hana::length(m2.storage))
            )>();
            return equal_helper(m1, m2, hana::bool_<same_length>);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct find_if_impl<Map> {
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
    struct any_of_impl<Map> {
        template <typename M, typename Pred>
        static constexpr auto apply(M map, Pred pred)
        { return hana::any_of(hana::keys(map), pred); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct unpack_impl<Map> {
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
    struct to_impl<Map, F, when<_models<Foldable, F>{}()>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            return hana::fold_left(
                static_cast<Xs&&>(xs), hana::make<Map>(), hana::insert
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_HPP
