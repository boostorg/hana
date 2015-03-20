/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/functional.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <iostream>
namespace hana = boost::hana;
using namespace hana::literals;


int main() {
    auto types = hana::tuple_t<short, int, short>;
    auto indexed_types = hana::zip(
        types, hana::to<hana::Tuple>(hana::range(0_c, hana::size(types)))
    );

    auto sorted_indexed_types = hana::sort_by([](auto t, auto u) {
        return hana::sizeof_(t[0_c]) < hana::sizeof_(u[0_c]);
    }, indexed_types);

    auto sorted_types = hana::transform(sorted_indexed_types, hana::head);
    auto sorted_indices = hana::transform(sorted_indexed_types, hana::last);
    using tuple = decltype(
        hana::unpack(sorted_types, hana::template_<hana::_tuple>)
    )::type;

    tuple t;
    std::cout << sizeof(t[sorted_indices[0_c]]) << std::endl;
    std::cout << sizeof(t[sorted_indices[1_c]]) << std::endl;
    std::cout << sizeof(t[sorted_indices[2_c]]) << std::endl;
}
