/*!
@file
Defines `boost::hana::MonadPlus`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_PLUS_HPP
#define BOOST_HANA_MONAD_PLUS_HPP

#include <boost/hana/fwd/monad_plus.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monad.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // concat
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct concat_impl : concat_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct concat_impl<M, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // empty
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct empty_impl : empty_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct empty_impl<M, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // prepend
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct prepend_impl : prepend_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct prepend_impl<M, when<condition>> : default_ {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return hana::concat(hana::lift<M>(static_cast<X&&>(x)),
                                static_cast<Xs&&>(xs));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // append
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct append_impl : append_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct append_impl<M, when<condition>> : default_ {
        template <typename Xs, typename X>
        static constexpr decltype(auto) apply(Xs&& xs, X&& x) {
            return hana::concat(static_cast<Xs&&>(xs),
                                hana::lift<M>(static_cast<X&&>(x)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // filter
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct filter_impl : filter_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct filter_impl<M, when<condition>> : default_ {
        struct go {
            template <typename Pred, typename X>
            constexpr decltype(auto) operator()(Pred&& pred, X&& x) const {
                decltype(auto) cond = static_cast<Pred&&>(pred)(x);
                return hana::if_(static_cast<decltype(cond)&&>(cond),
                    hana::lift<M>(static_cast<X&&>(x)),
                    empty<M>()
                );
            }
        };

        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::chain(static_cast<Xs&&>(xs),
                hana::partial(go{}, static_cast<Pred&&>(pred))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // cycle
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct cycle_impl : cycle_impl<M, when<true>> { };

    namespace mdetail {
        template <typename M, detail::std::size_t n, bool = n % 2 == 0>
        struct cycle_helper;

        template <typename M>
        struct cycle_helper<M, 0, true> {
            template <typename Xs>
            static constexpr decltype(auto) apply(Xs const&)
            { return empty<M>(); }
        };

        template <typename M, detail::std::size_t n>
        struct cycle_helper<M, n, true> {
            template <typename Xs>
            static constexpr decltype(auto) apply(Xs const& xs)
            { return cycle_helper<M, n/2>::apply(hana::concat(xs, xs)); }
        };

        template <typename M, detail::std::size_t n>
        struct cycle_helper<M, n, false> {
            template <typename Xs>
            static constexpr decltype(auto) apply(Xs const& xs)
            { return hana::concat(xs, cycle_helper<M, n-1>::apply(xs)); }
        };
    }

    template <typename M, bool condition>
    struct cycle_impl<M, when<condition>> : default_ {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs const& xs, N const&) {
            constexpr detail::std::size_t n = hana::value<N>();
            return mdetail::cycle_helper<M, n>::apply(xs);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // remove_if
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct remove_if_impl : remove_if_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct remove_if_impl<M, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::filter(static_cast<Xs&&>(xs),
                        hana::compose(hana::not_, static_cast<Pred&&>(pred)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // remove
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct remove_impl : remove_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct remove_impl<M, when<condition>> : default_ {
        template <typename Xs, typename Value>
        static constexpr decltype(auto) apply(Xs&& xs, Value&& value) {
            return hana::remove_if(static_cast<Xs&&>(xs),
                        hana::equal.to(static_cast<Value&&>(value)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // repeat
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct repeat_impl : repeat_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct repeat_impl<M, when<condition>> : default_ {
        template <typename X, typename N>
        static constexpr decltype(auto) apply(X&& x, N&& n) {
            return hana::cycle(hana::lift<M>(static_cast<X&&>(x)),
                               static_cast<N&&>(n));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // prefix
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct prefix_impl : prefix_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct prefix_impl<M, when<condition>> : default_ {
        template <typename Xs, typename Z>
        static constexpr decltype(auto) apply(Xs&& xs, Z&& z) {
            return hana::chain(static_cast<Xs&&>(xs),
                hana::partial(append, hana::lift<M>(static_cast<Z&&>(z))));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // suffix
    //////////////////////////////////////////////////////////////////////////
    template <typename M, typename>
    struct suffix_impl : suffix_impl<M, when<true>> { };

    template <typename M, bool condition>
    struct suffix_impl<M, when<condition>> : default_ {
        template <typename Xs, typename Z>
        static constexpr decltype(auto) apply(Xs&& xs, Z&& z) {
            return hana::chain(static_cast<Xs&&>(xs),
                hana::partial(hana::prepend, hana::lift<M>(static_cast<Z&&>(z))));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename M>
    struct models_impl<MonadPlus, M>
        : _integral_constant<bool,
            !is_default<concat_impl<M>>{}() &&
            !is_default<empty_impl<M>>{}()
        >
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_PLUS_HPP
