/*
This file is used to as a dirty worksheet for prototyping and checking stuff
that does not need to be documented. Always leave this empty.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename ...x>
struct apply_on_impl {
    template <bool mfc, typename = void>
    struct impl {
        template <typename f>
        static constexpr auto apply(f)
        { return type<typename f::template apply<x...>::type>; }
    };

    template <typename Dummy>
    struct impl<false, Dummy> {
        template <typename F>
        static constexpr auto apply(F f)
        { return f(type<x>...); }
    };

    template <typename F>
    constexpr auto operator()(F f) const
    { return impl<detail::is_metafunction_class<F>::value>::apply(f); }
};

#define BOOST_HANA_PP_CAT_IMPL(x, y) x ## y
#define BOOST_HANA_PP_CAT(x, y) BOOST_HANA_PP_CAT_IMPL(x, y)
#define BOOST_HANA_STATIC_ASSERT__(...)                                     \
    do {                                                                    \
        auto BOOST_HANA_PP_CAT(BOOST_HANA_TMP_VAR_, __LINE__) = (__VA_ARGS__);\
        static_assert(BOOST_HANA_PP_CAT(BOOST_HANA_TMP_VAR_, __LINE__), #__VA_ARGS__);\
    } while (false)                                                         \
/**/

template <typename ...x>
constexpr apply_on_impl<x...> apply_on{};

int main() {
    BOOST_HANA_STATIC_ASSERT__(apply_on<int>(id) == type<int>);
    BOOST_HANA_STATIC_ASSERT__(apply_on<int>(metafunction<std::add_pointer>) == type<int*>);
}
