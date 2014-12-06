/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    //! [heterogeneous]
    BOOST_HANA_CONSTEXPR_LAMBDA auto safe_make_unsigned = [](auto t) {
        return eval_if(trait<std::is_integral>(t),
            [=](auto id) { return id(template_<std::make_unsigned_t>)(t); },
            always(t)
        );
    };

    BOOST_HANA_CONSTANT_CHECK(safe_make_unsigned(type<void>) == type<void>);
    BOOST_HANA_CONSTANT_CHECK(safe_make_unsigned(type<int>) == type<unsigned int>);
    //! [heterogeneous]
    (void)safe_make_unsigned;

    //! [homogeneous]
    BOOST_HANA_CONSTEXPR_LAMBDA auto safe_divide = [](auto x, auto y) {
        return eval_if(y == 0,
            [=](auto) { return 0; },
            [=](auto id) { return id(x) / y; }
        );
    };

    BOOST_HANA_CONSTEXPR_CHECK(safe_divide(6, 3) == 2);
    BOOST_HANA_CONSTEXPR_CHECK(safe_divide(6, 0) == 0);
    //! [homogeneous]
}
