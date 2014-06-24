/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

template <typename ...Xs>
constexpr auto make_storage(Xs ...xs) {
    auto storage = [=](auto f) { return f(xs...); };
    return storage;
}

template <typename ...Xs>
using storage_type = decltype(make_storage((*(Xs*)0)...));

template <typename ...Xs>
struct tuple {
    explicit constexpr tuple(Xs ...xs)
        : storage{make_storage(xs...)}
    { }

    storage_type<Xs...> storage;
};


int main() {
    tuple<int, char, float> t{1, '2', 3.3f};
}
