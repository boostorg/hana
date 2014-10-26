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

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_same.hpp>
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

        template <typename Xs>
        static constexpr decltype(auto) length_impl(Xs&& xs) {
            auto plus1 = [](auto&& n, auto const&) -> decltype(auto) {
                return succ(detail::std::forward<decltype(n)>(n));
            };
            return foldl(detail::std::forward<Xs>(xs), size_t<0>, plus1);
        }

        template <typename Xs>
        static constexpr decltype(auto) minimum_impl(Xs&& xs)
        { return minimum_by(less, detail::std::forward<Xs>(xs)); }

        template <typename Xs>
        static constexpr decltype(auto) maximum_impl(Xs&& xs)
        { return maximum_by(less, detail::std::forward<Xs>(xs)); }

        template <typename Pred, typename Xs>
        static constexpr decltype(auto) minimum_by_impl(Pred&& pred, Xs&& xs) {
            return foldl1(detail::std::forward<Xs>(xs),
                [pred(detail::std::forward<Pred>(pred))](auto&& x, auto&& y) -> decltype(auto) {
                    // Make sure that pred(x, y) is evaluated before the other
                    // arguments so we can safely std::forward them.
                    decltype(auto) r = pred(x, y);
                    return if_(detail::std::forward<decltype(r)>(r),
                        detail::std::forward<decltype(x)>(x),
                        detail::std::forward<decltype(y)>(y)
                    );
                }
            );
        }

        template <typename Pred, typename Xs>
        static constexpr decltype(auto) maximum_by_impl(Pred&& pred, Xs&& xs) {
            return foldl1(detail::std::forward<Xs>(xs),
                [pred(detail::std::forward<Pred>(pred))](auto&& x, auto&& y) -> decltype(auto) {
                    // Make sure that pred(x, y) is evaluated before the other
                    // arguments so we can safely std::forward them.
                    decltype(auto) r = pred(x, y);
                    return if_(detail::std::forward<decltype(r)>(r),
                        detail::std::forward<decltype(y)>(y),
                        detail::std::forward<decltype(x)>(x)
                    );
                }
            );
        }

        //! @todo Make it possible to specify the Monoid that's used?
        template <typename Xs>
        static constexpr decltype(auto) sum_impl(Xs&& xs) {
            using M = Integral;
            return foldl(detail::std::forward<Xs>(xs), zero<M>, plus);
        }

        //! @todo Make it possible to specify the Ring that's used?
        template <typename Xs>
        static constexpr decltype(auto) product_impl(Xs&& xs) {
            using R = Integral;
            return foldl(detail::std::forward<Xs>(xs), one<R>, mult);
        }

        template <typename Xs, typename Pred>
        static constexpr decltype(auto) count_impl(Xs&& xs, Pred&& pred) {
            return foldl(detail::std::forward<Xs>(xs), size_t<0>,
                [pred(detail::std::forward<Pred>(pred))](auto&& counter, auto&& x) -> decltype(auto) {
                    return if_(pred(detail::std::forward<decltype(x)>(x)),
                        succ(counter),
                        counter
                    );
                }
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) unpack_impl(Xs&& xs, F&& f) {
            return foldl(detail::std::forward<Xs>(xs), detail::std::forward<F>(f), partial)();
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) for_each_impl(Xs&& xs, F&& f) {
            // we ignore the state all the way
            foldl(detail::std::forward<Xs>(xs), 0,
                [f(detail::std::forward<F>(f))](int ignore, auto&& x) {
                    f(detail::std::forward<decltype(x)>(x));
                    return 0;
                }
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
                [s(detail::std::forward<S>(s)), f(detail::std::forward<F>(f))]
                (auto&& ...xs) -> decltype(auto) {
                    return detail::variadic::foldl(f, s,
                        detail::std::forward<decltype(xs)>(xs)...
                    );
                }
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) foldl1_impl(Xs&& xs, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                [f(detail::std::forward<F>(f))](auto&& ...xs) -> decltype(auto) {
                    return detail::variadic::foldl1(f,
                        detail::std::forward<decltype(xs)>(xs)...
                    );
                }
            );
        }

        template <typename Xs, typename S, typename F>
        static constexpr decltype(auto) foldr_impl(Xs&& xs, S&& s, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                [s(detail::std::forward<S>(s)), f(detail::std::forward<F>(f))]
                (auto&& ...xs) -> decltype(auto) {
                    return detail::variadic::foldr(f, s,
                        detail::std::forward<decltype(xs)>(xs)...
                    );
                }
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) foldr1_impl(Xs&& xs, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                [f(detail::std::forward<F>(f))](auto&& ...xs) -> decltype(auto) {
                    return detail::variadic::foldr1(f,
                        detail::std::forward<decltype(xs)>(xs)...
                    );
                }
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) for_each_impl(Xs&& xs, F&& f) {
            return unpack(detail::std::forward<Xs>(xs),
                partial(detail::variadic::for_each, detail::std::forward<F>(f))
            );
        }

        template <typename Xs>
        static constexpr decltype(auto) length_impl(Xs&& xs) {
            return unpack(detail::std::forward<Xs>(xs), [](auto const& ...xs) {
                return size_t<sizeof...(xs)>;
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_HPP
