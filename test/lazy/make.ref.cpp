/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/eval.hpp>
#include <boost/hana/lazy.hpp>

#include <functional>
namespace hana = boost::hana;


// We make it non-copyable and non-movable to make sure it is taken by
// reference below.
template <typename Signature>
struct Function;

template <typename Return, typename ...Args>
struct Function<Return(Args...)> {
    std::function<Return(Args...)> f_;

    Function(std::function<Return(Args...)> f) : f_(f) { }
    Function(Function const&) = delete;
    Function(Function &&) = delete;

    template <typename ...T>
    decltype(auto) operator()(T&& ...t) const {
        return f_(static_cast<T&&>(t)...);
    }
};

int main() {
    // lazy value
    {
        int i = 3;
        auto lazy = hana::make_lazy(std::ref(i));
        int& i_ref = hana::eval(lazy);
        BOOST_HANA_RUNTIME_CHECK(&i_ref == &i);
    }

    // lazy function call
    {
        Function<void(int&, char&)> f([](int& a, char& b) -> void {
            a = 10;
            b = 'z';
        });
        int a = 3;
        char b = 'b';
        auto lazy = hana::make_lazy(std::ref(f))(std::ref(a), std::ref(b));

        BOOST_HANA_RUNTIME_CHECK(a == 3);
        BOOST_HANA_RUNTIME_CHECK(b == 'b');
        hana::eval(lazy);
        BOOST_HANA_RUNTIME_CHECK(a == 10);
        BOOST_HANA_RUNTIME_CHECK(b == 'z');
    }
}
