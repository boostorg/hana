/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/tail.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


struct RefTuple { };

template <typename ...T>
struct ref_tuple {
    hana::tuple<T&...> storage_;
};


namespace boost { namespace hana {
    template <typename ...T>
    struct tag_of<ref_tuple<T...>> {
        using type = RefTuple;
    };

    template <>
    struct make_impl<RefTuple> {
        template <typename ...T>
        static constexpr auto apply(T&& ...t) {
            return ref_tuple<T...>{{t...}};
        }
    };

    template <>
    struct at_impl<RefTuple> {
        template <typename Xs, typename N>
        static constexpr decltype(auto) apply(Xs&& xs, N const& n) {
            return hana::at(static_cast<Xs&&>(xs).storage_, n);
        }
    };

    template <>
    struct is_empty_impl<RefTuple> {
        template <typename Xs>
        static constexpr auto apply(Xs const& xs) {
            return hana::is_empty(xs.storage_);
        }
    };

    template <>
    struct tail_impl<RefTuple> {
        template <typename T, typename ...U, std::size_t ...n>
        static constexpr ref_tuple<U...>
        helper(ref_tuple<T, U...> xs, std::index_sequence<n...>) {
            return {{hana::at_c<n+1>(xs.storage_)...}};
        }

        template <typename T, typename ...U>
        static constexpr ref_tuple<U...> apply(ref_tuple<T, U...> xs) {
            return helper(xs, std::make_index_sequence<sizeof...(U)>{});
        }
    };
}}


int main() {
    int i = 0, j = 1, k = 2;

    auto refs = hana::make<RefTuple>(i, j, k);
    hana::at_c<0>(refs) = 3;
    BOOST_HANA_RUNTIME_CHECK(i == 3);

    BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(refs)));

    ref_tuple<int&, int&> tail = hana::tail(refs);
    hana::at_c<0>(tail) = 4;
    BOOST_HANA_RUNTIME_CHECK(j == 4);
}
