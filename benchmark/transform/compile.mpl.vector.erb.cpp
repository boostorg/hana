/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/mpl/quote.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>


template <typename X>
struct f { using type = X; };

template <int i>
struct t { };

using vector = boost::mpl::vector<
    <%= (1..input_size).to_a.map { |n| "t<#{n}>" }.join(', ') %>
>;

using result = boost::mpl::transform<vector, boost::mpl::quote1<f>>::type;


int main() { }
