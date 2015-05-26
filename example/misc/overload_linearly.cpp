/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/tuple.hpp>

#include <string>
using namespace boost::hana;


// We have an utility in the Functional module that does pretty much the
// same, but is more compile-time efficient. It is still interesting to
// see this implemented with sequences and the SFINAE combinator.

auto overload_linearly = [](auto ...candidates) {
    return [=](auto ...args) {
        auto maybe_function = find_if(make_tuple(candidates...), [=](auto f) {
            return is_just(sfinae(f)(args...));
        });
        auto result = transform(maybe_function, [=](auto f) {
            return f(args...);
        });
        return result;
    };
};

int main() {
    auto f = ::overload_linearly(
        [](std::string s) { return s + "abcd"; },
        [](int i) { return i + 1; },
        [](double f) { return f + 2; }
    );

    BOOST_HANA_RUNTIME_CHECK(f(1) == just(1 + 1));
    BOOST_HANA_RUNTIME_CHECK(f(2.3) == just(static_cast<int>(2.3) + 1));
}
