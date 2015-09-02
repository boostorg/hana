/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/concept/monad_plus.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functional/iterate.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/value.hpp>
using namespace boost::hana;


struct LazyList;

template <typename X, typename Xs>
struct lazy_cons_type {
    X x;
    Xs xs;
    struct hana { using datatype = LazyList; };
};

auto lazy_cons = [](auto x, auto xs) {
    return lazy_cons_type<decltype(x), decltype(xs)>{x, xs};
};

struct lazy_nil_type { struct hana { using datatype = LazyList; }; };

constexpr lazy_nil_type lazy_nil{};

auto repeat_ = fix([](auto repeat, auto x) {
    return lazy_cons(x, make_lazy(repeat)(x));
});

namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct at_impl<LazyList> {
        template <typename Xs, typename N>
        static constexpr auto apply(Xs&& lcons, N const&) {
            constexpr std::size_t n = hana::value<N>();
            return hana::iterate<n>(hana::tail, lcons).x;
        }
    };

    template <>
    struct tail_impl<LazyList> {
        template <typename Xs>
        static constexpr auto apply(Xs lcons)
        { return eval(lcons.xs); }
    };

    template <>
    struct is_empty_impl<LazyList> {
        template <typename Xs>
        static constexpr auto apply(Xs)
        { return false_c; }

        static constexpr auto apply(lazy_nil_type)
        { return true_c; }
    };

    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    template <>
    struct prepend_impl<LazyList> {
        template <typename Xs, typename X>
        static constexpr auto apply(Xs xs, X x)
        { return lazy_cons(x, make_lazy(xs)); }
    };

    template <>
    struct empty_impl<LazyList> {
        static constexpr auto apply()
        { return lazy_nil; }
    };
}}


int main() {
    BOOST_HANA_CONSTANT_CHECK(!is_empty(repeat_(1)));
    BOOST_HANA_CONSTEXPR_CHECK(front(repeat_(1)) == 1);
    BOOST_HANA_CONSTEXPR_CHECK(at(repeat_(1), size_c<10>) == 1);
}
