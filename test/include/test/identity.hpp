/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_IDENTITY_HPP
#define BOOST_HANA_TEST_TEST_IDENTITY_HPP

#include <boost/hana/detail/create.hpp>
#include <boost/hana/logical.hpp>

// instances
#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/orderable.hpp>


namespace boost { namespace hana {
    namespace test {
        struct Identity;

        template <typename T>
        struct _identity {
            T value;
            struct hana { using datatype = Identity; };
        };

        constexpr detail::create<_identity> identity{};
    }

    template <>
    struct Comparable::instance<test::Identity, test::Identity>
        : Comparable::equal_mcd
    {
        template <typename Id1, typename Id2>
        static constexpr auto equal_impl(Id1 x, Id2 y) {
            return equal(x.value, y.value);
        }
    };

    template <>
    struct Orderable::instance<test::Identity, test::Identity>
        : Orderable::less_mcd
    {
        template <typename Id1, typename Id2>
        static constexpr auto less_impl(Id1 x, Id2 y) {
            return less(x.value, y.value);
        }
    };

    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_FUNCTOR_FMAP_MCD
    //  BOOST_HANA_TEST_FUNCTOR_ADJUST_MCD_MCD
    //
    // If neither is defined, the MCD used is unspecified.
#ifdef BOOST_HANA_TEST_FUNCTOR_FMAP_MCD
    template <>
    struct Functor::instance<test::Identity>
        : Functor::fmap_mcd
    {
        template <typename Id, typename F>
        static constexpr auto fmap_impl(Id self, F f) {
            return test::identity(f(self.value));
        }
    };
#else
    template <>
    struct Functor::instance<test::Identity>
        : Functor::adjust_mcd
    {
        template <typename Id, typename P, typename F>
        static constexpr auto adjust_impl(Id self, P p, F f) {
            auto x = eval_if(p(self.value),
                [=](auto _) { return _(f)(self.value); },
                [=](auto _) { return self.value; }
            );
            return test::identity(x);
        }
    };
#endif

    template <>
    struct Applicative::instance<test::Identity>
        : Applicative::mcd
    {
        static constexpr auto lift_impl = test::identity;

        template <typename F, typename X>
        static constexpr auto ap_impl(F f, X x) {
            return test::identity(f.value(x.value));
        }
    };

    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_MONAD_FLATTEN_MCD
    //  BOOST_HANA_TEST_MONAD_BIND_MCD
    //
    // If neither is defined, the MCD used is unspecified.
#ifdef BOOST_HANA_TEST_MONAD_FLATTEN_MCD
    template <>
    struct Monad::instance<test::Identity>
        : Monad::flatten_mcd<test::Identity>
    {
        template <typename Id>
        static constexpr auto flatten_impl(Id self) {
            return self.value;
        }
    };
#else
    template <>
    struct Monad::instance<test::Identity>
        : Monad::bind_mcd<test::Identity>
    {
        template <typename X, typename F>
        static constexpr auto bind_impl(X x, F f) {
            return f(x.value);
        }
    };
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_IDENTITY_HPP
