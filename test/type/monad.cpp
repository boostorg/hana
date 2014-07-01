/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/type.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/functional.hpp>
using namespace boost::hana;


template <typename ...> struct F;
struct X { };

int main() {
    BOOST_HANA_STATIC_ASSERT(join(decltype_(decltype_(X{}))) == decltype_(X{}));
    BOOST_HANA_STATIC_ASSERT(join(decltype_(decltype_(decltype_(X{})))) == decltype_(decltype_(X{})));

    BOOST_HANA_STATIC_ASSERT(bind(type<X>, compose(decltype_, template_<F>)) == type<F<X>>);
}
