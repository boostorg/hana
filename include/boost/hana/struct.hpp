/*!
@file
Defines `boost::hana::Struct`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_STRUCT_HPP
#define BOOST_HANA_STRUCT_HPP

#include <boost/hana/fwd/struct.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/struct_macros.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // accessors
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct accessors_impl : accessors_impl<S, when<true>> { };

    template <typename S, bool condition>
    struct accessors_impl<S, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // members
    //////////////////////////////////////////////////////////////////////////
    template <typename S, typename>
    struct members_impl : members_impl<S, when<true>> { };

    namespace struct_detail {
        template <typename Holder, typename Forward>
        struct members_helper {
            Holder object;
            template <typename Accessor>
            constexpr decltype(auto) operator()(Accessor&& accessor) const {
                return hana::second(static_cast<Accessor&&>(accessor))(
                    static_cast<Forward>(object)
                );
            }
        };
    }

    template <typename S, bool condition>
    struct members_impl<S, when<condition>> : default_ {
        template <typename Object>
        static constexpr auto apply(Object&& object) {
            return hana::transform(hana::accessors<S>(),
                struct_detail::members_helper<Object&, Object&&>{object}
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // keys
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct keys_impl<S, when<_models<Struct, S>{}()>> {
        template <typename Object>
        static constexpr auto apply(Object const&) {
            return hana::transform(hana::accessors<S>(), first);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename S>
    struct models_impl<Struct, S>
        : _integral_constant<bool,
            !is_default<accessors_impl<S>>{}()
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for data types with a nested `hana::accessors_impl`
    //////////////////////////////////////////////////////////////////////////
    namespace struct_detail {
        template <typename ...>
        struct is_valid { static constexpr bool value = true; };
    }

    template <typename S>
    struct accessors_impl<S, when<
        struct_detail::is_valid<typename S::hana::accessors_impl>::value
    >>
        : S::hana::accessors_impl
    { };

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
    struct equal_impl<S, S, when<_models<Struct, S>{}()>> {
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
    struct unpack_impl<S, when<_models<Struct, S>{}()>> {
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
    struct find_if_impl<S, when<_models<Struct, S>{}()>> {
        template <typename X, typename Pred>
        static constexpr decltype(auto) apply(X&& x, Pred&& pred) {
            return hana::transform(
                hana::find_if(hana::accessors<S>(),
                    hana::compose(static_cast<Pred&&>(pred), first)
                ),
                struct_detail::get_member<X>{static_cast<X&&>(x)}
            );
        }
    };

    template <typename S>
    struct any_of_impl<S, when<_models<Struct, S>{}()>> {
        template <typename X, typename Pred>
        static constexpr decltype(auto) apply(X const&, Pred&& pred) {
            return hana::any_of(hana::accessors<S>(),
                    hana::compose(static_cast<Pred&&>(pred), hana::first));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_STRUCT_HPP
