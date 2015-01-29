/*!
@file
Defines `boost::hana::Foldable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_HPP
#define BOOST_HANA_FOLDABLE_HPP

#include <boost/hana/fwd/foldable.hpp>

#include <boost/hana/core/method.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/detail/variadic/foldl.hpp>
#include <boost/hana/detail/variadic/foldl1.hpp>
#include <boost/hana/detail/variadic/foldr.hpp>
#include <boost/hana/detail/variadic/foldr1.hpp>
#include <boost/hana/detail/variadic/for_each.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    namespace foldable_detail {
        struct end { };

        template <typename F>
        struct fold1_helper {
            F f;

            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) const
            { return f(detail::std::forward<X>(x), detail::std::forward<Y>(y)); }

            template <typename X>
            constexpr X operator()(X&& x, end) const
            { return detail::std::forward<X>(x); }

            template <typename Y>
            constexpr Y operator()(end, Y&& y) const
            { return detail::std::forward<Y>(y); }
        };

        struct inc {
            template <typename N, typename Ignore>
            constexpr decltype(auto) operator()(N&& n, Ignore const&) const
            { return succ(detail::std::forward<N>(n)); }
        };

        struct length_helper {
            template <typename ...Xs>
            constexpr auto operator()(Xs const& ...) const
            { return size_t<sizeof...(Xs)>; }
        };

        template <typename F>
        struct for_each_helper {
            F f;
            template <typename X>
            constexpr int operator()(int ignore, X&& x) const& {
                f(detail::std::forward<X>(x));
                return 0;
            }
            template <typename X>
            constexpr int operator()(int ignore, X&& x) & {
                f(detail::std::forward<X>(x));
                return 0;
            }
            template <typename X>
            constexpr int operator()(int ignore, X&& x) && {
                detail::std::move(f)(detail::std::forward<X>(x));
                return 0;
            }
        };

        //! @todo
        //! These implementations can be more efficient when the
        //! minimal complete definition that's used is `foldl`.
#if 0
    template <typename Xs, typename F>
    static constexpr decltype(auto) foldr1_impl(Xs&& xs, F&& f) {
        decltype(auto) result = foldr(
            detail::std::forward<Xs>(xs),
            end{},
            fold1_helper<F>{detail::std::forward<F>(f)}
        );

        static_assert(!detail::std::is_same<decltype(result), end>::value,
        "boost::hana::foldr1 requires a non-empty structure");
        return result;
    }

    template <typename Xs, typename F>
    static constexpr decltype(auto) foldl1_impl(Xs&& xs, F&& f) {
        decltype(auto) result = foldl(
            detail::std::forward<Xs>(xs),
            end{},
            fold1_helper<F>{detail::std::forward<F>(f)}
        );

        static_assert(!detail::std::is_same<decltype(result), end>::value,
        "boost::hana::foldl1 requires a non-empty structure");
        return result;
    }

    template <typename Xs>
    static constexpr decltype(auto) length_impl(Xs&& xs)
    { return foldl(detail::std::forward<Xs>(xs), size_t<0>, inc{}); }

    template <typename Xs, typename F>
    static constexpr void for_each_impl(Xs&& xs, F&& f) {
        // we ignore the state all the way
        foldl(detail::std::forward<Xs>(xs), 0,
            detail::create<for_each_helper>{}(detail::std::forward<F>(f))
        );
    }
#endif
    } // end namespace foldable_detail

    template <typename T>
    struct foldl_impl<T, when<models<Foldable(T)>{}>> {
        template <typename Xs, typename S, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, S&& s, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                partial(
                    detail::variadic::foldl,
                    detail::std::forward<F>(f),
                    detail::std::forward<S>(s)
                )
            );
        }
    };

    template <typename T>
    struct foldr_impl<T, when<models<Foldable(T)>{}>> {
        template <typename Xs, typename S, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, S&& s, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                partial(
                    detail::variadic::foldr,
                    detail::std::forward<F>(f),
                    detail::std::forward<S>(s)
                )
            );
        }
    };

    template <typename T, typename>
    struct foldr1_impl {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                partial(detail::variadic::foldr1, detail::std::forward<F>(f))
            );
        }
    };

    template <typename T, typename>
    struct foldl1_impl {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                partial(detail::variadic::foldl1, detail::std::forward<F>(f))
            );
        }
    };

    template <typename T, typename>
    struct length_impl {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs const& xs)
        { return unpack(xs, foldable_detail::length_helper{}); }
    };

    template <typename T, typename>
    struct minimum_impl {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return minimum_by(less, detail::std::forward<Xs>(xs)); }
    };

    template <typename T, typename>
    struct maximum_impl {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return maximum_by(less, detail::std::forward<Xs>(xs)); }
    };

    namespace foldable_detail {
        template <typename Pred>
        struct minpred {
            Pred pred;
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) const& {
                decltype(auto) r = pred(x, y);
                return if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<X>(x),
                    detail::std::forward<Y>(y)
                );
            }
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) & {
                decltype(auto) r = pred(x, y);
                return if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<X>(x),
                    detail::std::forward<Y>(y)
                );
            }
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) && {
                decltype(auto) r = detail::std::move(pred)(x, y);
                return if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<X>(x),
                    detail::std::forward<Y>(y)
                );
            }
        };

        template <typename Pred>
        struct maxpred {
            Pred pred;
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) const& {
                decltype(auto) r = pred(x, y);
                return if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<Y>(y),
                    detail::std::forward<X>(x)
                );
            }
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) & {
                decltype(auto) r = pred(x, y);
                return if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<Y>(y),
                    detail::std::forward<X>(x)
                );
            }
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) && {
                decltype(auto) r = detail::std::move(pred)(x, y);
                return if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<Y>(y),
                    detail::std::forward<X>(x)
                );
            }
        };
    }

    template <typename T, typename>
    struct minimum_by_impl {
        template <typename Pred, typename Xs>
        static constexpr decltype(auto) apply(Pred&& pred, Xs&& xs) {
            return foldl1(detail::std::forward<Xs>(xs),
                detail::create<foldable_detail::minpred>{}(
                    detail::std::forward<Pred>(pred)
                )
            );
        }
    };

    template <typename T, typename>
    struct maximum_by_impl {
        template <typename Pred, typename Xs>
        static constexpr decltype(auto) apply(Pred&& pred, Xs&& xs) {
            return foldl1(detail::std::forward<Xs>(xs),
                detail::create<foldable_detail::maxpred>{}(
                    detail::std::forward<Pred>(pred)
                )
            );
        }
    };


    template <typename T, typename>
    struct sum_impl {
        //! @todo Make it possible to specify the Monoid that's used?
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using M = Integral<int>;
            return foldl(detail::std::forward<Xs>(xs), zero<M>(), plus);
        }
    };


    template <typename T, typename>
    struct product_impl {
        //! @todo Make it possible to specify the Ring that's used?
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using R = Integral<int>;
            return foldl(detail::std::forward<Xs>(xs), one<R>(), mult);
        }
    };


    namespace foldable_detail {
        template <typename Pred>
        struct countpred {
            Pred pred;
            template <typename Counter, typename X>
            constexpr decltype(auto) operator()(Counter&& counter, X&& x) const& {
                return if_(pred(detail::std::forward<X>(x)),
                    succ(counter),
                    counter
                );
            }
            template <typename Counter, typename X>
            constexpr decltype(auto) operator()(Counter&& counter, X&& x) & {
                return if_(pred(detail::std::forward<X>(x)),
                    succ(counter),
                    counter
                );
            }
            template <typename Counter, typename X>
            constexpr decltype(auto) operator()(Counter&& counter, X&& x) && {
                return if_(detail::std::move(pred)(detail::std::forward<X>(x)),
                    succ(counter),
                    counter
                );
            }
        };
    }

    template <typename T, typename>
    struct count_impl {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return foldl(detail::std::forward<Xs>(xs), size_t<0>,
                detail::create<foldable_detail::countpred>{}(
                    detail::std::forward<Pred>(pred)
                )
            );
        }
    };

    template <typename T>
    struct unpack_impl<T, when<models<Foldable(T)>{}>> {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return foldl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f),
                partial
            )();
        }
    };

    template <typename T, typename>
    struct for_each_impl {
        template <typename Xs, typename F>
        static constexpr void apply(Xs&& xs, F&& f) {
            unpack(detail::std::forward<Xs>(xs),
                partial(detail::variadic::for_each, detail::std::forward<F>(f))
            );
        }
    };

    template <typename T, detail::std::size_t N>
    struct unpack_impl<T[N]> {
        template <typename Xs, typename F, detail::std::size_t ...i>
        static constexpr decltype(auto)
        helper(Xs&& xs, F&& f, detail::std::index_sequence<i...>) {
            return detail::std::forward<F>(f)(
                detail::std::forward<T>(xs[i])...
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f),
                detail::std::make_index_sequence<N>{}
            );
        }
    };

    template <typename T, detail::std::size_t N>
    struct models<Foldable(T[N])> : detail::std::true_type { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_HPP
