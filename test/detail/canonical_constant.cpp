/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/canonical_constant.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/auto/constant.hpp>
using namespace boost::hana;


template <typename T, T v>
struct canonical {
    static constexpr decltype(auto) get()
    { return v; }
    struct hana { using datatype = detail::CanonicalConstant<T>; };
};

namespace boost { namespace hana { namespace test {
    template <typename T>
    auto objects<detail::CanonicalConstant<T>> = make<Tuple>(
        ::canonical<T, 0>{},
        ::canonical<T, 1>{},
        ::canonical<T, 2>{},
        ::canonical<T, 3>{}
    );

    template <typename T>
    auto instances<detail::CanonicalConstant<T>> = make<Tuple>(
        type<Constant>
    );
}}}


int main() {
    test::check_datatype<detail::CanonicalConstant<int>>();
    test::check_datatype<detail::CanonicalConstant<long>>();
}
