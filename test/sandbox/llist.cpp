/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <cstddef>


template <typename T>
struct llist {
    union Storage {
        struct {
            T head_;
            llist const* next_;
        } it_;
        std::nullptr_t nil_;

        constexpr Storage() : nil_{} { }
        constexpr Storage(T t, llist const* n) : it_{t, n} { }
    } storage_;

    enum class Which { NIL, NEXT } which_;

    constexpr llist()
        : storage_{}, which_{Which::NIL}
    { }

    constexpr llist(T x, llist const* nxt)
        : storage_{x, nxt}, which_{Which::NEXT}
    { }

    friend constexpr bool is_empty(llist self)
    { return self.which_ == Which::NIL; }

    friend constexpr T head(llist self) {
        if (is_empty(self))
            throw "head on empty list";
        return self.storage_.it_.head_;
    }

    friend constexpr llist tail(llist self) {
        if (is_empty(self))
            throw "tail on empty list";
        return *self.storage_.it_.next_;
    }

    friend constexpr llist cons(T x, llist const& xs) {
        return llist{x, &xs};
    }

    template <typename State, typename F>
    friend constexpr auto foldr(llist xs, State s, F f) {
        if (is_empty(xs))
            return s;
        else
            return f(head(xs), foldr(tail(xs), s, f));
    }
};



//////////////////////////////////////////////////////////////////////////////
#include <functional>


constexpr llist<int> xs{};
static_assert(is_empty(xs), "");
static_assert(!is_empty(cons(1, xs)), "");
static_assert(head(cons(1, xs)) == 1, "");
static_assert(head(cons(0, cons(1, xs))) == 0, "");
static_assert(head(tail(cons(0, cons(1, xs)))) == 1, "");

static_assert(foldr(cons(1, cons(2, cons(3, llist<int>{}))), 0, std::plus<>{}) == 1 + 2 + 3 + 0, "");


int main() {

}
