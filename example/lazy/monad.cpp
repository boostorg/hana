/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/lazy.hpp>

#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
using namespace boost::hana;


template <typename T>
auto read = lazy([](std::istream& stream) {
    T x;
    stream >> x;
    std::cout << "read " << x << " from the stream\n";
    return x;
});

int main() {
    //! [main]
    std::stringstream ss;
    int in = 123;

    std::cout << "creating the monadic chain...\n";
    auto out = read<int>(std::ref(ss))
        >> [](auto x) {
            std::cout << "performing x + 1...\n";
            return lazy(x + 1);
        }
        >> [](auto x) {
            std::cout << "performing x / 2...\n";
            return lazy(x / 2);
        };

    std::cout << "putting " << in << " in the stream...\n";
    ss << in; // nothing is evaluated yet

    std::cout << "evaluating the monadic chain...\n";
    auto eout = eval(out);

    std::cout << "the result of the monadic chain is " << eout << "\n";
    assert(eout == (in + 1) / 2);
    //! [main]
}
