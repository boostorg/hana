/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_IDENTITY_UNIQUE_FUNCTION_HPP
#define BOOST_HANA_TEST_IDENTITY_UNIQUE_FUNCTION_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/detail/constexpr.hpp>


template <typename T>
struct Function { };

namespace boost { namespace hana {
    template <typename T>
    struct Comparable::instance<Function<T>, Function<T>>
        : Comparable::equal_mcd
    {
        template <typename F, typename G>
        static constexpr auto equal_impl(F f, G g) {
            return equal(f.value, g.value);
        }
    };
}}

template <typename X, typename Unique>
struct function_impl {
    X value;
    using hana_datatype = Function<Unique>;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto unique_function = [](auto unique) {
    return [](auto x) {
        return function_impl<decltype(x), decltype(unique)>{x};
    };
};

#endif //! BOOST_HANA_TEST_IDENTITY_UNIQUE_FUNCTION_HPP
