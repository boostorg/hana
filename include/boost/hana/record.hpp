/*!
@file
Defines `boost::hana::Record`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_RECORD_HPP
#define BOOST_HANA_RECORD_HPP

#include <boost/hana/fwd/record.hpp>

#include <boost/hana/comparable.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/foldable.hpp>
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
    struct members_impl<R, when<condition>> {
        static_assert(wrong<members_impl<R>>{},
        "no definition of boost::hana::members for the given data type");
    };

    template <typename R>
    struct members_impl<R, when_valid<typename R::hana::members_impl>>
        : R::hana::members_impl
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model of Comparable
    //////////////////////////////////////////////////////////////////////////
    template <typename R>
    struct models<Comparable(R), when<is_a<Record, R>{}>>
        : detail::std::true_type
    { };

    template <typename R>
    struct equal_impl<R, R, when<is_a<Record, R>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const& x, Y const& y) {
            return all(members<R>(), [&x, &y](auto&& member) -> decltype(auto) {
                auto accessor = second(detail::std::forward<decltype(member)>(member));
                return equal(accessor(x), accessor(y));
            });
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model of Foldable
    //////////////////////////////////////////////////////////////////////////
    template <typename R>
    struct models<Foldable(R), when<is_a<Record, R>{}>>
        : detail::std::true_type
    { };

    namespace record_detail {
        // This is equivalent to `demux`, except that `demux` can't forward
        // the `udt` because it does not know the `g`s are accessors. Hence,
        // this can result in faster code.
        struct almost_demux {
            template <typename F, typename Udt, typename ...Accessor>
            constexpr decltype(auto)
            operator()(F&& f, Udt&& udt, Accessor& ...g) const {
                return detail::std::forward<F>(f)(
                    detail::std::forward<Accessor>(g)(
                        detail::std::forward<Udt>(udt)
                    )...
                );
            }
        };
    }

    template <typename R>
    struct unpack_impl<R, when<is_a<Record, R>{}>> {
        template <typename Udt, typename F>
        static constexpr decltype(auto) apply(Udt&& udt, F&& f) {
            return hana::unpack(hana::members<R>(udt),
                hana::partial(record_detail::almost_demux{},
                              detail::std::forward<F>(f),
                              detail::std::forward<Udt>(udt)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model of Searchable
    //////////////////////////////////////////////////////////////////////////
    template <typename R>
    struct models<Searchable(R), when<is_a<Record, R>{}>>
        : detail::std::true_type
    { };

    template <typename R>
    struct find_impl<R, when<is_a<Record, R>{}>> {
        template <typename X, typename Pred>
        static constexpr decltype(auto) apply(X&& x, Pred&& pred) {
            return transform(
                find(members<R>(), [&pred](auto&& member) -> decltype(auto) {
                    return pred(first(detail::std::forward<decltype(member)>(member)));
                }),
                [&x](auto&& member) -> decltype(auto) {
                    return second(detail::std::forward<decltype(member)>(member))(
                        detail::std::forward<X>(x)
                    );
                }
            );
        }
    };

    template <typename R>
    struct any_impl<R, when<is_a<Record, R>{}>> {
        template <typename X, typename Pred>
        static constexpr decltype(auto) apply(X const&, Pred&& pred) {
            return any(members<R>(), [&pred](auto&& member) -> decltype(auto) {
                return pred(first(detail::std::forward<decltype(member)>(member)));
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_RECORD_HPP
