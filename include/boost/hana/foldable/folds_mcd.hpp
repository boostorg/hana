/*!
@file
Defines `boost::hana::Foldable::folds_mcd`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FOLDABLE_FOLDS_MCD_HPP
#define BOOST_HANA_FOLDABLE_FOLDS_MCD_HPP

#include <boost/hana/foldable/foldable.hpp>

#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monoid/monoid.hpp>
#include <boost/hana/orderable/orderable.hpp>


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
            constexpr decltype(auto) operator()(X&& x, end) const
            { return id(detail::std::forward<X>(x)); }

            template <typename Y>
            constexpr decltype(auto) operator()(end, Y&& y) const
            { return id(detail::std::forward<Y>(y)); }
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
                return plus(detail::std::forward<decltype(n)>(n), size_t<1>);
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
                        plus(counter, size_t<1>),
                        counter
                    );
                }
            );
        }

        template <typename Xs, typename F>
        static constexpr decltype(auto) unpack_impl(Xs&& xs, F&& f) {
            return foldl(detail::std::forward<Xs>(xs), detail::std::forward<F>(f),
                [](auto&& g, auto&& x) -> decltype(auto) {
                    return [
                        g(detail::std::forward<decltype(g)>(g)),
                        x(detail::std::forward<decltype(x)>(x))
                    ](auto&& ...y) -> decltype(auto) {
                        return g(x, detail::std::forward<decltype(y)>(y)...);
                    };
                }
            )();
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
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_FOLDS_MCD_HPP
