/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    namespace sandbox {
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
        template <typename T, typename Enable = void>
        struct Logical;

        BOOST_HANA_CONSTEXPR_LAMBDA auto if_ = [](auto logical, auto then_, auto else_) {
            return Logical<datatype_t<decltype(logical)>>::if_impl(logical, then_, else_);
        };

        BOOST_HANA_CONSTEXPR_LAMBDA auto eval_if = [](auto logical, auto then_, auto else_) {
            return Logical<datatype_t<decltype(logical)>>::eval_if_impl(logical, then_, else_);
        };

        BOOST_HANA_CONSTEXPR_LAMBDA auto eval_else_if = eval_if;
        BOOST_HANA_CONSTEXPR_LAMBDA auto eval_else = id;

        BOOST_HANA_CONSTEXPR_LAMBDA auto else_if = if_;
        BOOST_HANA_CONSTEXPR_LAMBDA auto else_ = id;

        constexpr struct _eval {
            template <typename F>
            static constexpr auto call(F f, int) -> decltype(f(id))
            { return f(id); }

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
    }

    template <>
    struct defaults<sandbox::Logical> {
        template <typename T, typename Enable = void>
        struct with : defaults<> {
            template <typename Cond, typename Then, typename Else>
            static constexpr auto if_impl(Cond c, Then t, Else e)
            { return sandbox::eval_if(c, always(t), always(e)); }
        };
    };

    template <>
    struct instance<sandbox::Logical> {
        template <typename T, typename Enable = void>
        struct with : defaults<sandbox::Logical>::template with<T> {
            template <typename Cond, typename Then, typename Else>
            static constexpr auto eval_if_impl(Cond c, Then t, Else e) {
                auto would_recurse = decltype_(sandbox::eval(c)) == decltype_(c);
                static_assert(!would_recurse(),
                "Condition is probably not a logical. Trying to evaluate "
                "it and use it as the condition to if_ again would cause "
                "infinite recursion.");
                return sandbox::eval_if(sandbox::eval(c), t, e);
            }
        };
    };

    namespace sandbox {
        template <typename T, typename Enable>
        struct Logical : instance<Logical>::template with<T> { };

        template <>
        struct Logical<bool> : defaults<Logical>::with<bool> {
            template <typename Then, typename Else>
            static constexpr auto eval_if_impl(bool cond, Then t, Else e)
            { return cond ? eval(t) : eval(e); }
        };

        template <>
        struct Logical<Integral> : defaults<Logical>::with<Integral> {
            template <typename Cond, typename Then, typename Else>
            static constexpr auto eval_if_impl(Cond c, Then t, Else e)
            { return eval_if_impl(bool_<c()>, t, e); }

            template <typename Then, typename Else>
            static constexpr auto eval_if_impl(decltype(true_), Then t, Else)
            { return eval(t); }

            template <typename Then, typename Else>
            static constexpr auto eval_if_impl(decltype(false_), Then, Else e)
            { return eval(e); }
        };
    }
}}

//////////////////////////////////////////////////////////////////////////////
#include <boost/hana/detail/static_assert.hpp>
#include <cassert>
using namespace boost::hana;


struct T; struct E;

void test_eval() {
    BOOST_HANA_STATIC_ASSERT(sandbox::eval([] { return 1; }) == 1);
    BOOST_HANA_STATIC_ASSERT(sandbox::eval([](auto id) { return id(1); }) == 1);
    BOOST_HANA_STATIC_ASSERT(sandbox::eval(1) == 1);
}

void test_bool_logical() {
    BOOST_HANA_STATIC_ASSERT(sandbox::if_(true, true, false) == true);
    BOOST_HANA_STATIC_ASSERT(sandbox::if_(false, true, false) == false);
}

void test_integral_logical() {
    // can also support heterogeneous branches
    BOOST_HANA_STATIC_ASSERT(sandbox::if_(true_, type<T>, type<E>) == type<T>);
    BOOST_HANA_STATIC_ASSERT(sandbox::if_(false_, type<T>, type<E>) == type<E>);
}

void test_eval_condition() {
    BOOST_HANA_STATIC_ASSERT(sandbox::if_([]{ return true; }, true, false) == true);
    BOOST_HANA_STATIC_ASSERT(sandbox::if_([]{ return false; }, true, false) == false);
    BOOST_HANA_STATIC_ASSERT(sandbox::if_([]{ return true_; }, type<T>, type<E>) == type<T>);
    BOOST_HANA_STATIC_ASSERT(sandbox::if_([]{ return false_; }, type<T>, type<E>) == type<E>);

    BOOST_HANA_STATIC_ASSERT(sandbox::if_([](auto) { return true; }, true, false) == true);
    BOOST_HANA_STATIC_ASSERT(sandbox::if_([](auto) { return false; }, true, false) == false);
    BOOST_HANA_STATIC_ASSERT(sandbox::if_([](auto) { return true_; }, type<T>, type<E>) == type<T>);
    BOOST_HANA_STATIC_ASSERT(sandbox::if_([](auto) { return false_; }, type<T>, type<E>) == type<E>);
}

void test_eval_branches() {
    BOOST_HANA_STATIC_ASSERT(sandbox::eval_if(true,   []{ return true; },    []{ return false; }) == true);
    BOOST_HANA_STATIC_ASSERT(sandbox::eval_if(false,  []{ return true; },    []{ return false; }) == false);
    BOOST_HANA_STATIC_ASSERT(sandbox::eval_if(true_,  []{ return type<T>; }, []{ return type<E>; }) == type<T>);
    BOOST_HANA_STATIC_ASSERT(sandbox::eval_if(false_, []{ return type<T>; }, []{ return type<E>; }) == type<E>);

    BOOST_HANA_STATIC_ASSERT(sandbox::eval_if(true,   [](auto) { return true; },    [](auto) { return false; })   == true);
    BOOST_HANA_STATIC_ASSERT(sandbox::eval_if(false,  [](auto) { return true; },    [](auto) { return false; })   == false);
    BOOST_HANA_STATIC_ASSERT(sandbox::eval_if(true_,  [](auto) { return type<T>; }, [](auto) { return type<E>; }) == type<T>);
    BOOST_HANA_STATIC_ASSERT(sandbox::eval_if(false_, [](auto) { return type<T>; }, [](auto) { return type<E>; }) == type<E>);
}

template <int i>
BOOST_HANA_CONSTEXPR_LAMBDA auto branch = decltype_(int_<i>);

void test_multi_branch() {
    BOOST_HANA_STATIC_ASSERT(
        sandbox::eval_if(false_,
            []{ return branch<1>; },

            // How to remove the need for a lambda?
            [] { return sandbox::eval_if([] { return true_; },
                    [](auto) { return branch<2>; },

                    sandbox::else_(
                        [](auto) { return branch<3>; }
                    ));
            }
        ) == branch<2>
    );

    BOOST_HANA_STATIC_ASSERT(
        sandbox::eval_if([](auto) { return false_; },
            [](auto) { return branch<1>; },

        sandbox::eval_if(true,
            2,

        sandbox::else_(
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
