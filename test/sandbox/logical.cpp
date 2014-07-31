/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/bool.hpp>
#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/type.hpp>
namespace hana = boost::hana;


// Difficulties:
// 1. eval_if(false_, ...) will use Logical<bool> because it will try
//    to eval(false_), which is false_(), which is false.
//
//    possible workaround 1: do not try false_() in eval(false_), and
//    whenever you want to have a lambda branch you _must_ write
//    [](auto) { ... } instead of []{ ... }
//
//    possible workaround 2: first try to dispatch to the condition
//    and then try to dispatch to eval(cond) in Logical. This is the
//    current solution.
//
// 2. could we explicitly say "i want this branch evaluated"?
//    something like if_(eval(cond), eval(branch1), eval(branch2)).
//
//
// Warning: make sure the whole approach with `eval` is not
// fundamentally broken. There seems to be some loss of information
// in eval(), which I suspect breaks the whole thing in subtle cases.
struct Logical {
    BOOST_HANA_TYPECLASS(Logical);
    struct mcd;
    template <typename T>
    struct default_instance;
};

BOOST_HANA_CONSTEXPR_LAMBDA auto if_ = [](auto logical, auto then_, auto else_) {
    return Logical::instance<hana::datatype_t<decltype(logical)>>::if_impl(logical, then_, else_);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto eval_if = [](auto logical, auto then_, auto else_) {
    return Logical::instance<hana::datatype_t<decltype(logical)>>::eval_if_impl(logical, then_, else_);
};

BOOST_HANA_CONSTEXPR_LAMBDA auto eval_else_if = eval_if;
BOOST_HANA_CONSTEXPR_LAMBDA auto eval_else = hana::id;

BOOST_HANA_CONSTEXPR_LAMBDA auto else_if = if_;
BOOST_HANA_CONSTEXPR_LAMBDA auto else_ = hana::id;

constexpr struct _eval {
    template <typename F>
    static constexpr auto call(F f, int) -> decltype(f(hana::id))
    { return f(hana::id); }

    template <typename F>
    static constexpr auto call(F f, long) -> decltype(f())
    { return f(); }

    template <typename F>
    static constexpr auto call(F f, ...)
    { return f; }

    template <typename F>
    constexpr auto operator()(F f) const
    { return call(f, int{0}); }

    template <typename F>
    constexpr auto operator[](F f) const
    { return [=](auto ...args) { return (*this)(f(args...)); }; }
} eval{};

struct Logical::mcd {
    template <typename Cond, typename Then, typename Else>
    static constexpr auto if_impl(Cond c, Then t, Else e)
    { return eval_if(c, hana::always(t), hana::always(e)); }
};

template <typename T>
struct Logical::default_instance : Logical::mcd {
    template <typename Cond, typename Then, typename Else>
    static constexpr auto eval_if_impl(Cond c, Then t, Else e) {
        auto would_recurse = hana::decltype_(eval(c)) == hana::decltype_(c);
        static_assert(!would_recurse(),
        "Condition is probably not a logical. Trying to evaluate "
        "it and use it as the condition to if_ again would cause "
        "infinite recursion.");
        return eval_if(eval(c), t, e);
    }
};

template <>
struct Logical::instance<bool> : Logical::mcd {
    template <typename Then, typename Else>
    static constexpr auto eval_if_impl(bool cond, Then t, Else e)
    { return cond ? eval(t) : eval(e); }
};

template <>
struct Logical::instance<hana::Integral> : Logical::mcd {
    template <typename Cond, typename Then, typename Else>
    static constexpr auto eval_if_impl(Cond c, Then t, Else e)
    { return eval_if_impl(hana::bool_<c()>, t, e); }

    template <typename Then, typename Else>
    static constexpr auto eval_if_impl(decltype(hana::true_), Then t, Else)
    { return eval(t); }

    template <typename Then, typename Else>
    static constexpr auto eval_if_impl(decltype(hana::false_), Then, Else e)
    { return eval(e); }
};

//////////////////////////////////////////////////////////////////////////////
#include <boost/hana/detail/static_assert.hpp>
#include <cassert>
using hana::true_;
using hana::false_;
using hana::type;


struct T; struct E;

void test_eval() {
    BOOST_HANA_STATIC_ASSERT(eval([] { return 1; }) == 1);
    BOOST_HANA_STATIC_ASSERT(eval([](auto id) { return id(1); }) == 1);
    BOOST_HANA_STATIC_ASSERT(eval(1) == 1);
}

void test_bool_logical() {
    BOOST_HANA_STATIC_ASSERT(if_(true, true, false) == true);
    BOOST_HANA_STATIC_ASSERT(if_(false, true, false) == false);
}

void test_integral_logical() {
    // can also support heterogeneous branches
    BOOST_HANA_STATIC_ASSERT(if_(true_, type<T>, type<E>) == type<T>);
    BOOST_HANA_STATIC_ASSERT(if_(false_, type<T>, type<E>) == type<E>);
}

void test_eval_condition() {
    BOOST_HANA_STATIC_ASSERT(if_([]{ return true; }, true, false) == true);
    BOOST_HANA_STATIC_ASSERT(if_([]{ return false; }, true, false) == false);
    BOOST_HANA_STATIC_ASSERT(if_([]{ return true_; }, type<T>, type<E>) == type<T>);
    BOOST_HANA_STATIC_ASSERT(if_([]{ return false_; }, type<T>, type<E>) == type<E>);

    BOOST_HANA_STATIC_ASSERT(if_([](auto) { return true; }, true, false) == true);
    BOOST_HANA_STATIC_ASSERT(if_([](auto) { return false; }, true, false) == false);
    BOOST_HANA_STATIC_ASSERT(if_([](auto) { return true_; }, type<T>, type<E>) == type<T>);
    BOOST_HANA_STATIC_ASSERT(if_([](auto) { return false_; }, type<T>, type<E>) == type<E>);
}

void test_eval_branches() {
    BOOST_HANA_STATIC_ASSERT(eval_if(true,   []{ return true; },    []{ return false; }) == true);
    BOOST_HANA_STATIC_ASSERT(eval_if(false,  []{ return true; },    []{ return false; }) == false);
    BOOST_HANA_STATIC_ASSERT(eval_if(true_,  []{ return type<T>; }, []{ return type<E>; }) == type<T>);
    BOOST_HANA_STATIC_ASSERT(eval_if(false_, []{ return type<T>; }, []{ return type<E>; }) == type<E>);

    BOOST_HANA_STATIC_ASSERT(eval_if(true,   [](auto) { return true; },    [](auto) { return false; })   == true);
    BOOST_HANA_STATIC_ASSERT(eval_if(false,  [](auto) { return true; },    [](auto) { return false; })   == false);
    BOOST_HANA_STATIC_ASSERT(eval_if(true_,  [](auto) { return type<T>; }, [](auto) { return type<E>; }) == type<T>);
    BOOST_HANA_STATIC_ASSERT(eval_if(false_, [](auto) { return type<T>; }, [](auto) { return type<E>; }) == type<E>);
}

template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto branch = hana::decltype_(hana::int_<i>);

void test_multi_branch() {
    BOOST_HANA_STATIC_ASSERT(
        eval_if(false_,
            []{ return branch<1>; },

            // How to remove the need for a lambda?
            [] { return eval_if([] { return true_; },
                    [](auto) { return branch<2>; },

                    else_(
                        [](auto) { return branch<3>; }
                    ));
            }
        ) == branch<2>
    );

    BOOST_HANA_STATIC_ASSERT(
        eval_if([](auto) { return false_; },
            [](auto) { return branch<1>; },

        eval_if(true,
            2,

        else_(
            3
        ))) == 2
    );
}

void test_syntactic_sugar() {
#if 0
    if_(cond)([]{

    })
    +else_if(cond)([]{

    })
    +else_(if_(cond)(

    ))
    +else_(

    )


    auto else_if = curry<3>([](auto c, auto t, auto e) {
        return else_(if_(c, t, e));
    });
#endif
}


int main() {
    test_eval();
    test_bool_logical();
    test_integral_logical();
    test_eval_condition();
    test_eval_branches();
    test_multi_branch();
    test_syntactic_sugar();
}
