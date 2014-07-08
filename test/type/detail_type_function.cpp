/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/constexpr.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename ...> struct result;
struct x1; struct x2; struct x3; struct x4;

BOOST_HANA_CONSTEXPR_LAMBDA auto test = [](auto f_) {
    using f = decltype(f_);
    static_assert(std::is_same<
        typename detail::type_function<f>::template apply<>::type,
        result<>
    >::value, "");

    static_assert(std::is_same<
        typename detail::type_function<f>::template apply<x1>::type,
        result<x1>
    >::value, "");

    static_assert(std::is_same<
        typename detail::type_function<f>::template apply<x1, x2>::type,
        result<x1, x2>
    >::value, "");

    static_assert(std::is_same<
        typename detail::type_function<f>::template apply<x1, x2, x3>::type,
        result<x1, x2, x3>
    >::value, "");

    static_assert(std::is_same<
        typename detail::type_function<f>::template apply<x1, x2, x3, x4>::type,
        result<x1, x2, x3, x4>
    >::value, "");
};

struct mf_class {
    template <typename ...t>
    struct apply {
        using type = result<t...>;
    };
};

namespace boost { namespace hana { namespace detail {
    template <>
    struct is_metafunction_class<mf_class> {
        static constexpr bool value = true;
    };
}}}

int main() {
    test([](auto ...t) {
        return type<result<typename decltype(t)::type...>>;
    });

    test(mf_class{});
}
