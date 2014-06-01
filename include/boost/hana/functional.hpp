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

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functor.hpp>


namespace boost { namespace hana {
    BOOST_HANA_CONSTEXPR_LAMBDA auto apply = [](auto f, auto ...xs) {
        return f(xs...);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto always = [](auto x) {
        return [=](auto ...y) { return x; };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto id = [](auto x) { return x; };

    BOOST_HANA_CONSTEXPR_LAMBDA auto compose = [](auto f, auto g) {
        return [=](auto x, auto ...xs) { return f(g(x), xs...); };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto flip = [](auto f) {
        return [=](auto x, auto y, auto ...z) { return f(y, x, z...); };
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto partial = [](auto f, auto ...xs) {
        return [=](auto ...ys) { return f(xs..., ys...); };
    };

    constexpr struct {
        template <typename F>
        constexpr auto operator()(F f) const {
            auto fst = [](auto a, ...) { return a; };
            //! @todo Find a simpler way to make `f` dependent.
            return [=](auto ...xs) {
                return f((*this)(fst(f, xs...)), xs...);
            };
        }
    } fix{};

    template <typename F, unsigned Arity>
    struct curry_impl;

    template <unsigned arity>
    BOOST_HANA_CONSTEXPR_LAMBDA auto curry = [](auto f) {
        return curry_impl<decltype(f), arity>{f};
    };

    template <typename F, unsigned Arity>
    struct curry_impl {
        F f;

        template <typename ...Args>
        constexpr auto operator()(Args ...args) const
        { return curry<Arity - sizeof...(Args)>(partial(f, args...))(); }

        constexpr auto operator()() const
        { return *this; }
    };

    template <>
    BOOST_HANA_CONSTEXPR_LAMBDA auto curry<0> = [](auto f) {
        return f;
    };

    struct Argwise;

    template <typename F, typename G>
    struct _argwise {
        F f;
        G g;
        using hana_datatype = Argwise;

        template <typename ...Xs>
        constexpr auto operator()(Xs ...xs) const
        { return f(g(xs)...); }
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto argwise = [](auto f) {
        return _argwise<decltype(f), decltype(id)>{f, id};
    };

    template <>
    struct Functor<Argwise> {
        template <typename H, typename F, typename G>
        static constexpr auto fmap_impl(H h, _argwise<F, G> aw) {
            return _argwise<F, decltype(compose(h, aw.g))>{aw.f, compose(h, aw.g)};
        }
    };

    constexpr struct Placeholder { } _{};

#define BOOST_HANA_PLACEHOLDER_BINARY_OP(op)                                \
    template <typename X>                                                   \
    constexpr auto operator op (X x, Placeholder)                           \
    { return [=](auto y) { return x op y; }; }                              \
                                                                            \
    template <typename Y>                                                   \
    constexpr auto operator op (Placeholder, Y y)                           \
    { return [=](auto x) { return x op y; }; }                              \
                                                                            \
    BOOST_HANA_CONSTEXPR_LAMBDA auto operator op (Placeholder, Placeholder) \
    { return [](auto x, auto y) { return x op y; }; }                       \
/**/

#define BOOST_HANA_PLACEHOLDER_UNARY_OP(op)                                 \
    BOOST_HANA_CONSTEXPR_LAMBDA auto operator op (Placeholder)              \
    { return [](auto x) { return op x; }; }                                 \
/**/
    // Arithmetic
    BOOST_HANA_PLACEHOLDER_UNARY_OP(+)
    BOOST_HANA_PLACEHOLDER_UNARY_OP(-)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(+)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(-)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(*)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(/)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(%)
    BOOST_HANA_PLACEHOLDER_UNARY_OP(~)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(&)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(|)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(^)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(<<)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(>>)

    // Comparison
    BOOST_HANA_PLACEHOLDER_BINARY_OP(==)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(!=)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(<)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(<=)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(>)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(>=)

    // Logical
    BOOST_HANA_PLACEHOLDER_BINARY_OP(||)
    BOOST_HANA_PLACEHOLDER_BINARY_OP(&&)
    BOOST_HANA_PLACEHOLDER_UNARY_OP(!)

    // Member access
    BOOST_HANA_PLACEHOLDER_UNARY_OP(*)
#undef BOOST_HANA_PREFIX_PLACEHOLDER_OP
#undef BOOST_HANA_BINARY_PLACEHOLDER_OP
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_HPP
