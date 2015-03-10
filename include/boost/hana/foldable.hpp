/*!
@file
Defines `boost::hana::Foldable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_HPP
#define BOOST_HANA_FOLDABLE_HPP

#include <boost/hana/fwd/foldable.hpp>

#include <boost/hana/applicative.hpp>
#include <boost/hana/core/default.hpp>
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
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/ring.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // foldl
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct foldl_impl : foldl_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct foldl_impl<T, when<condition>> : default_ {
        template <typename Xs, typename S, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, S&& s, F&& f) {
            return hana::unpack(detail::std::forward<Xs>(xs),
                hana::partial(
                    detail::variadic::foldl,
                    detail::std::forward<F>(f),
                    detail::std::forward<S>(s)
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // foldlM
    //////////////////////////////////////////////////////////////////////////
    namespace foldable_detail {
        struct foldlM_helper {
            template <typename F, typename X, typename K, typename Z>
            constexpr decltype(auto) operator()(F&& f, X&& x, K&& k, Z&& z) const {
                return hana::bind(
                    detail::std::forward<F>(f)(
                        detail::std::forward<Z>(z),
                        detail::std::forward<X>(x)
                    ),
                    detail::std::forward<K>(k)
                );
            }
        };
    }

    template <typename T, typename>
    struct foldlM_impl : foldlM_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct foldlM_impl<T, when<condition>> : default_ {
        template <typename M, typename Xs, typename S, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, S&& s, F&& f) {
            return hana::foldr(
                detail::std::forward<Xs>(xs),
                hana::lift<M>,
                hana::curry<3>(hana::partial(
                    foldable_detail::foldlM_helper{}, detail::std::forward<F>(f)
                ))
            )(detail::std::forward<S>(s));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // foldr
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct foldr_impl : foldr_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct foldr_impl<T, when<condition>> : default_ {
        template <typename Xs, typename S, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, S&& s, F&& f) {
            return hana::unpack(detail::std::forward<Xs>(xs),
                hana::partial(
                    detail::variadic::foldr,
                    detail::std::forward<F>(f),
                    detail::std::forward<S>(s)
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // foldrM
    //////////////////////////////////////////////////////////////////////////
    namespace foldable_detail {
        struct foldrM_helper {
            template <typename F, typename K, typename X, typename Z>
            constexpr decltype(auto) operator()(F&& f, K&& k, X&& x, Z&& z) const {
                return hana::bind(
                    detail::std::forward<F>(f)(
                        detail::std::forward<X>(x),
                        detail::std::forward<Z>(z)
                    ),
                    detail::std::forward<K>(k)
                );
            }
        };
    }

    template <typename T, typename>
    struct foldrM_impl : foldrM_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct foldrM_impl<T, when<condition>> : default_ {
        template <typename M, typename Xs, typename S, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, S&& s, F&& f) {
            return hana::foldl(
                detail::std::forward<Xs>(xs),
                hana::lift<M>,
                hana::curry<3>(hana::partial(
                    foldable_detail::foldrM_helper{}, detail::std::forward<F>(f)
                ))
            )(detail::std::forward<S>(s));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // foldr1
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct foldr1_impl : foldr1_impl<T, when<true>> { };

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

        template <typename T, bool = is_default<unpack_impl<T>>{}>
        struct foldr1_helper {
            template <typename Xs, typename F>
            static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
                return hana::unpack(detail::std::forward<Xs>(xs),
                    hana::partial(detail::variadic::foldr1,
                                  detail::std::forward<F>(f))
                );
            }
        };

        template <typename T>
        struct foldr1_helper<T, true> {
            template <typename Xs, typename F>
            static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
                decltype(auto) result = hana::foldr(
                    detail::std::forward<Xs>(xs),
                    end{},
                    fold1_helper<F>{detail::std::forward<F>(f)}
                );

                static_assert(!detail::std::is_same<decltype(result), end>{},
                "hana::foldr1(xs, f) requires xs to be non-empty");
                return result;
            }
        };
    }

    template <typename T, bool condition>
    struct foldr1_impl<T, when<condition>>
        : foldable_detail::foldr1_helper<T>, default_
    { };


    //////////////////////////////////////////////////////////////////////////
    // foldl1
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct foldl1_impl : foldl1_impl<T, when<true>> { };

    namespace foldable_detail {
        template <typename T, bool = is_default<unpack_impl<T>>{}>
        struct foldl1_helper {
            template <typename Xs, typename F>
            static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
                return hana::unpack(detail::std::forward<Xs>(xs),
                    hana::partial(detail::variadic::foldl1,
                                  detail::std::forward<F>(f))
                );
            }
        };

        template <typename T>
        struct foldl1_helper<T, true> {
            template <typename Xs, typename F>
            static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
                decltype(auto) result = hana::foldl(
                    detail::std::forward<Xs>(xs),
                    end{},
                    fold1_helper<F>{detail::std::forward<F>(f)}
                );

                static_assert(!detail::std::is_same<decltype(result), end>{},
                "hana::foldl1(xs, f) requires xs to be non-empty");
                return result;
            }
        };
    }

    template <typename T, bool condition>
    struct foldl1_impl<T, when<condition>>
        : foldable_detail::foldl1_helper<T>, default_
    { };

    //////////////////////////////////////////////////////////////////////////
    // for_each
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct for_each_impl : for_each_impl<T, when<true>> { };

    namespace foldable_detail {
        template <typename F>
        struct for_each_fun {
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

        template <typename T, bool = is_default<unpack_impl<T>>{}>
        struct for_each_helper {
            template <typename Xs, typename F>
            static constexpr void apply(Xs&& xs, F&& f) {
                hana::unpack(detail::std::forward<Xs>(xs),
                    hana::partial(detail::variadic::for_each,
                                  detail::std::forward<F>(f))
                );
            }
        };

        template <typename T>
        struct for_each_helper<T, true> {
            template <typename Xs, typename F>
            static constexpr void apply(Xs&& xs, F&& f) {
                // we ignore the state all the way
                hana::foldl(detail::std::forward<Xs>(xs), 0,
                    detail::create<for_each_fun>{}(detail::std::forward<F>(f))
                );
            }
        };
    }

    template <typename T, bool condition>
    struct for_each_impl<T, when<condition>>
        : foldable_detail::for_each_helper<T>, default_
    { };

    //////////////////////////////////////////////////////////////////////////
    // length
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct length_impl : length_impl<T, when<true>> { };

    namespace foldable_detail {
        struct argn {
            template <typename ...Xs>
            constexpr auto operator()(Xs const& ...) const
            { return size_t<sizeof...(Xs)>; }
        };

        struct inc {
            template <typename N, typename Ignore>
            constexpr decltype(auto) operator()(N&& n, Ignore const&) const
            { return hana::succ(detail::std::forward<N>(n)); }
        };

        template <typename T, bool = is_default<unpack_impl<T>>{}>
        struct length_helper {
            template <typename Xs>
            static constexpr decltype(auto) apply(Xs const& xs)
            { return hana::unpack(xs, argn{}); }
        };

        template <typename T>
        struct length_helper<T, true> {
            template <typename Xs>
            static constexpr decltype(auto) apply(Xs&& xs) {
                return hana::foldl(detail::std::forward<Xs>(xs), size_t<0>, inc{});
            }
        };
    }

    template <typename T, bool condition>
    struct length_impl<T, when<condition>>
        : foldable_detail::length_helper<T>, default_
    { };

    //////////////////////////////////////////////////////////////////////////
    // minimum_by
    //////////////////////////////////////////////////////////////////////////
    namespace foldable_detail {
        template <typename Pred>
        struct minpred {
            Pred pred;
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) const& {
                decltype(auto) r = pred(x, y);
                return hana::if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<X>(x),
                    detail::std::forward<Y>(y)
                );
            }
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) & {
                decltype(auto) r = pred(x, y);
                return hana::if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<X>(x),
                    detail::std::forward<Y>(y)
                );
            }
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) && {
                decltype(auto) r = detail::std::move(pred)(x, y);
                return hana::if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<X>(x),
                    detail::std::forward<Y>(y)
                );
            }
        };
    }

    template <typename T, typename>
    struct minimum_by_impl : minimum_by_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct minimum_by_impl<T, when<condition>> : default_ {
        template <typename Pred, typename Xs>
        static constexpr decltype(auto) apply(Pred&& pred, Xs&& xs) {
            return hana::foldl1(detail::std::forward<Xs>(xs),
                detail::create<foldable_detail::minpred>{}(
                    detail::std::forward<Pred>(pred)
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // minimum
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct minimum_impl : minimum_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct minimum_impl<T, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::minimum_by(less, detail::std::forward<Xs>(xs)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // maximum_by
    //////////////////////////////////////////////////////////////////////////
    namespace foldable_detail {
        template <typename Pred>
        struct maxpred {
            Pred pred;
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) const& {
                decltype(auto) r = pred(x, y);
                return hana::if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<Y>(y),
                    detail::std::forward<X>(x)
                );
            }
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) & {
                decltype(auto) r = pred(x, y);
                return hana::if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<Y>(y),
                    detail::std::forward<X>(x)
                );
            }
            template <typename X, typename Y>
            constexpr decltype(auto) operator()(X&& x, Y&& y) && {
                decltype(auto) r = detail::std::move(pred)(x, y);
                return hana::if_(detail::std::forward<decltype(r)>(r),
                    detail::std::forward<Y>(y),
                    detail::std::forward<X>(x)
                );
            }
        };
    }

    template <typename T, typename>
    struct maximum_by_impl : maximum_by_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct maximum_by_impl<T, when<condition>> : default_ {
        template <typename Pred, typename Xs>
        static constexpr decltype(auto) apply(Pred&& pred, Xs&& xs) {
            return hana::foldl1(detail::std::forward<Xs>(xs),
                detail::create<foldable_detail::maxpred>{}(
                    detail::std::forward<Pred>(pred)
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // maximum
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct maximum_impl : maximum_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct maximum_impl<T, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs)
        { return hana::maximum_by(less, detail::std::forward<Xs>(xs)); }
    };

    //////////////////////////////////////////////////////////////////////////
    // sum
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct sum_impl : sum_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct sum_impl<T, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using M = IntegralConstant<int>;
            return hana::foldl(detail::std::forward<Xs>(xs), zero<M>(), plus);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // product
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct product_impl : product_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct product_impl<T, when<condition>> : default_ {
        template <typename Xs>
        static constexpr decltype(auto) apply(Xs&& xs) {
            using R = IntegralConstant<int>;
            return hana::foldl(detail::std::forward<Xs>(xs), one<R>(), mult);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // count
    //////////////////////////////////////////////////////////////////////////
    namespace foldable_detail {
        template <typename Pred>
        struct countpred {
            Pred pred;
            template <typename Counter, typename X>
            constexpr decltype(auto) operator()(Counter&& counter, X&& x) const& {
                return hana::if_(pred(detail::std::forward<X>(x)),
                    hana::succ(counter),
                    counter
                );
            }
            template <typename Counter, typename X>
            constexpr decltype(auto) operator()(Counter&& counter, X&& x) & {
                return hana::if_(pred(detail::std::forward<X>(x)),
                    hana::succ(counter),
                    counter
                );
            }
            template <typename Counter, typename X>
            constexpr decltype(auto) operator()(Counter&& counter, X&& x) && {
                return hana::if_(detail::std::move(pred)(detail::std::forward<X>(x)),
                    hana::succ(counter),
                    counter
                );
            }
        };
    }

    template <typename T, typename>
    struct count_impl : count_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct count_impl<T, when<condition>> : default_ {
        template <typename Xs, typename Pred>
        static constexpr decltype(auto) apply(Xs&& xs, Pred&& pred) {
            return hana::foldl(detail::std::forward<Xs>(xs), size_t<0>,
                detail::create<foldable_detail::countpred>{}(
                    detail::std::forward<Pred>(pred)
                )
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // unpack
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename>
    struct unpack_impl : unpack_impl<T, when<true>> { };

    template <typename T, bool condition>
    struct unpack_impl<T, when<condition>> : default_ {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return hana::foldl(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f),
                partial
            )();
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename T>
    struct models<Foldable, T>
        : detail::std::integral_constant<bool,
            (!is_default<foldl_impl<T>>{} && !is_default<foldr_impl<T>>{}) ||
            !is_default<unpack_impl<T>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for builtin arrays
    //////////////////////////////////////////////////////////////////////////
    template <typename T, detail::std::size_t N>
    struct unpack_impl<T[N]> {
        template <typename Xs, typename F, detail::std::size_t ...i>
        static constexpr decltype(auto)
        unpack_helper(Xs&& xs, F&& f, detail::std::index_sequence<i...>) {
            return detail::std::forward<F>(f)(
                detail::std::forward<Xs>(xs)[i]...
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs&& xs, F&& f) {
            return unpack_helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f),
                detail::std::make_index_sequence<N>{}
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_HPP
