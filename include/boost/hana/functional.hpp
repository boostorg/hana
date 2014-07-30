/*!
@file
Defines the @ref group-functional module.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_HPP
#define BOOST_HANA_FUNCTIONAL_HPP

#include <boost/hana/detail/at_index/best.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/left_folds/variadic_unrolled.hpp>
#include <boost/hana/detail/std/size_t.hpp>


namespace boost { namespace hana {
    //! @defgroup group-functional Functional
    //! General purpose function objects.
    //! @{

    //! The identity function -- returns its argument unchanged.
    //!
    //! ### Example
    //! @snippet example/functional/id.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto id = [](auto x) {
        return x;
    };

    //! Return a constant function returning `x` regardless of the
    //! argument(s) it is invoked with.
    //!
    //! Specifically, `always(x)` is a function such that
    //! @code
    //!     always(x)(y...) == x
    //! @endcode
    //! for any `y...`.
    //!
    //! @note
    //! This can be useful to make an expression dependent in order to delay
    //! the instantiation of its type by the compiler.
    //! @snippet example/functional/always/dependent.cpp main
    //!
    //!
    //! ### Example
    //! @snippet example/functional/always/basic.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto always = [](auto x) {
        return [=](auto ...y) { return x; };
    };

    //! Invoke `f` with `x...` as arguments.
    //!
    //!
    //! @param f
    //! A function called as `f(x...)` whose result is the return
    //! value of `apply`.
    //!
    //! @param x...
    //! The arguments to call `f` with. The number of `x...` must match the
    //! arity of `f`.
    //!
    //!
    //! ### Example
    //! @snippet example/functional/apply.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto apply = [](auto f, auto ...x) {
        return f(x...);
    };

    //! Return the `n`th passed argument.
    //!
    //! Specifically, `arg<n>(x1, ..., xn, ..., xm)` is equivalent to `xn`.
    //! Note that indexing starts at 1, so `arg<1>` returns the 1st argument,
    //! `arg<2>` the 2nd and so on. Using `arg<0>` is an error. Passing
    //! less than `n` arguments to `arg<n>` is also an error.
    //!
    //!
    //! @tparam n
    //! An unsigned integer representing the argument to return. `n` must be
    //! positive (meaning nonzero).
    //!
    //! @param x...
    //! A variadic pack of arguments from which the `n`th one is returned.
    //!
    //!
    //! @internal
    //! ### Discussion: could `n` be dynamic?
    //! We could have chosen `arg` to be used like `arg(n)(x...)` instead of
    //! `arg<n>(x...)`. Provided all the arguments were of the same type, it
    //! would then be possible for `n` to only be known at runtime. However,
    //! we would then lose the ability to assert the in-boundedness of `n`
    //! statically.
    //!
    //! ### Rationale for `n` being a non-type template parameter
    //! I claim that the only interesting use case is with a compile-time
    //! `n`, which means that the usage would become `arg(int_<n>)(x...)`,
    //! which is more cumbersome to write than `arg<n>(x...)`. This is open
    //! for discussion.
    //! @endinternal
    //!
    //! ### Example
    //! @snippet example/functional/arg.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto arg = [](auto ...x) {
        unspecified
    };
#else
    template <detail::std::size_t n>
    BOOST_HANA_CONSTEXPR_LAMBDA auto arg = [](auto ...x) {
        static_assert(n > 0,
        "invalid usage of arg with n == 0");

        static_assert(sizeof...(x) >= n,
        "invalid usage of arg with too few arguments");

        // Since compilers will typically try to continue for a bit after
        // an error/static assertion, we must avoid sending the compiler in
        // a very long computation if n == 0.
        return detail::at_index::best<n == 0 ? 0 : n - 1>(x...);
    };
#endif

    namespace functional_detail {
        BOOST_HANA_CONSTEXPR_LAMBDA auto compose2 = [](auto f, auto g) {
            return [=](auto x, auto ...xs) { return f(g(x), xs...); };
        };
    }

    //! Return the composition of two functions or more.
    //!
    //! `compose` is defined inductively. When given more than two functions,
    //! `compose(f, g, h...)` is equivalent to `compose(f, compose(g, h...))`.
    //! When given two functions, `compose(f, g)` is a function such that
    //! @code
    //!     compose(f, g)(x, y...) == f(g(x), y...)
    //! @endcode
    //!
    //! If you need composition of the form `f(g(x, y...))`, use `demux` instead.
    //!
    //! @note
    //! `compose` is an associative operation; `compose(f, compose(g, h))`
    //! is equivalent to `compose(compose(f, g), h)`.
    //!
    //! @internal
    //! ### Proof of associativity
    //!
    //! @code
    //!     compose(f, compose(g, h))(x, xs...) == f(compose(g, h)(x), xs...)
    //!                                         == f(g(h(x)), xs...)
    //!
    //!     compose(compose(f, g), h)(x, xs...) == compose(f, g)(h(x), xs...)
    //!                                         == f(g(h(x)), xs...)
    //! @endcode
    //! @endinternal
    //!
    //! ### Example
    //! @snippet example/functional/compose.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto compose = [](auto f, auto g, auto ...h) {
        unspecified
    };
#else
    BOOST_HANA_CONSTEXPR_LAMBDA auto compose = [](auto f, auto g, auto ...h) {
        return detail::left_folds::variadic_unrolled(
            functional_detail::compose2, f, g, h...
        );
    };
#endif

    //! Partially apply a function to some arguments.
    //!
    //! Specifically, `partial(f, x...)` is a function such that
    //! @code
    //!     partial(f, x...)(y...) == f(x..., y...)
    //! @endcode
    //!
    //! Given the semantics, the arity of `f` must match the number of
    //! arguments passed in total, i.e. `sizeof...(x) + sizeof...(y)`.
    //!
    //! ### Example
    //! @snippet example/functional/partial.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto partial = [](auto f, auto ...x) {
        return [=](auto ...y) {
            return f(x..., y...);
        };
    };

    //! Curry a function up to the given number of arguments.
    //!
    //! [Currying][Wikipedia.currying] is a technique in which we consider a
    //! function taking multiple arguments (or, equivalently, a tuple of
    //! arguments), and turn it into a function which takes a single argument
    //! and returns a function to handle the remaining arguments. To help
    //! visualize, let's denote the type of a function `f` which takes
    //! arguments of types `X1, ..., Xn` and returns a `R` as
    //! @code
    //!     (X1, ..., Xn) -> R
    //! @endcode
    //!
    //! Then, currying is the process of taking `f` and turning it into an
    //! equivalent function (call it `g`) of type
    //! @code
    //!     X1 -> (X2 -> (... -> (Xn -> R)))
    //! @endcode
    //!
    //! This gives us the following equivalence, where `x1`, ..., `xn` are
    //! objects of type `X1`, ..., `Xn` respectively:
    //! @code
    //!     f(x1, ..., xn) == g(x1)...(xn)
    //! @endcode
    //!
    //! Currying can be useful in several situations, especially when working
    //! with higher-order functions.
    //!
    //! This `curry` utility is an implementation of currying in C++.
    //! Specifically, `curry<n>(f)` is a function such that
    //! @code
    //!     curry<n>(f)(x1)...(xn) == f(x1, ..., xn)
    //! @endcode
    //!
    //! Note that the `n` has to be specified explicitly because the existence
    //! of functions with variadic arguments in C++ make it impossible to know
    //! when currying should stop.
    //!
    //! Unlike usual currying, this implementation also allows a curried
    //! function to be called with several arguments at a time. Hence, the
    //! following always holds
    //! @code
    //!     curry<n>(f)(x1, ..., xk) == curry<n - k>(f)(x1)...(xk)
    //! @endcode
    //!
    //! Of course, this requires `k` to be less than or equal to `n`; failure
    //! to satisfy this will trigger a static assertion. This syntax is
    //! supported because it makes curried functions usable where normal
    //! functions are expected.
    //!
    //! Another "extension" is that `curry<0>(f)` is supported: `curry<0>(f)`
    //! is a nullary function; whereas the classical definition for currying
    //! seems to leave this case undefined, as nullary functions don't make
    //! much sense in purely functional languages.
    //! @snippet example/functional/curry.cpp curry0
    //!
    //!
    //! ### Example
    //! @snippet example/functional/curry.cpp main
    //!
    //! @todo
    //! Consider making `curry<0>(f)` equivalent to `f`. It might be more
    //! general or less useful if it creates subtle pitfalls.
    //!
    //! [Wikipedia.currying]: http://en.wikipedia.org/wiki/Currying
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t arity>
    constexpr auto curry = [](auto f) {
        return unspecified;
    };
#else
    namespace functional_detail {
        template <typename F, detail::std::size_t arity>
        struct curry {
            F f;

            template <typename ...X>
            constexpr auto operator()(X ...x) const {
                static_assert(arity >= sizeof...(x),
                "too many arguments provided to boost::hana::curry");

                auto g = partial(f, x...);
                return curry<decltype(g), arity - sizeof...(x)>{g}();
            }

            constexpr auto operator()() const
            { return *this; }
        };

        template <typename F>
        struct curry<F, 0> {
            F f;

            constexpr auto operator()() const
            { return f(); }
        };
    }

    template <detail::std::size_t arity>
    BOOST_HANA_CONSTEXPR_LAMBDA auto curry = [](auto f) {
        return functional_detail::curry<decltype(f), arity>{f};
    };
#endif

    //! Invoke a function with the results of invoking other functions
    //! on its arguments.
    //!
    //! Specifically, `demux(f, g...)` is a function such that
    //! @code
    //!     demux(f, g...)(x...) == f(g(x...)...)
    //! @endcode
    //!
    //! Each `g` is called with all the arguments, and then `f` is called
    //! with the result of each `g`. Hence, the arity of `f` must match
    //! the number of `g`s.
    //!
    //! This is called `demux` because of a vague similarity between this
    //! device and a demultiplexer in signal processing. `demux` takes what
    //! can be seen as a continuation (`f`), a bunch of functions to split a
    //! signal (`g...`) and zero or more arguments representing the signal
    //! (`x...`). Then, it calls the continuation with the result of
    //! splitting the signal with whatever functions where given.
    //!
    //! @note
    //! When used with two functions only, `demux` is associative. In other
    //! words, `demux(f, demux(g, h))` is equivalent to `demux(demux(f, g), h)`.
    //!
    //! @internal
    //! ### Proof of associativity in the binary case
    //!
    //! @code
    //!     demux(f, demux(g, h))(x...) == f(demux(g, h)(x...))
    //!                                 == f(g(h(x...)))
    //!
    //!     demux(demux(f, g), h)(x...) == demux(f, g)(h(x...))
    //!                                 == f(g(h(x...)))
    //! @endcode
    //! @endinternal
    //!
    //! ### Example
    //! @snippet example/functional/demux.cpp main
    //!
    //! @todo
    //! I think this is equivalent to `fmap . fmap`.
    //! See http://stackoverflow.com/q/5821089/627587
    BOOST_HANA_CONSTEXPR_LAMBDA auto demux = [](auto f, auto ...g) {
        return [=](auto ...x) { return f(g(x...)...); };
    };

    //! Return a function computing the fixed point of a function.
    //!
    //! `fix` is an implementation of the [Y-combinator][], also called the
    //! fixed-point combinator. It encodes the idea of recursion, and in fact
    //! any recursive function can be written in terms of it.
    //!
    //! Specifically, `fix(f)` is a function such that
    //! @code
    //!     fix(f)(x...) == f(fix(f), x...)
    //! @endcode
    //!
    //! This definition allows `f` to use its first argument as a continuation
    //! to call itself recursively. Indeed, if `f` calls its first argument
    //! with `y...`, it is equivalent to calling `f(fix(f), y...)` per the
    //! above equation.
    //!
    //! Most of the time, it is more convenient and efficient to define
    //! recursive functions without using a fixed-point combinator. However,
    //! there are some cases where `fix` provides either more flexibility
    //! (e.g. the ability to change the callback inside `f`) or makes it
    //! possible to write functions that couldn't be defined recursively
    //! otherwise.
    //!
    //! @param f
    //! A function called as `f(self, x...)`, where `x...` are the arguments
    //! in the `fix(f)(x...)` expression and `self` is `fix(f)`.
    //!
    //! ### Example
    //! @snippet example/functional/fix.cpp main
    //!
    //! [Y-combinator]: http://en.wikipedia.org/wiki/Fixed-point_combinator
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto fix = [](auto f) {
        return [=](auto ...x) {
            return f(fix(f), x...);
        };
    };
#else
    namespace functional_detail {
        struct fix {
            template <typename F>
            constexpr auto operator()(F f) const {
                return [=](auto ...x) {
                    return f((*this)(always(f)(x...)), x...);
                };
            }
        };
    }

    constexpr functional_detail::fix fix{};
#endif

    //! Invoke a function with its two first arguments reversed.
    //!
    //! Specifically, `flip(f)` is a function such that
    //! @code
    //!     flip(f)(x, y, z...) == f(y, x, z...)
    //! @endcode
    //!
    //! ### Example
    //! @snippet example/functional/flip.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto flip = [](auto f) {
        return [=](auto x, auto y, auto ...z) {
            return f(y, x, z...);
        };
    };

    //! Invoke a function with the result of invoking other functions on its
    //! arguments, in lockstep.
    //!
    //! Specifically, `lockstep(f, g1, ..., gN)` is a function such that
    //! @code
    //!     lockstep(f, g1, ..., gN)(x1, ..., xN) == f(g1(x1), ..., gN(xN))
    //! @endcode
    //!
    //! Since each `g` is invoked on its corresponding argument in lockstep,
    //! the number of arguments must match the number of `g`s.
    //!
    //! ### Example
    //! @snippet example/functional/lockstep.cpp main
    //!
    //! @todo
    //! I think this is equivalent to `<*>` for `((->) r)`.
    BOOST_HANA_CONSTEXPR_LAMBDA auto lockstep = [](auto f, auto ...g) {
        return [f, g...](auto ...x) {
            return f(g(x)...);
        };
    };

    //! Invoke a function with the result of invoking another function on
    //! each argument.
    //!
    //! Specifically, `on(f, g)` is a function such that
    //! @code
    //!     on(f, g)(x...) == f(g(x)...)
    //! @endcode
    //!
    //! For convenience, `on` also supports an equivalent infix syntax
    //! @code
    //!     f /on/ g == on(f, g)
    //! @endcode
    //!
    //! The infix syntax allows a great deal of expressiveness, especially
    //! when used in combination with some higher order algorithms:
    //! @snippet example/functional/on/infix.cpp main
    //!
    //! The infix syntax is very naive and it requires the usual left-to-right
    //! associativity of the `/` operator. Hence, only `f /on/ g`, which is
    //! parsed as `(f / on) / g`, will work, and more clever parenthesizing
    //! like `f / (on / g)` will fail. This is on purpose; the goal here is
    //! not to provide a full-blown expression template engine, but simply
    //! to make some common idioms easier to write.
    //!
    //! @note
    //! `on` is associative, i.e. `on(f, on(g, h))` is equivalent to
    //! `on(on(f, g), h)`.
    //!
    //! @internal
    //! ### Proof of associativity
    //!
    //! @code
    //!     on(f, on(g, h))(xs...) == f(on(g, h)(xs)...)
    //!                            == f(g(h(xs))...)
    //!
    //!     on(on(f, g), h)(xs...) == on(f, g)(h(xs)...)
    //!                            == f(g(h(xs))...)
    //! @endcode
    //! @endinternal
    //!
    //! ### Example
    //! @snippet example/functional/on/multiply.cpp main
    //!
    //! ### Example
    //! @snippet example/functional/on/plus.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    BOOST_HANA_CONSTEXPR_LAMBDA auto on = [](auto f, auto g) {
        return [=](auto ...x) {
            return f(g(x)...);
        };
    };
#else
    namespace functional_detail {
        struct on {
            template <typename F, typename G>
            constexpr auto operator()(F f, G g) const {
                return [=](auto ...x) {
                    return f(g(x)...);
                };
            }
        };

        template <typename F>
        struct left_ok { F function; };

        template <typename F, typename G>
        constexpr auto operator/(left_ok<F> f, G g)
        { return on{}(f.function, g); }

        template <typename F>
        constexpr auto operator/(F f, on)
        { return left_ok<F>{f}; }
    }

    constexpr functional_detail::on on{};
#endif

    //! Create simple functions representing C++ operators inline.
    //!
    //! Specifically, `_` is an object used as a placeholder to build
    //! function objects representing calls to C++ operators. It works
    //! by overloading the operators between `_` and any object so that
    //! they return a function object which actually calls the corresponding
    //! operator on its argument(s). Hence, for any supported operator `@`:
    //! @code
    //!     (_ @ _)(x, y) == x @ y
    //! @endcode
    //!
    //! Operators may also be partially applied to one argument inline:
    //! @code
    //!     (x @ _)(y) == x @ y
    //!     (_ @ y)(x) == x @ y
    //! @endcode
    //!
    //! When invoked with more arguments than required, functions created with
    //! `_` will discard the superfluous instead of triggering an error:
    //! @code
    //!     (_ @ _)(x, y, z...) == x @ y
    //! @endcode
    //!
    //! This makes functions created with `_` easier to use in higher-order
    //! algorithms, which sometime provide more information than necessary
    //! to their callbacks.
    //!
    //! ### Supported operators
    //! - Arithmetic: binary `+`, binary `-`, `/`, `*`, `%`, unary `+`, unary `-`
    //! - Bitwise: `~`, `&`, `|`, `^`, `<<`, `>>`
    //! - Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
    //! - %Logical: `||`, `&&`, `!`
    //! - Member access: `*` (dereference), `[]` (array subscript)
    //! - Other: `()` (function call)
    //!
    //! More complex functionality like the ability to compose placeholders
    //! into larger function objects inline are not supported. This is on
    //! purpose; you should either use C++14 generic lambdas or a library
    //! like [Boost.Phoenix][] if you need bigger guns. The goal here is
    //! to save you a couple of characters in simple situations.
    //!
    //! ### Example
    //! @snippet example/functional/placeholder.cpp main
    //!
    //! [Boost.Phoenix]: http://www.boost.org/doc/libs/release/libs/phoenix/doc/html/index.html
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr unspecified _{};

    //! @} end of group-functional
#else
    namespace functional_detail {
        struct placeholder {
            template <typename X>
            constexpr auto operator[](X x) const
            { return [=](auto c, auto ...z) { return c[x]; }; }

            template <typename ...X>
            constexpr auto operator()(X ...x) const
            { return [=](auto f, auto ...z) { return f(x...); }; }
        };

#define BOOST_HANA_PLACEHOLDER_BINARY_OP(op)                                \
    template <typename X>                                                   \
    constexpr auto operator op (X x, placeholder)                           \
    { return [=](auto y, auto ...z) { return x op y; }; }                   \
                                                                            \
    template <typename Y>                                                   \
    constexpr auto operator op (placeholder, Y y)                           \
    { return [=](auto x, auto ...z) { return x op y; }; }                   \
                                                                            \
    BOOST_HANA_CONSTEXPR_LAMBDA auto operator op (placeholder, placeholder) \
    { return [](auto x, auto y, auto ...z) { return x op y; }; }            \
/**/

#define BOOST_HANA_PLACEHOLDER_UNARY_OP(op)                                 \
    BOOST_HANA_CONSTEXPR_LAMBDA auto operator op (placeholder)              \
    { return [](auto x, auto ...z) { return op x; }; }                      \
/**/
            // Arithmetic
            BOOST_HANA_PLACEHOLDER_UNARY_OP(+)
            BOOST_HANA_PLACEHOLDER_UNARY_OP(-)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(+)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(-)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(*)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(/)
            BOOST_HANA_PLACEHOLDER_BINARY_OP(%)

            // Bitwise
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

            // Member access (array subscript is a member function)
            BOOST_HANA_PLACEHOLDER_UNARY_OP(*)

            // Other (function call is a member function)

#undef BOOST_HANA_PREFIX_PLACEHOLDER_OP
#undef BOOST_HANA_BINARY_PLACEHOLDER_OP
    }

    constexpr functional_detail::placeholder _{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_HPP
