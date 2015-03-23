/*!
@file
Defines `boost::hana::iterate`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_ITERATE_HPP
#define BOOST_HANA_FUNCTIONAL_ITERATE_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/functional/partial.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Applies another function `n` times to its argument.
    //!
    //! Given a function `f` and an argument `x`, `iterate<n>(f, x)` returns
    //! the result of applying `f` `n` times to its argument. In other words,
    //! @code
    //!     iterate<n>(f, x) == f(f( ... f(x)))
    //!                         ^^^^^^^^^^ n times total
    //! @endcode
    //!
    //! If `n == 0`, `iterate<n>(f, x)` returns the `x` argument unchanged
    //! and `f` is never applied. It is important to note that the function
    //! passed to `iterate<n>` must be a unary function. Indeed, since `f`
    //! will be called with the result of the previous `f` application, it
    //! may only take a single argument.
    //!
    //! In addition to what's documented above, `iterate` can also be
    //! partially applied to the function argument out-of-the-box. In
    //! other words, `iterate<n>(f)` is a function object applying `f`
    //! `n` times to the argument it is called with, which means that
    //! @code
    //!     iterate<n>(f)(x) == iterate<n>(f, x)
    //! @endcode
    //!
    //! This is provided for convenience, and it turns out to be especially
    //! useful in conjunction with higher-order algorithms.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a function \f$ f : T \to T \f$ and `x` and argument of data
    //! type `T`, the signature is
    //! \f$
    //!     \mathrm{iterate_n} : (T \to T) \times T \to T
    //! \f$
    //!
    //! @tparam n
    //! An unsigned integer representing the number of times that `f`
    //! should be applied to its argument.
    //!
    //! @param f
    //! A function to apply `n` times to its argument.
    //!
    //! @param x
    //! The initial value to call `f` with.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/functional/iterate.cpp main
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <std::size_t n>
    constexpr auto iterate = [](auto&& f) {
        return [perfect-capture](auto&& x) -> decltype(auto) {
            return f(f( ... f(forwarded(x))));
        };
    };
#else
    template <detail::std::size_t n, typename = when<true>>
    struct _iterate;

    template <>
    struct _iterate<0> {
        template <typename F, typename X>
        constexpr X operator()(F&&, X&& x) const
        { return detail::std::forward<X>(x); }
    };

    template <>
    struct _iterate<1> {
        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const {
            return f(detail::std::forward<X>(x));
        }
    };

    template <>
    struct _iterate<2> {
        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const {
            return f(f(detail::std::forward<X>(x)));
        }
    };

    template <>
    struct _iterate<3> {
        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const {
            return f(f(f(detail::std::forward<X>(x))));
        }
    };

    template <>
    struct _iterate<4> {
        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const {
            return f(f(f(f(detail::std::forward<X>(x)))));
        }
    };

    template <>
    struct _iterate<5> {
        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const {
            return f(f(f(f(f(detail::std::forward<X>(x))))));
        }
    };

    template <detail::std::size_t n>
    struct _iterate<n, when<(n >= 6) && (n < 12)>> {
        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const {
            return _iterate<n - 6>{}(f,
                f(f(f(f(f(f(detail::std::forward<X>(x)))))))
            );
        }
    };

    template <detail::std::size_t n>
    struct _iterate<n, when<(n >= 12) && (n < 24)>> {
        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const {
            return _iterate<n - 12>{}(f,
                f(f(f(f(f(f(f(f(f(f(f(f(
                    detail::std::forward<X>(x)
                ))))))))))))
            );
        }
    };

    template <detail::std::size_t n>
    struct _iterate<n, when<(n >= 24) && (n < 48)>> {
        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const {
            return _iterate<n - 24>{}(f,
                f(f(f(f(f(f(f(f(f(f(f(f(
                f(f(f(f(f(f(f(f(f(f(f(f(
                    detail::std::forward<X>(x)
                ))))))))))))
                ))))))))))))
            );
        }
    };

    template <detail::std::size_t n>
    struct _iterate<n, when<(n >= 48)>> {
        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const {
            return _iterate<n - 48>{}(f,
                f(f(f(f(f(f(f(f(f(f(f(f(
                f(f(f(f(f(f(f(f(f(f(f(f(
                f(f(f(f(f(f(f(f(f(f(f(f(
                f(f(f(f(f(f(f(f(f(f(f(f(
                    detail::std::forward<X>(x)
                ))))))))))))
                ))))))))))))
                ))))))))))))
                ))))))))))))
            );
        }
    };

    template <detail::std::size_t n>
    struct _make_iterate {
        template <typename F>
        constexpr decltype(auto) operator()(F&& f) const
        { return hana::partial(_iterate<n>{}, detail::std::forward<F>(f)); }

        template <typename F, typename X>
        constexpr decltype(auto) operator()(F&& f, X&& x) const {
            return _iterate<n>{}(detail::std::forward<F>(f),
                                 detail::std::forward<X>(x));
        }
    };

    template <detail::std::size_t n>
    constexpr _make_iterate<n> iterate{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_ITERATE_HPP
