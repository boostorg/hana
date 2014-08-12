/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_SEQ_FOLDABLE_HPP
#define BOOST_HANA_TEST_TEST_SEQ_FOLDABLE_HPP

// Define either one to select which MCD is used:
//  BOOST_HANA_TEST_FOLDABLE_FOLDS_MCD
//  BOOST_HANA_TEST_FOLDABLE_UNPACK_MCD
//  BOOST_HANA_TEST_FOLDABLE_ITERABLE_MCD
//
// If neither is defined, the MCD used is unspecified.


#if defined(BOOST_HANA_TEST_FOLDABLE_FOLDS_MCD)

#include <boost/hana/foldable/folds_mcd.hpp>
#include <test/seq/seq.hpp>


namespace boost { namespace hana {
    template <>
    struct Foldable::instance<test::Seq>
        : Foldable::folds_mcd
    {
        template <typename F, typename S, typename X, typename ...Xs>
        static constexpr auto foldr_helper(F f, S s, X x, Xs ...xs)
        { return f(x, foldr_helper(f, s, xs...)); }

        template <typename F, typename S>
        static constexpr auto foldr_helper(F f, S s)
        { return s; }

        template <typename Xs, typename S, typename F>
        static constexpr auto foldr_impl(Xs xs, S s, F f) {
            return xs.storage([=](auto ...xs) {
                return foldr_helper(f, s, xs...);
            });
        }

        template <typename F, typename S, typename X, typename ...Xs>
        static constexpr auto foldl_helper(F f, S s, X x, Xs ...xs)
        { return foldl_helper(f, f(s, x), xs...); }

        template <typename F, typename S>
        static constexpr auto foldl_helper(F f, S s)
        { return s; }

        template <typename Xs, typename S, typename F>
        static constexpr auto foldl_impl(Xs xs, S s, F f) {
            return xs.storage([=](auto ...xs) {
                return foldl_helper(f, s, xs...);
            });
        }
    };
}}

#elif defined(BOOST_HANA_TEST_FOLDABLE_UNPACK_MCD)

#include <boost/hana/foldable/unpack_mcd.hpp>
#include <test/seq/seq.hpp>


namespace boost { namespace hana {
    template <>
    struct Foldable::instance<test::Seq>
        : Foldable::unpack_mcd
    {
        template <typename Xs, typename F>
        static constexpr auto unpack_impl(Xs xs, F f)
        { return xs.storage(f); }
    };
}}

#else

#include <test/seq/iterable.hpp>
#include <boost/hana/foldable/iterable_mcd.hpp>

#endif

#endif //! BOOST_HANA_TEST_TEST_SEQ_FOLDABLE_HPP
