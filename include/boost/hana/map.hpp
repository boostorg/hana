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

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/decay.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/demux.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/product.hpp>
#include <boost/hana/record.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/sequence.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // _map
    //////////////////////////////////////////////////////////////////////////
    template <typename ...Pairs>
    struct _map : operators::adl {
        _tuple<Pairs...> storage;
        using hana = _map;
        using datatype = Map;

        _map() = default;
        _map(_map const&) = default;
        _map(_map&&) = default;
        _map(_map&) = default;
        template <typename ...Ys>
        explicit constexpr _map(Ys&& ...ys)
            : storage{static_cast<Ys&&>(ys)...}
        { }
    };

    //////////////////////////////////////////////////////////////////////////
    // make<Map>
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct make_impl<Map> {
        template <typename ...Pairs>
        static constexpr auto apply(Pairs&& ...pairs) {
            return _map<typename detail::std::decay<Pairs>::type...>{
                static_cast<Pairs&&>(pairs)...
            };
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // keys
    //////////////////////////////////////////////////////////////////////////
    //! @cond
    template <typename Map>
    constexpr decltype(auto) _keys::operator()(Map&& map) const {
        return hana::transform(static_cast<Map&&>(map).storage, first);
    }

    //////////////////////////////////////////////////////////////////////////
    // values
    //////////////////////////////////////////////////////////////////////////
    template <typename Map>
    constexpr decltype(auto) _values::operator()(Map&& map) const {
        return hana::transform(static_cast<Map&&>(map).storage, second);
    }

    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct operators::of<Map>
        : operators::of<Comparable>
    { };

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct equal_impl<Map, Map> {
        template <typename M1, typename M2>
        static constexpr auto apply(M1 const& m1, M2 const& m2) {
            return hana::and_(
                hana::equal(hana::length(m1.storage), hana::length(m2.storage)),
                hana::all_of(hana::keys(m1), hana::demux(equal)(
                    hana::partial(find, m1),
                    hana::partial(find, m2)
                ))
            );
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
                    hana::compose(static_cast<Pred&&>(pred), first)),
                second
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
    // Conversions
    //////////////////////////////////////////////////////////////////////////
    namespace map_detail {
        struct extract {
            template <typename X, typename Member>
            constexpr decltype(auto) operator()(X&& x, Member&& member) const {
                using P = typename datatype<Member>::type;
                return hana::make<P>(
                    hana::first(static_cast<Member&&>(member)),
                    hana::second(static_cast<Member&&>(member))(
                        static_cast<X&&>(x)
                    )
                );
            }
        };
    }

    template <typename R>
    struct to_impl<Map, R, when<_models<Record, R>{}>> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x) {
            return hana::to<Map>(
                hana::transform(members<R>(),
                    hana::partial(map_detail::extract{},
                                  static_cast<X&&>(x))));
        }
    };

    template <typename F>
    struct to_impl<Map, F, when<_models<Foldable, F>{} && !_models<Record, F>{}>> {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::unpack(static_cast<Xs&&>(xs), make<Map>); }
    };

    template <typename S>
    struct to_impl<S, Map, when<_models<Sequence, S>{}>> {
        template <typename M>
        static constexpr decltype(auto) apply(M&& m)
        { return hana::to<S>(static_cast<M&&>(m).storage); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAP_HPP
