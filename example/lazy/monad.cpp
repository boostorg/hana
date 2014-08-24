/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/lazy.hpp>

#include <functional>
#include <iostream>
#include <sstream>
using namespace boost::hana;


//! [main]
template <typename T>
auto read = [](std::istream& stream) {
    T x;
    stream >> x;
    std::cout << "read " << x << " from the stream\n";
    return x;
};

int main() {
    std::stringstream ss;
    int in = 123;

    std::cout << "creating the monadic chain...\n";
    auto out = lazy(read<int>)(std::ref(ss))
        | [](auto x) {
            std::cout << "performing x + 1...\n";
            return lazy(x + 1);
        }
        | [](auto x) {
            std::cout << "performing x / 2...\n";
            return lazy(x / 2);
        };

    std::cout << "putting " << in << " in the stream...\n";
    ss << in; // nothing is evaluated yet

    std::cout << "evaluating the monadic chain...\n";
    auto eout = eval(out);

    std::cout << "the result of the monadic chain is " << eout << "\n";
    BOOST_HANA_RUNTIME_ASSERT(eout == (in + 1) / 2);
}
//! [main]
