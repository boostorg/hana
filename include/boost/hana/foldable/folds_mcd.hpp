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

#include <boost/hana/detail/std/is_same.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/monoid/monoid.hpp>
#include <boost/hana/orderable/orderable.hpp>


namespace boost { namespace hana {
    //! @details
    //! Minimal complete definition: `foldl` and `foldr`
    struct Foldable::folds_mcd {
    private:
        struct end { };

        template <typename F>
        struct fold1_helper {
            F f;

            template <typename X, typename Y>
            constexpr auto operator()(X x, Y y) const
            { return f(x, y); }

            template <typename X>
            constexpr auto operator()(X x, end) const
            { return x; }

            template <typename Y>
            constexpr auto operator()(end, Y y) const
            { return y; }
        };

    public:
        template <typename Xs, typename F>
        static constexpr auto foldr1_impl(Xs xs, F f) {
            using R = decltype(foldr(xs, end{}, fold1_helper<F>{f}));
            static_assert(!detail::std::is_same<R, end>::value,
            "boost::hana::foldr1 requires a non-empty structure");

            return foldr(xs, end{}, fold1_helper<F>{f});
        }

        template <typename Xs, typename F>
        static constexpr auto foldl1_impl(Xs xs, F f) {
            using R = decltype(foldl(xs, end{}, fold1_helper<F>{f}));
            static_assert(!detail::std::is_same<R, end>::value,
            "boost::hana::foldl1 requires a non-empty structure");

            return foldl(xs, end{}, fold1_helper<F>{f});
        }

        template <typename Xs>
        static constexpr auto length_impl(Xs xs) {
            auto plus1 = [](auto n, auto _) { return plus(n, size_t<1>); };
            return foldl(xs, size_t<0>, plus1);
        }

        template <typename Xs>
        static constexpr auto minimum_impl(Xs xs)
        { return minimum_by(less, xs); }

        template <typename Xs>
        static constexpr auto maximum_impl(Xs xs)
        { return maximum_by(less, xs); }

        template <typename Pred, typename Xs>
        static constexpr auto minimum_by_impl(Pred pred, Xs xs) {
            return foldl1(xs, [=](auto x, auto y) {
                return if_(pred(x, y), x, y);
            });
        }

        template <typename Pred, typename Xs>
        static constexpr auto maximum_by_impl(Pred pred, Xs xs) {
            return foldl1(xs, [=](auto x, auto y) {
                return if_(pred(x, y), y, x);
            });
        }

        //! @todo Make it possible to specify the Monoid that's used?
        template <typename Xs>
        static constexpr auto sum_impl(Xs xs) {
            using M = Integral;
            return foldl(xs, zero<M>, plus);
        }

        //! @todo Make it possible to specify the Ring that's used?
        template <typename Xs>
        static constexpr auto product_impl(Xs xs) {
            using R = Integral;
            return foldl(xs, one<R>, mult);
        }

        template <typename Xs, typename Pred>
        static constexpr auto count_impl(Xs xs, Pred pred) {
            return foldl(xs, size_t<0>, [=](auto counter, auto x) {
                return if_(pred(x), plus(counter, size_t<1>), counter);
            });
        }

        template <typename Xs, typename F>
        static constexpr auto unpack_impl(Xs xs, F f) {
            return foldl(xs, f, [](auto g, auto x) {
                return [=](auto ...y) { return g(x, y...); };
            })();
        }

        template <typename Xs, typename F>
        static constexpr auto for_each_impl(Xs xs, F f) {
            // we ignore the state all the way
            foldl(xs, 0, [=](auto, auto x) {
                f(x);
                return 0;
            });
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FOLDABLE_FOLDS_MCD_HPP
