/*!
@file
Defines `boost::hana::Functor`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTOR_HPP
#define BOOST_HANA_FUNCTOR_HPP

#include <boost/hana/fwd/functor.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
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
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::adjust_if(static_cast<Xs&&>(xs),
                                   hana::always(true_),
                                   static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // adjust_if
    //////////////////////////////////////////////////////////////////////////
    template <typename Fun, typename>
    struct adjust_if_impl : adjust_if_impl<Fun, when<true>> { };

    namespace functor_detail {
        struct go {
            template <typename F, typename X>
            static constexpr decltype(auto) helper(decltype(true_), F&& f, X&& x)
            { return static_cast<F&&>(f)(static_cast<X&&>(x)); }

            template <typename F, typename X>
            static constexpr X helper(decltype(false_), F&&, X&& x)
            { return static_cast<X&&>(x); }

            template <typename F, typename X>
            static constexpr decltype(auto) helper(bool cond, F&& f, X&& x) {
                return cond ? static_cast<F&&>(f)(static_cast<X&&>(x))
                            : static_cast<X&&>(x);
            }

            template <typename Pred, typename F, typename X>
            constexpr decltype(auto) operator()(Pred&& pred, F&& f, X&& x) const {
                auto cond = hana::if_(static_cast<Pred&&>(pred)(x),
                    true_, false_
                );
                return go::helper(cond, static_cast<F&&>(f),
                                        static_cast<X&&>(x));
            }
        };
    }

    template <typename Fun, bool condition>
    struct adjust_if_impl<Fun, when<condition>> : default_ {
        template <typename Xs, typename Pred, typename F>
        static constexpr auto apply(Xs&& xs, Pred&& pred, F&& f) {
            return hana::transform(static_cast<Xs&&>(xs),
                hana::partial(functor_detail::go{},
                        static_cast<Pred&&>(pred),
                        static_cast<F&&>(f)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // adjust
    //////////////////////////////////////////////////////////////////////////
    template <typename Fun, typename>
    struct adjust_impl : adjust_impl<Fun, when<true>> { };

    template <typename Fun, bool condition>
    struct adjust_impl<Fun, when<condition>> : default_ {
        template <typename Xs, typename Value, typename F>
        static constexpr auto apply(Xs&& xs, Value&& value, F&& f) {
            return hana::adjust_if(
                static_cast<Xs&&>(xs),
                hana::equal.to(static_cast<Value&&>(value)),
                static_cast<F&&>(f)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // replace_if
    //////////////////////////////////////////////////////////////////////////
    template <typename Fun, typename>
    struct replace_if_impl : replace_if_impl<Fun, when<true>> { };

    template <typename Fun, bool condition>
    struct replace_if_impl<Fun, when<condition>> : default_ {
        template <typename Xs, typename Pred, typename Value>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred, Value&& v) {
            return hana::adjust_if(static_cast<Xs&&>(xs),
                static_cast<Pred&&>(pred),
                hana::always(static_cast<Value&&>(v))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // replace
    //////////////////////////////////////////////////////////////////////////
    template <typename Fun, typename>
    struct replace_impl : replace_impl<Fun, when<true>> { };

    template <typename Fun, bool condition>
    struct replace_impl<Fun, when<condition>> : default_ {
        template <typename Xs, typename OldVal, typename NewVal>
        static constexpr decltype(auto)
        apply(Xs&& xs, OldVal&& oldval, NewVal&& newval) {
            return hana::replace_if(
                static_cast<Xs&&>(xs),
                hana::equal.to(static_cast<OldVal&&>(oldval)),
                static_cast<NewVal&&>(newval)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // fill
    //////////////////////////////////////////////////////////////////////////
    template <typename Fun, typename>
    struct fill_impl : fill_impl<Fun, when<true>> { };

    template <typename Fun, bool condition>
    struct fill_impl<Fun, when<condition>> : default_ {
        template <typename Xs, typename Value>
        static constexpr decltype(auto) apply(Xs&& xs, Value&& v) {
            return hana::transform(static_cast<Xs&&>(xs),
                                   hana::always(static_cast<Value&&>(v))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct models_impl<Functor, F>
        : _integral_constant<bool,
            !is_default<transform_impl<F>>{}() ||
            !is_default<adjust_if_impl<F>>{}()
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTOR_HPP
