/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_INJECTION_HPP
#define BOOST_HANA_TEST_TEST_INJECTION_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    namespace test {
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
                struct hana { using datatype = Injection; };
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

        namespace injection_detail {
            template <int i>
            struct wknd {
                BOOST_HANA_CONSTEXPR_LAMBDA auto operator()() const {
                    return injection([]{})();
                }
            };
        }

        //! A unique object.
        //! @relates Injection
        //!
        //! Specifically, `x<i>` is an object comparing unequal to all other
        //! objects, except for `x<i>` itself. The comparison returns a
        //! compile-time `Logical`.
        template <int i>
        BOOST_HANA_CONSTEXPR_LAMBDA auto x = injection_detail::wknd<i>{}();
    } // end namespace test

    // Comparable
    template <typename _>
    struct equal_impl<test::Injection,test::Injection, _> {
        template <unsigned long n, typename X, typename Y, typename Token,
            typename = detail::std::enable_if_t<(n > 0)>>
        static constexpr auto apply(
            test::injection_detail::injection_impl<n, X, Token> x,
            test::injection_detail::injection_impl<n, Y, Token> y
        ) {
            return x.storage([=](auto x, auto ...xs) {
                return y.storage([=](auto y, auto ...ys) {
                    auto token = []{};
                    return and_(
                        equal(x, y),
                        equal(
                            test::injection(token)(xs...),
                            test::injection(token)(ys...)
                        )
                    );
                });
            });
        }

        template <typename X, typename Y, typename Token>
        static constexpr auto apply(
            test::injection_detail::injection_impl<0, X, Token>,
            test::injection_detail::injection_impl<0, Y, Token>
        )
        { return true_; }

        template <typename X, typename Y>
        static constexpr auto apply(X, Y)
        { return false_; }
    };

    // Orderable
    template <>
    struct Orderable::instance<test::Injection, test::Injection>
        : Orderable::less_mcd
    {
        template <typename X, typename Y, typename Token>
        static constexpr auto less_impl(
            test::injection_detail::injection_impl<1, X, Token> x,
            test::injection_detail::injection_impl<1, Y, Token> y
        ) {
            return x.storage([=](auto x) {
                return y.storage([=](auto y) {
                    return less(x, y);
                });
            });
        }
    };
}} // end namespace boost::hana

#endif //! BOOST_HANA_TEST_TEST_INJECTION_HPP
