/*!
@file
Defines `boost::hana::Functor`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_HPP
#define BOOST_HANA_FUNCTOR_HPP

#include <boost/hana/fwd/functor.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // transform
    //////////////////////////////////////////////////////////////////////////
    template <typename Fun, typename>
    struct transform_impl : transform_impl<Fun, when<true>> { };

    template <typename Fun, bool condition>
    struct transform_impl<Fun, when<condition>> : default_ {
        static_assert(!is_default<adjust_impl<Fun>>{},
        "no definition of boost::hana::transform for the given data type");

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::adjust(detail::std::forward<Xs>(xs),
                                hana::always(true_),
                                detail::std::forward<F>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // adjust
    //////////////////////////////////////////////////////////////////////////
    template <typename Fun, typename>
    struct adjust_impl : adjust_impl<Fun, when<true>> { };

    namespace functor_detail {
        struct go {
            template <typename F, typename X>
            static constexpr decltype(auto) helper(detail::std::true_type, F&& f, X&& x)
            { return detail::std::forward<F>(f)(detail::std::forward<X>(x)); }

            template <typename F, typename X>
            static constexpr X helper(detail::std::false_type, F&&, X&& x)
            { return detail::std::forward<X>(x); }

            template <typename F, typename X>
            static constexpr decltype(auto) helper(bool cond, F&& f, X&& x) {
                return cond ? detail::std::forward<F>(f)(detail::std::forward<X>(x))
                            : detail::std::forward<X>(x);
            }

            template <typename Pred, typename F, typename X>
            constexpr decltype(auto) operator()(Pred&& pred, F&& f, X&& x) const {
                auto cond = hana::if_(detail::std::forward<Pred>(pred)(x),
                    detail::std::true_type{},
                    detail::std::false_type{}
                );
                return go::helper(cond, detail::std::forward<F>(f),
                                        detail::std::forward<X>(x));
            }
        };
    }

    template <typename Fun, bool condition>
    struct adjust_impl<Fun, when<condition>> : default_ {
        static_assert(!is_default<transform_impl<Fun>>{},
        "no definition of boost::hana::adjust for the given data type");

        template <typename Xs, typename Pred, typename F>
        static constexpr auto apply(Xs&& xs, Pred&& pred, F&& f) {
            return hana::transform(detail::std::forward<Xs>(xs),
                hana::partial(functor_detail::go{},
                        detail::std::forward<Pred>(pred),
                        detail::std::forward<F>(f)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // replace
    //////////////////////////////////////////////////////////////////////////
    template <typename Fun, typename>
    struct replace_impl : replace_impl<Fun, when<true>> { };

    template <typename Fun, bool condition>
    struct replace_impl<Fun, when<condition>> {
        template <typename Xs, typename Pred, typename Value>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred, Value&& v) {
            return hana::adjust(detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred),
                hana::always(detail::std::forward<Value>(v))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // fill
    //////////////////////////////////////////////////////////////////////////
    template <typename Fun, typename>
    struct fill_impl : fill_impl<Fun, when<true>> { };

    template <typename Fun, bool condition>
    struct fill_impl<Fun, when<condition>> {
        template <typename Xs, typename Value>
        static constexpr decltype(auto) apply(Xs&& xs, Value&& v) {
            return hana::transform(detail::std::forward<Xs>(xs),
                                   hana::always(detail::std::forward<Value>(v))
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_HPP
