/*
@copyright Jason Rice 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
<% if input_size > 10 %>
    #define BOOST_MPL_LIMIT_MAP_SIZE <%= ((input_size + 9) / 10) * 10 %>
<% end %>

#include <boost/mpl/at.hpp>
#include <boost/mpl/map.hpp>
namespace mpl = boost::mpl;


template <int>
struct x { };

struct undefined {};

using map = mpl::map<
    <%= (0...input_size).map { |n| "mpl::pair<x<#{n}>, undefined>" }
      .join(', ') %>
>;
using result = typename mpl::at<map, x<<%= input_size-1 %>>>::type;

int main() { }
