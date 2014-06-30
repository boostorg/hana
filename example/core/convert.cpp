/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/adapted/std_list.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/list.hpp>

#include <cassert>
#include <list>
#include <string>
#include <vector>
using namespace boost::hana;


struct StdVector;
struct StdList;

namespace boost { namespace hana {
    template <typename T>
    struct datatype<std::vector<T>> { using type = StdVector; };

    template <>
    struct Comparable::instance<StdVector, StdVector> {
        template <typename T>
        static auto equal_impl(std::vector<T> xs, std::vector<T> ys)
        { return xs == ys; }
    };

    template <>
    struct convert<StdVector, StdList> {
        template <typename T>
        static std::vector<T> apply(std::list<T> xs) {
            std::vector<T> v{};
            for (auto x: xs)
                v.push_back(x);
            return v;
        }
    };
}}

int main() {
    auto xs = list(
        std::list<int>{1, 2, 3, 4},
        std::list<char>{'1', '2', '3', '4'},
        std::list<std::string>{"1", "2", "3", "4"}
    );

    assert(fmap(to<StdVector>, xs) == list(
        std::vector<int>{1, 2, 3, 4},
        std::vector<char>{'1', '2', '3', '4'},
        std::vector<std::string>{"1", "2", "3", "4"}
    ));
}
