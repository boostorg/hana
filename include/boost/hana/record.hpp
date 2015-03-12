/*!
@file
Defines `boost::hana::Record`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RECORD_HPP
#define BOOST_HANA_RECORD_HPP

#include <boost/hana/fwd/record.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/product.hpp>
#include <boost/hana/searchable.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // members
    //////////////////////////////////////////////////////////////////////////
    template <typename R, typename>
    struct members_impl : members_impl<R, when<true>> { };

    template <typename R, bool condition>
    struct members_impl<R, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename R>
    struct models_impl<Record, R>
        : _integral_constant<bool,
            !is_default<members_impl<R>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for data types with a nested `hana::members_impl`
    //////////////////////////////////////////////////////////////////////////
    template <typename R>
    struct members_impl<R, when_valid<typename R::hana::members_impl>>
        : R::hana::members_impl
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model of Comparable
    //////////////////////////////////////////////////////////////////////////
    namespace record_detail {
        struct compare_members_of {
            template <typename X, typename Y, typename Member>
            constexpr decltype(auto) operator()(X&& x, Y&& y, Member&& member) const {
                auto accessor = hana::second(detail::std::forward<Member>(member));
                return hana::equal(accessor(detail::std::forward<X>(x)),
                                   accessor(detail::std::forward<Y>(y)));
            }
        };
    }

    template <typename R>
    struct equal_impl<R, R, when<_models<Record, R>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::all_of(members<R>(),
                hana::partial(record_detail::compare_members_of{},
                              detail::std::forward<X>(x),
                              detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model of Foldable
    //////////////////////////////////////////////////////////////////////////
    namespace record_detail {
        // This is equivalent to `demux`, except that `demux` can't forward
        // the `udt` because it does not know the `g`s are accessors. Hence,
        // this can result in faster code.
        struct almost_demux {
            template <typename F, typename Udt, typename ...Members>
            constexpr decltype(auto)
            operator()(F&& f, Udt&& udt, Members&& ...g) const {
                return detail::std::forward<F>(f)(
                    hana::second(detail::std::forward<Members>(g))(
                        detail::std::forward<Udt>(udt)
                    )...
                );
            }
        };
    }

    template <typename R>
    struct unpack_impl<R, when<_models<Record, R>{}>> {
        template <typename Udt, typename F>
        static constexpr decltype(auto) apply(Udt&& udt, F&& f) {
            return hana::unpack(hana::members<R>(),
                hana::partial(record_detail::almost_demux{},
                              detail::std::forward<F>(f),
                              detail::std::forward<Udt>(udt)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model of Searchable
    //////////////////////////////////////////////////////////////////////////
    namespace record_detail {
        template <typename X>
        struct get_member {
            X x;
            template <typename Member>
            constexpr decltype(auto) operator()(Member&& member) && {
                return hana::second(detail::std::forward<Member>(member))(
                    detail::std::forward<X>(x)
                );
            }
        };
    }

    template <typename R>
    struct find_impl<R, when<_models<Record, R>{}>> {
        template <typename X, typename Pred>
        static constexpr decltype(auto) apply(X&& x, Pred&& pred) {
            return hana::transform(
                hana::find(members<R>(),
                    hana::compose(detail::std::forward<Pred>(pred), first)
                ),
                record_detail::get_member<X>{detail::std::forward<X>(x)}
            );
        }
    };

    template <typename R>
    struct any_of_impl<R, when<_models<Record, R>{}>> {
        template <typename X, typename Pred>
        static constexpr decltype(auto) apply(X const&, Pred&& pred) {
            return hana::any_of(members<R>(),
                    hana::compose(detail::std::forward<Pred>(pred), first));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RECORD_HPP
