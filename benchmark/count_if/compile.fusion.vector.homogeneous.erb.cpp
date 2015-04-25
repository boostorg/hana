/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/fusion/include/count_if.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/push_back.hpp>


struct is_even {
    template <typename N>
    constexpr auto operator()(N n) const { return n % 2 == 0; }
};

int main() {
    auto vector = <%= fusion_vector((1..input_size).map { |n| "#{n}" }) %>;
    auto result = boost::fusion::count_if(vector, is_even{});
    (void)result;
}
