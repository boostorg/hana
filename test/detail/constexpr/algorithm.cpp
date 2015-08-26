/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/orderable.hpp>
#include <boost/hana/functional/placeholder.hpp>

#include <boost/hana/detail/constexpr/algorithm.hpp>
using namespace boost::hana;
namespace cx = boost::hana::detail::constexpr_;


// The algorithms are taken from the suggested implementations on cppreference.
// Hence, we assume them to be correct and we only make sure they compile, to
// avoid stupid mistakes I could have made when copy/pasting and editing.
//
// Oh, and we also make sure they can be used in a constexpr context.
constexpr int constexpr_context() {
    int x = 0, y = 1;
    cx::swap(x, y);

    int array[6] = {1, 2, 3, 4, 5, 6};
    int* first = array;
    int* last = array + 6;

    cx::reverse(first, last);

    cx::next_permutation(first, last, less);
    cx::next_permutation(first, last);

    cx::lexicographical_compare(first, last, first, last, less);
    cx::lexicographical_compare(first, last, first, last);

    cx::equal(first, last, first, last, equal);
    cx::equal(first, last, first, last);

    cx::sort(first, last, equal);
    cx::sort(first, last);

    cx::find(first, last, 3);
    cx::find_if(first, last, equal.to(3));

    cx::iota(first, last, 0);

    cx::count(first, last, 2);

    cx::accumulate(first, last, 0);
    cx::accumulate(first, last, 1, _ * _);

    cx::min_element(first, last);

    return 0;
}

constexpr int must_be_constexpr = constexpr_context();

int main() { }
