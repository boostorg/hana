/*!
 * @file
 * Defines functional utilities.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_HPP
#define BOOST_HANA_FUNCTIONAL_HPP

namespace boost { namespace hana {
    template <template <typename ...> class f>
    struct _lift {
        template <typename ...Args>
        f<Args...> operator()(Args ...) const;
    };

    template <template <typename ...> class f>
    constexpr auto lift = _lift<f>{};

    constexpr struct _apply {
        template <typename F, typename ...Args>
        constexpr auto operator()(F f, Args ...args) const
        { return f(args...); }
    } apply{};


    constexpr struct _always {
    private:
        template <typename X>
        struct alwayser {
            X x;
            template <typename ...Y>
            constexpr auto operator()(Y ...) const
            { return x; }
        };

    public:
        template <typename X>
        constexpr auto operator()(X x) const
        { return alwayser<X>{x}; }
    } always{};

    constexpr struct _id {
        template <typename X>
        constexpr auto operator()(X x) const
        { return x; }
    } id{};

    constexpr struct _compose {
    private:
        template <typename F, typename G>
        struct composer {
            F f; G g;
            template <typename X, typename ...Xs>
            constexpr auto operator()(X x, Xs ...xs) const
            { return f(g(x), xs...); }
        };

    public:
        template <typename F, typename G>
        constexpr auto operator()(F f, G g) const
        { return composer<F, G>{f, g}; }
    } compose{};

    constexpr struct _flip {
    private:
        template <typename F>
        struct flipper {
            F f;
            template <typename X, typename Y, typename ...Ys>
            constexpr auto operator()(X x, Y y, Ys ...ys) const
            { return f(y, x, ys...); }
        };

    public:
        template <typename F>
        constexpr flipper<F> operator()(F f) const
        { return {f}; }
    } flip{};

    constexpr struct _partial {
        template <typename F, typename ...Args>
        constexpr auto operator()(F f, Args ...xs) const
        { return [=](auto ...ys) { return f(xs..., ys...); }; }
    } partial{};
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_HPP
