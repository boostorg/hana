/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/push_back.hpp>
#include <boost/fusion/include/transform.hpp>


struct f {
    template <typename X>
    constexpr X operator()(X x) const { return x; }
};

struct x { };

int main() {
    auto vector = <%= fusion_vector((1..input_size).map { "x{}" }) %>;
    auto result = boost::fusion::as_vector(boost::fusion::transform(vector, f{}));
    (void)result;
}
