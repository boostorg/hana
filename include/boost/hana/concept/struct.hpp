/*!
@file
Defines `boost::hana::Struct`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CONCEPT_STRUCT_HPP
#define BOOST_HANA_CONCEPT_STRUCT_HPP

#include <boost/hana/fwd/concept/struct.hpp>

#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/concept/functor.hpp>
#include <boost/hana/keys.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/concept/searchable.hpp>

#include <boost/hana/accessors.hpp>
#include <boost/hana/adapt_struct.hpp>
#include <boost/hana/define_struct.hpp>
#include <boost/hana/keys.hpp>
#include <boost/hana/members.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct models_impl<Struct, S> {
        static constexpr bool value = !is_default<accessors_impl<S>>::value;
    };

    //////////////////////////////////////////////////////////////////////////
    // Model of Comparable
    //////////////////////////////////////////////////////////////////////////
    namespace struct_detail {
        struct compare_members_of {
            template <typename X, typename Y, typename Member>
            constexpr decltype(auto) operator()(X&& x, Y&& y, Member&& member) const {
                auto accessor = hana::second(static_cast<Member&&>(member));
                return hana::equal(accessor(static_cast<X&&>(x)),
                                   accessor(static_cast<Y&&>(y)));
            }
        };
    }

    template <typename S>
    struct equal_impl<S, S, when<_models<Struct, S>::value>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::all_of(hana::accessors<S>(),
                hana::partial(struct_detail::compare_members_of{},
                              static_cast<X&&>(x),
                              static_cast<Y&&>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model of Foldable
    //////////////////////////////////////////////////////////////////////////
    namespace struct_detail {
        // This is equivalent to `demux`, except that `demux` can't forward
        // the `udt` because it does not know the `g`s are accessors. Hence,
        // this can result in faster code.
        struct almost_demux {
            template <typename F, typename Udt, typename ...Members>
            constexpr decltype(auto)
            operator()(F&& f, Udt&& udt, Members&& ...g) const {
                return static_cast<F&&>(f)(hana::make_pair(
                    hana::first(static_cast<Members&&>(g)),
                    hana::second(static_cast<Members&&>(g))
                                                (static_cast<Udt&&>(udt))
                )...);
            }
        };
    }

    template <typename S>
    struct unpack_impl<S, when<_models<Struct, S>::value>> {
        template <typename Udt, typename F>
        static constexpr decltype(auto) apply(Udt&& udt, F&& f) {
            return hana::unpack(hana::accessors<S>(),
                hana::partial(struct_detail::almost_demux{},
                              static_cast<F&&>(f),
                              static_cast<Udt&&>(udt)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model of Searchable
    //////////////////////////////////////////////////////////////////////////
    namespace struct_detail {
        template <typename X>
        struct get_member {
            X x;
            template <typename Member>
            constexpr decltype(auto) operator()(Member&& member) && {
                return hana::second(static_cast<Member&&>(member))(
                    static_cast<X&&>(x)
                );
            }
        };
    }

    template <typename S>
    struct find_if_impl<S, when<_models<Struct, S>::value>> {
        template <typename X, typename Pred>
        static constexpr decltype(auto) apply(X&& x, Pred&& pred) {
            return hana::transform(
                hana::find_if(hana::accessors<S>(),
                    hana::compose(static_cast<Pred&&>(pred), hana::first)
                ),
                struct_detail::get_member<X>{static_cast<X&&>(x)}
            );
        }
    };

    template <typename S>
    struct any_of_impl<S, when<_models<Struct, S>::value>> {
        template <typename X, typename Pred>
        static constexpr decltype(auto) apply(X const&, Pred&& pred) {
            return hana::any_of(hana::accessors<S>(),
                    hana::compose(static_cast<Pred&&>(pred), hana::first));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_CONCEPT_STRUCT_HPP
