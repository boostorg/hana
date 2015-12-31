/*
@copyright Jason Rice 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/at_key.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


struct my_pair_tag { };

template <typename First_, typename Second_>
struct my_pair {
    using First = First_;
    using Second = Second_;
    using hana_tag = my_pair_tag;
};

namespace boost { namespace hana {
    template <>
    struct first_impl<my_pair_tag> {
        template <typename First, typename Second>
        static constexpr decltype(auto) apply(my_pair<First, Second>)
        { return First{}; }
    };

    template<>
    struct second_impl<my_pair_tag> {
        template <typename First, typename Second>
        static constexpr decltype(auto) apply(my_pair<First, Second>)
        { return Second{}; }
    };
}} // end namespace boost::hana

template <int>
struct x { };

struct undefined { };

int main() {
    constexpr auto map = hana::make_map(
        <%= (0...200).map { |n|
            "my_pair<hana::type<x<#{n}>>, undefined>{}"
        }.join(', ') %>
    );
    constexpr auto result = hana::at_key(map, hana::type_c<x<<%= input_size-1 %>>>);
    (void)result;
}
