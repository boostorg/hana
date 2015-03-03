/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/when.hpp>

#include <type_traits>
using namespace boost::hana;


namespace example1 {
//! [when]
template <typename T, typename = when<true>>
struct base_template;

template <typename T>
struct base_template<T, when<std::is_integral<T>{}>> {
    // something useful...
};
//! [when]
}

namespace example2 {
//! [when_valid]
template <typename T, typename = when<true>>
struct base_template;

template <typename T>
struct base_template<T, when_valid<typename T::value_type>> {
    // something useful...
};
//! [when_valid]
}

int main() { }
