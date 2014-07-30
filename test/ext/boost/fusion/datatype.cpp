/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/boost/fusion.hpp>

#include <boost/hana/core/datatype.hpp>

#include "helper.hpp"
#include <boost/fusion/container/generation/make_set.hpp>
#include <type_traits>
using namespace boost::hana;


template <bool should>
auto test = [](auto container) {
    static_assert(std::is_same<
        datatype_t<decltype(container())>,
        FusionNonAssociativeForwardSequence
    >::value == should, "");

    static_assert(std::is_same<
        datatype_t<decltype(container(1))>,
        FusionNonAssociativeForwardSequence
    >::value == should, "");

    static_assert(std::is_same<
        datatype_t<decltype(container(1, '2'))>,
        FusionNonAssociativeForwardSequence
    >::value == should, "");

    static_assert(std::is_same<
        datatype_t<decltype(container(1, '2', 3.3))>,
        FusionNonAssociativeForwardSequence
    >::value == should, "");
};

int main() {
    with_nonassociative_forward_sequences(test<true>);
    test<false>([](auto ...xs) { return boost::fusion::make_set(xs...); });
    test<false>([](auto ...xs) { return int{1}; });
    test<false>([](auto ...xs) { return; });
}
