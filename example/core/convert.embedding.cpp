/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/when.hpp>

#include <vector>
using namespace boost::hana;


//! [is_embedded]
static_assert(is_embedded<int, long>{}, "");

// int -> unsigned long could cause negative values to be lost
static_assert(!is_embedded<int, unsigned int long>{}, "");

// similarly, float can't represent all the values of int
static_assert(!is_embedded<int, float>{}, "");

// OK, the conversion is lossless
static_assert(is_embedded<float, double>{}, "");
//! [is_embedded]


//! [embedding]
namespace boost { namespace hana {
    template <typename To, typename From>
    struct to_impl<std::vector<To>, std::vector<From>,
        when<is_convertible<From, To>{}()>>
        : embedding<is_embedded<From, To>{}()>
    {
        static std::vector<To> apply(std::vector<From> const& xs) {
            std::vector<To> result;
            for (auto const& x: xs)
                result.push_back(to<To>(x));
            return result;
        }
    };
}}

int main() {
    BOOST_HANA_RUNTIME_CHECK(
        to<std::vector<int>>(std::vector<float>{1.1, 2.2, 3.3})
                        ==
        std::vector<int>{1, 2, 3}
    );

    static_assert(!is_embedded<std::vector<float>, std::vector<int>>{}, "");
}
//! [embedding]
