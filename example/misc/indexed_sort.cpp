/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>
using namespace boost::hana;
using namespace boost::hana::literals;


auto indexed_sort = [](auto list, auto predicate) {
    auto indexed_list = zip(list, to<Tuple>(range(0_c, size(list))));
    auto sorted = sort.by(predicate ^on^ front, indexed_list);
    return make_pair(transform(sorted, front), transform(sorted, back));
};

int main() {
    auto types = tuple_t<char[4], char[2], char[1], char[5], char[3]>;
    auto sorted = indexed_sort(types, [](auto t, auto u) {
        return sizeof_(t) < sizeof_(u);
    });
    using Tup = decltype(unpack(first(sorted), template_<tuple>))::type;
    auto indices = second(indexed_sort(second(sorted), less));

    // When accessed through the indices sequence, the tuple appears to be
    // ordered as the `types` above. However, as can be seen in the
    // static_assert below, the tuple is actually ordered differently.
    Tup tup;
    char const(&a)[4] = tup[indices[0_c]];
    char const(&b)[2] = tup[indices[1_c]];
    char const(&c)[1] = tup[indices[2_c]];
    char const(&d)[5] = tup[indices[3_c]];
    char const(&e)[3] = tup[indices[4_c]];

    static_assert(std::is_same<
        Tup,
        tuple<char[1], char[2], char[3], char[4], char[5]>
    >{}, "");

    (void)a; (void)b; (void)c; (void)d; (void)e;
}
