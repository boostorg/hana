/*!
@file
Defines `boost::hana::Maybe`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MAYBE_HPP
#define BOOST_HANA_MAYBE_HPP

#include <boost/hana/fwd/maybe.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp> // required by fwd decl
#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/traversable.hpp>


namespace boost { namespace hana {
    // only_when
    namespace maybe_detail {
        template <typename F, typename X>
        struct just_f_x {
            F f; X x;
            template <typename Id>
            constexpr decltype(auto) operator()(Id _) && {
                return just(_(detail::std::forward<F>(f))(
                    detail::std::forward<X>(x)
                ));
            }

            template <typename Id>
            constexpr decltype(auto) operator()(Id _) &
            { return just(_(f)(x)); }

            template <typename Id>
            constexpr decltype(auto) operator()(Id _) const&
            { return just(_(f)(x)); }
        };
    }

    template <typename Pred, typename F, typename X>
    constexpr decltype(auto) _only_when::operator()(Pred&& pred, F&& f, X&& x) const {
        return eval_if(detail::std::forward<Pred>(pred)(x),
            maybe_detail::just_f_x<F, X>{detail::std::forward<F>(f),
                                         detail::std::forward<X>(x)},
            always(nothing)
        );
    }

    // Comparable
    template <>
    struct equal_impl<Maybe, Maybe> {
        template <typename T, typename U>
        static constexpr decltype(auto) apply(_just<T> const& t, _just<U> const& u)
        { return equal(t.val, u.val); }

        static constexpr auto apply(_nothing const&, _nothing const&)
        { return true_; }

        template <typename T, typename U>
        static constexpr auto apply(T const&, U const&)
        { return false_; }
    };

    // Orderable
    template <>
    struct less_impl<Maybe, Maybe> {
        template <typename T>
        static constexpr auto apply(_nothing const&, _just<T> const&)
        { return true_; }

        static constexpr auto apply(_nothing const&, _nothing const&)
        { return false_; }

        template <typename T>
        static constexpr auto apply(_just<T> const&, _nothing const&)
        { return false_; }

        template <typename T, typename U>
        static constexpr auto apply(_just<T> const& x, _just<U> const& y)
        { return less(x.val, y.val); }
    };

    // Functor
    template <>
    struct fmap_impl<Maybe> {
        template <typename M, typename F>
        static constexpr decltype(auto) apply(M&& m, F&& f) {
            return maybe(
                nothing,
                compose(just, detail::std::forward<F>(f)),
                detail::std::forward<M>(m)
            );
        }
    };

    // Applicative
    template <>
    struct lift_impl<Maybe> {
        template <typename X>
        static constexpr decltype(auto) apply(X&& x)
        { return just(detail::std::forward<X>(x)); }
    };

    template <>
    struct ap_impl<Maybe> {
        template <typename F, typename X>
        static constexpr decltype(auto) apply_impl(F&& f, X&& x, detail::std::true_type) {
            return just(detail::std::forward<F>(f).val(detail::std::forward<X>(x).val));
        }

        template <typename F, typename X>
        static constexpr auto apply_impl(F&&, X&&, detail::std::false_type)
        { return nothing; }

        template <typename F, typename X>
        static constexpr auto apply(F&& f, X&& x) {
            auto f_is_just = is_just(f);
            auto x_is_just = is_just(x);
            return apply_impl(
                detail::std::forward<F>(f), detail::std::forward<X>(x),
                detail::std::integral_constant<bool,
                    value(f_is_just) && value(x_is_just)
                >{}
            );
        }
    };

    // Monad
    template <>
    struct flatten_impl<Maybe> {
        template <typename MMX>
        static constexpr decltype(auto) apply(MMX&& mmx) {
            return maybe(nothing, id, detail::std::forward<MMX>(mmx));
        }
    };

    // Traversable
    template <>
    struct traverse_impl<Maybe> {
        template <typename A, typename F>
        static constexpr decltype(auto) apply(_nothing const&, F&& f)
        { return lift<A>(nothing); }

        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_just<T> const& x, F&& f)
        { return fmap(detail::std::forward<F>(f)(x.val), just); }

        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_just<T>& x, F&& f)
        { return fmap(detail::std::forward<F>(f)(x.val), just); }

        template <typename A, typename T, typename F>
        static constexpr decltype(auto) apply(_just<T>&& x, F&& f)
        { return fmap(detail::std::forward<F>(f)(detail::std::move(x.val)), just); }
    };

    // Foldable
    template <>
    struct unpack_impl<Maybe> {
        template <typename M, typename F>
        static constexpr decltype(auto) apply(M&& m, F&& f)
        { return detail::std::forward<F>(f)(detail::std::forward<M>(m).val); }

        template <typename F>
        static constexpr decltype(auto) apply(_nothing const&, F&& f)
        { return detail::std::forward<F>(f)(); }

        template <typename F>
        static constexpr decltype(auto) apply(_nothing&&, F&& f)
        { return detail::std::forward<F>(f)(); }

        template <typename F>
        static constexpr decltype(auto) apply(_nothing&, F&& f)
        { return detail::std::forward<F>(f)(); }
    };

    // Searchable
    template <>
    struct find_impl<Maybe> {
        template <typename M, typename Pred>
        static constexpr decltype(auto) apply(M&& m, Pred&& pred) {
            return only_when(detail::std::forward<Pred>(pred), id,
                                        detail::std::forward<M>(m).val);
        }

        template <typename Pred>
        static constexpr decltype(auto) apply(_nothing const&, Pred&&)
        { return nothing; }

        template <typename Pred>
        static constexpr decltype(auto) apply(_nothing&&, Pred&&)
        { return nothing; }

        template <typename Pred>
        static constexpr decltype(auto) apply(_nothing&, Pred&&)
        { return nothing; }
    };

    template <>
    struct any_impl<Maybe> {
        template <typename M, typename Pred>
        static constexpr decltype(auto) apply(M&& m, Pred&& p) {
            return maybe(false_,
                detail::std::forward<Pred>(p),
                detail::std::forward<M>(m)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_MAYBE_HPP
