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

#include <boost/hana/core/models.hpp>
#include <boost/hana/detail/create.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/detail/variadic/foldl.hpp>
#include <boost/hana/detail/variadic/foldl1.hpp>
#include <boost/hana/detail/variadic/foldr.hpp>
#include <boost/hana/detail/variadic/foldr1.hpp>
#include <boost/hana/detail/variadic/for_each.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    //! @details
    //! Minimal complete definition: `foldl` and `foldr`
    //!
    //! @bug
    //! The functions in this class do not perfectly forward their captures
    //! when it would be possible to do so, because the language does not make
    //! it possible.
    struct Foldable::folds_mcd {
    private:
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

    public:
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

        struct inc {
            template <typename N, typename Ignore>
            constexpr decltype(auto) operator()(N&& n, Ignore const&) const {
                return succ(detail::std::forward<N>(n));
            }
        };

        template <typename Xs>
        static constexpr decltype(auto) length_impl(Xs&& xs)
        { return foldl(detail::std::forward<Xs>(xs), size_t<0>, inc{}); }

        template <typename Xs>
        static constexpr decltype(auto) minimum_impl(Xs&& xs)
        { return minimum_by(less, detail::std::forward<Xs>(xs)); }

        template <typename Xs>
        static constexpr decltype(auto) maximum_impl(Xs&& xs)
        { return maximum_by(less, detail::std::forward<Xs>(xs)); }

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

        template <typename Pred, typename Xs>
        static constexpr decltype(auto) minimum_by_impl(Pred&& pred, Xs&& xs) {
            return foldl1(detail::std::forward<Xs>(xs),
                detail::create<minpred>{}(detail::std::forward<Pred>(pred))
            );
        }

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

        template <typename Pred, typename Xs>
        static constexpr decltype(auto) maximum_by_impl(Pred&& pred, Xs&& xs) {
            return foldl1(detail::std::forward<Xs>(xs),
                detail::create<maxpred>{}(detail::std::forward<Pred>(pred))
            );
        }

        //! @todo Make it possible to specify the Monoid that's used?
        template <typename Xs>
        static constexpr decltype(auto) sum_impl(Xs&& xs) {
            using M = Integral<int>;
            return foldl(detail::std::forward<Xs>(xs), zero<M>(), plus);
        }

        //! @todo Make it possible to specify the Ring that's used?
        template <typename Xs>
        static constexpr decltype(auto) product_impl(Xs&& xs) {
            using R = Integral<int>;
            return foldl(detail::std::forward<Xs>(xs), one<R>(), mult);
        }

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

        template <typename Xs, typename Pred>
        static constexpr decltype(auto) count_impl(Xs&& xs, Pred&& pred) {
            return foldl(detail::std::forward<Xs>(xs), size_t<0>,
                detail::create<countpred>{}(detail::std::forward<Pred>(pred))
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) unpack_impl(Xs&& xs, F&& f) {
            return foldl(detail::std::forward<Xs>(xs), detail::std::forward<F>(f), partial)();
        }

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

        template <typename Xs, typename F>
        static constexpr decltype(auto) for_each_impl(Xs&& xs, F&& f) {
            // we ignore the state all the way
            foldl(detail::std::forward<Xs>(xs), 0,
                detail::create<for_each_helper>{}(detail::std::forward<F>(f))
            );
        }
    };

    //! Minimal complete definition: `unpack`
    //!
    //! @bug
    //! The functions in this class do not perfectly forward their captures
    //! when it would be possible to do so, because the language does not make
    //! it possible.
    struct Foldable::unpack_mcd : Foldable::folds_mcd {
        template <typename Xs, typename S, typename F>
        static constexpr decltype(auto) foldl_impl(Xs&& xs, S&& s, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                partial(
                    detail::variadic::foldl,
                    detail::std::forward<F>(f),
                    detail::std::forward<S>(s)
                )
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) foldl1_impl(Xs&& xs, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                partial(detail::variadic::foldl1, detail::std::forward<F>(f))
            );
        }

        template <typename Xs, typename S, typename F>
        static constexpr decltype(auto) foldr_impl(Xs&& xs, S&& s, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                partial(
                    detail::variadic::foldr,
                    detail::std::forward<F>(f),
                    detail::std::forward<S>(s)
                )
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) foldr1_impl(Xs&& xs, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                partial(detail::variadic::foldr1, detail::std::forward<F>(f))
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) for_each_impl(Xs&& xs, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                partial(detail::variadic::for_each, detail::std::forward<F>(f))
            );
        }

        struct length_helper {
            template <typename ...Xs>
            constexpr auto operator()(Xs const& ...) const {
                return size_t<sizeof...(Xs)>;
            }
        };
        template <typename Xs>
        static constexpr decltype(auto) length_impl(Xs&& xs) {
            return unpack(detail::std::forward<Xs>(xs), length_helper{});
        }
    };

    //! Instance of `Foldable` for array types.
    //!
    //! Builtin arrays whose size is known can be folded as-if they were
    //! homogeneous tuples.
    //!
    //! @note
    //! Builtin arrays can't be made more than `Foldable` (e.g. `Iterable`)
    //! because they can't be empty and they also can't be returned from
    //! functions.
    template <typename T, detail::std::size_t N>
    struct Foldable::instance<T[N]>
        : Foldable::unpack_mcd
    {
        template <typename Xs, typename F, detail::std::size_t ...i>
        static constexpr decltype(auto)
        unpack_helper(Xs&& xs, F&& f, detail::std::index_sequence<i...>) {
            return detail::std::forward<F>(f)(
                detail::std::forward<T>(xs[i])...
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) unpack_impl(Xs&& xs, F&& f) {
            return unpack_helper(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f),
                detail::std::make_index_sequence<N>{}
            );
        }
    };

    template <typename T>
    struct Foldable::instance<T, when<models<Foldable(T)>{}>>
        : Foldable::unpack_mcd
    { };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_HPP
