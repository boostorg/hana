// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/view.hpp>
namespace hana = boost::hana;


template <int> struct undefined { };

int main() {
    {
        undefined<0> x{};
        auto single = hana::detail::single_view(x);
        BOOST_HANA_CONSTANT_CHECK(hana::not_(hana::is_empty(single)));
    }
}
