/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/closure.hpp>

#include <type_traits>
using namespace boost::hana;


// See this bug: https://llvm.org/bugs/show_bug.cgi?id=24173

template <typename ...Xs>
constexpr detail::closure<std::decay_t<Xs>...> f(Xs&& ...xs)
{ return detail::closure<std::decay_t<Xs>...>{static_cast<Xs&&>(xs)...}; }

int main() {
    f(f(f(f(f(f(f(f(f(f(f(f(1))))))))))));
}
