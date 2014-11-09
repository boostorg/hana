/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>
namespace hana = boost::hana;


// Similar represents a relaxed equality.
struct Similar {
    BOOST_HANA_BINARY_TYPECLASS(Similar);
    struct mcd { };

    // By default, two (seemingly) unrelated things are not similar.
    template <typename T, typename U>
    struct default_instance : mcd {
        template <typename X, typename Y>
        static constexpr auto similar_impl(X x, Y y)
        { return hana::false_; }
    };
};

// Return whether `x` and `y` are similar.
// Notice that we dispatch on both data types.
BOOST_HANA_CONSTEXPR_LAMBDA auto similar = [](auto x, auto y) {
    return Similar::instance<
        hana::datatype_t<decltype(x)>,
        hana::datatype_t<decltype(y)>
    >::similar_impl(x, y);
};

// Two STL containers are similar if they are std::equal
template <typename X, typename Y>
struct Similar::instance<X, Y, hana::when_valid<
    decltype((void)std::declval<X>().begin()),
    decltype((void)std::declval<Y>().begin())
>> : Similar::mcd {
    static constexpr auto similar_impl(X x, Y y)
    { return std::equal(x.begin(), x.end(), y.begin(), y.end()); }
};

int main() {
    std::vector<char> foo_vec{'f', 'o', 'o'};
    std::string foo_str = "foo";

    // We can now compare arbitrary containers
    // (not that it's brilliant to do so)
    BOOST_HANA_RUNTIME_CHECK(similar(foo_vec, foo_str));

    // And arbitrary stuff now compare unequal.
    BOOST_HANA_CONSTANT_CHECK(!similar(1, foo_str));
}
