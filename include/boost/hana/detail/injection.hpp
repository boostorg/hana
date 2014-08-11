/*!
@file
Defines `boost::hana::detail::Injection`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_INJECTION_HPP
#define BOOST_HANA_DETAIL_INJECTION_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/orderable/less_mcd.hpp>


namespace boost { namespace hana {
namespace detail {
    //! @ingroup group-details
    //! A monotonic injective function.
    //!
    //! This is used in the unit tests, where we often just need a function
    //! which preserves equality and order, but which also satisfies the
    //! following law for all `Injection`s `f` and `g`:
    //! @code
    //!     f(x) == g(x) if and only if f === g
    //! @endcode
    //! where `===` means _was created by the same call to `injection`_.
    //! This allows creating several such functions in the unit tests while
    //! conserving precious information such as the fact that
    //! `f(g(x)) != g(f(x))`.
    struct Injection { };

    namespace injection_detail {
        template <unsigned long n, typename Storage, typename Token>
        struct injection_impl {
            Storage storage;
            using hana_datatype = Injection;
        };
    }

    //! Return an injective function different from any other function.
    //! @relates Injection
    //!
    //! Specifically, `injection` must be call with a lambda expression
    //! like `injection([]{})`, and returns an injective function which
    //! compares different to any other function. The type of the lambda
    //! expression, which is unique, is used to make the returned function
    //! unique.
    //!
    //! Note that the function returned by `injection` can be called with
    //! any number of arguments, but its image is `Orderable` only when
    //! it is called with a single `Orderable` argument. Also, only
    //! elements in the same image are orderable with one another.
    BOOST_HANA_CONSTEXPR_LAMBDA auto injection = [](auto token) {
        return [](auto ...xs) {
            auto storage = [=](auto f) { return f(xs...); };
            return injection_detail::injection_impl<
                sizeof...(xs), decltype(storage), decltype(token)
            >{storage};
        };
    };
}

template <>
struct Comparable::instance<detail::Injection, detail::Injection>
    : Comparable::equal_mcd
{
    template <unsigned long n, typename X, typename Y, typename Token,
        typename = detail::std::enable_if_t<(n > 0)>>
    static constexpr auto equal_impl(
        detail::injection_detail::injection_impl<n, X, Token> x,
        detail::injection_detail::injection_impl<n, Y, Token> y
    ) {
        return x.storage([=](auto x, auto ...xs) {
            return y.storage([=](auto y, auto ...ys) {
                return and_(
                    equal(x, y),
                    equal(
                        detail::injection(Token{})(xs...),
                        detail::injection(Token{})(ys...)
                    )
                );
            });
        });
    }

    template <typename X, typename Y, typename Token>
    static constexpr auto equal_impl(
        detail::injection_detail::injection_impl<0, X, Token>,
        detail::injection_detail::injection_impl<0, Y, Token>
    )
    { return true_; }

    template <typename X, typename Y>
    static constexpr auto equal_impl(X, Y)
    { return false_; }
};

template <>
struct Orderable::instance<detail::Injection, detail::Injection>
    : Orderable::less_mcd
{
    template <typename X, typename Y, typename Token>
    static constexpr auto less_impl(
        detail::injection_detail::injection_impl<1, X, Token> x,
        detail::injection_detail::injection_impl<1, Y, Token> y
    ) {
        return x.storage([=](auto x) {
            return y.storage([=](auto y) {
                return less(x, y);
            });
        });
    }
};
}}

#endif //! BOOST_HANA_DETAIL_INJECTION_HPP
