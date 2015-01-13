/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/generate_integer_sequence.hpp>
#include <boost/hana/detail/std/integer_sequence.hpp>

#include <cstddef>
#include <type_traits>
using namespace boost::hana;


// `generate_integer_sequence` without function is equivalent to
// `make_integer_sequence`
template <std::size_t i>
struct without_function {
    static_assert(std::is_same<
        detail::generate_integer_sequence<int, i>,
        detail::std::make_integer_sequence<int, i>
    >::value, "");
};
template struct without_function<0>;
template struct without_function<1>;
template struct without_function<2>;
template struct without_function<3>;
template struct without_function<4>;
template struct without_function<5>;



// `generate_integer_sequence` uses the provided function to create the
// `integer_sequence`.
struct ones {
    template <typename Array>
    constexpr auto operator()(Array result) const {
        for (auto i = 0; i != result.size(); ++i)
            result[i] = 1;
        return result;
    }
};
static_assert(std::is_same<
    detail::generate_integer_sequence<int, 0, ones>,
    detail::std::integer_sequence<int>
>::value, "");

static_assert(std::is_same<
    detail::generate_integer_sequence<int, 1, ones>,
    detail::std::integer_sequence<int, 1>
>::value, "");

static_assert(std::is_same<
    detail::generate_integer_sequence<int, 2, ones>,
    detail::std::integer_sequence<int, 1, 1>
>::value, "");

static_assert(std::is_same<
    detail::generate_integer_sequence<int, 3, ones>,
    detail::std::integer_sequence<int, 1, 1, 1>
>::value, "");

static_assert(std::is_same<
    detail::generate_integer_sequence<int, 4, ones>,
    detail::std::integer_sequence<int, 1, 1, 1, 1>
>::value, "");

int main() {

}
