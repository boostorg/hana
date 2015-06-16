/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/closure.hpp>

#include <memory>
using namespace boost::hana;


// Tuples of smart pointers; based on LLVM bug #18350
// auto_ptr doesn't have a copy constructor that takes a const &, but tuple does.
int main() {
    {
        detail::closure<std::unique_ptr<char>> up;
        detail::closure<std::shared_ptr<char>> sp;
        detail::closure<std::weak_ptr  <char>> wp;
    }
    {
        detail::closure<std::unique_ptr<char[]>> up;
        detail::closure<std::shared_ptr<char[]>> sp;
        detail::closure<std::weak_ptr  <char[]>> wp;
    }
    {
        detail::closure<std::unique_ptr<char[5]>> up;
        detail::closure<std::shared_ptr<char[5]>> sp;
        detail::closure<std::weak_ptr  <char[5]>> wp;
    }
}
