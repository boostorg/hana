/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_TEST_SEQ_HPP
#define BOOST_HANA_TEST_TEST_SEQ_HPP

#include <boost/hana/concept/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/concept/monad_plus.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace hana {
    namespace test {
        struct Seq;

        template <typename Storage>
        struct seq_type {
            explicit constexpr seq_type(Storage s) : storage(s) { }
            Storage storage;
            using hana_tag = Seq;
        };

        struct seq_t {
            template <typename ...Xs>
            constexpr decltype(auto) operator()(Xs ...xs) const {
                auto storage = hana::make_tuple(xs...);
                return seq_type<decltype(storage)>(storage);
            }
        };
        constexpr seq_t seq{};
    }

    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //
    // Define either one to select which MCD is used:
    //  BOOST_HANA_TEST_FOLDABLE_FOLD_LEFT_MCD
    //  BOOST_HANA_TEST_FOLDABLE_UNPACK_MCD
    //  BOOST_HANA_TEST_FOLDABLE_ITERABLE_MCD
    //
    // If neither is defined, the MCD used is unspecified.
    //////////////////////////////////////////////////////////////////////////
#ifdef BOOST_HANA_TEST_FOLDABLE_FOLD_LEFT_MCD
    template <>
    struct fold_left_impl<test::Seq> {
        template <typename Xs, typename S, typename F>
        static constexpr auto apply(Xs xs, S s, F f) {
            return hana::fold_left(xs.storage, s, f);
        }

        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f) {
            return hana::fold_left(xs.storage, f);
        }
    };
#elif defined(BOOST_HANA_TEST_FOLDABLE_ITERABLE_MCD)
    template <>
    struct length_impl<test::Seq> {
        template <typename Xs>
        static constexpr auto apply(Xs const& xs) {
            return hana::length(xs.storage);
        }
    };
#else
    template <>
    struct unpack_impl<test::Seq> {
        template <typename Xs, typename F>
        static constexpr auto apply(Xs xs, F f)
        { return hana::unpack(xs.storage, f); }
    };
#endif

    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<test::Seq> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N&& n) {
            return hana::at(static_cast<Xs&&>(xs).storage, n);
        }
    };

    template <>
    struct tail_impl<test::Seq> {
        template <typename Xs>
        static constexpr auto apply(Xs xs) {
            return hana::unpack(hana::tail(xs.storage), test::seq);
        }
    };

    template <>
    struct is_empty_impl<test::Seq> {
        template <typename Xs>
        static constexpr auto apply(Xs xs) {
            return hana::is_empty(xs.storage);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct Sequence<test::Seq> {
        static constexpr bool value = true;
    };

    template <>
    struct make_impl<test::Seq> {
        template <typename ...Xs>
        static constexpr auto apply(Xs&& ...xs) {
            return test::seq(static_cast<Xs&&>(xs)...);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_TEST_SEQ_HPP
