/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_BASE_HPP
#define BOOST_HANA_TEST_LAWS_BASE_HPP

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/tuple.hpp>

#include <iostream>
#include <type_traits>
#include <utility>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Misc
    //////////////////////////////////////////////////////////////////////////
    namespace test {
        struct laws;

        auto foreach = hana::for_each;
        auto foreach3 = [](auto xs, auto pred) {
            hana::for_each(xs, [=](auto a) {
            hana::for_each(xs, [=](auto b) {
            hana::for_each(xs, [=](auto c) {
                pred(a, b, c);
            });});});
        };

        auto foreach2 = [](auto xs, auto pred) {
            hana::for_each(xs, [=](auto a) {
            hana::for_each(xs, [=](auto b) {
                pred(a, b);
            });});
        };

        struct _implies {
            template <typename P, typename Q>
            constexpr decltype(auto) operator()(P&& p, Q&& q) const {
                return hana::or_(hana::not_(detail::std::forward<P>(p)),
                                 detail::std::forward<Q>(q));
            }
        };
        constexpr auto implies = hana::infix(_implies{});

        struct _iff {
            template <typename P, typename Q>
            constexpr decltype(auto) operator()(P&& p, Q&& q) const {
                return hana::and_(implies(p, q), implies(q, p));
            }
        };
        constexpr auto iff = hana::infix(_iff{});

        template <typename Cond, typename F>
        constexpr decltype(auto) only_when_(Cond cond, F f) {
            return hana::eval_if(cond, f, [](auto){ });
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Tracked
    //////////////////////////////////////////////////////////////////////////
    struct Tracked {
        enum class State { CONSTRUCTED, MOVED_FROM, DESTROYED };

        int value;
        State state;

        explicit Tracked(int k) : value{k}, state{State::CONSTRUCTED} {
            std::cerr << "constructing " << *this << '\n';
        }

        Tracked(Tracked const& t) : value{t.value}, state{State::CONSTRUCTED} {
            BOOST_HANA_RUNTIME_CHECK(t.state != State::MOVED_FROM &&
                "copying a moved-from object");

            BOOST_HANA_RUNTIME_CHECK(t.state != State::DESTROYED &&
                "copying a destroyed object");

            std::cerr << "copying " << *this << '\n';
        }

        Tracked(Tracked&& t) : value{t.value}, state{State::CONSTRUCTED} {
            BOOST_HANA_RUNTIME_CHECK(t.state != State::MOVED_FROM &&
                "double moving from an object");

            BOOST_HANA_RUNTIME_CHECK(t.state != State::DESTROYED &&
                "moving from a destroyed object");

            std::cerr << "moving " << t << '\n';
            t.state = State::MOVED_FROM;
        }

        ~Tracked() {
            BOOST_HANA_RUNTIME_CHECK(state != State::DESTROYED &&
                "double-destroying an object");

            std::cerr << "destructing " << *this << '\n';
            state = State::DESTROYED;
        }

        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits>&
        operator<<(std::basic_ostream<CharT, Traits>& os, Tracked const& t) {
            os << "Tracked{" << t.value << "}";
            switch (t.state) {
            case State::CONSTRUCTED:
                os << "[ok]"; break;
            case State::MOVED_FROM:
                os << "[moved from]"; break;
            case State::DESTROYED:
                os << "[destroyed]"; break;
            default:
                BOOST_HANA_RUNTIME_CHECK(false && "never reached");
            }
            return os;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // InjectionResult
    //////////////////////////////////////////////////////////////////////////
    namespace test {
        struct InjectionResult { };

        template <int i, typename ...X>
        struct injection_result {
            struct hana { using datatype = InjectionResult; };
            static constexpr int injection_id = i;
            boost::hana::_tuple<X...> args;
            Tracked tracker;

            template <typename ...Y, typename = decltype(_tuple<X...>{std::declval<Y>()...})>
            constexpr explicit injection_result(Y&& ...y)
                : args{boost::hana::detail::std::forward<Y>(y)...}, tracker{i}
            { }
        };

        //! A monotonic injective function.
        //!
        //! This is used in the unit tests, where we often just need a function
        //! which preserves equality and order, but which also satisfies the
        //! following law for all `Injection`s `f` and `g`:
        //! @code
        //!     f(x) == g(x) if and only if f === g
        //! @endcode
        //! where `===` means _was created by the same call to `injection`_.
        //! This allows creating several such functions in the unit tests while
        //! conserving precious information such as the fact that
        //! `f(g(x)) != g(f(x))`.
        template <int i>
        struct _injection {
            template <typename ...X>
            constexpr auto operator()(X&& ...x) const {
                return injection_result<i,
                    typename hana::detail::std::decay<X>::type...
                >{hana::detail::std::forward<X>(x)...};
            }
        };
    }

    template <>
    struct equal_impl<test::InjectionResult, test::InjectionResult> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y) {
            return hana::and_(
                hana::bool_<X::injection_id == Y::injection_id>,
                hana::equal(x.args, y.args)
            );
        }
    };

    template <>
    struct less_impl<test::InjectionResult, test::InjectionResult> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y) {
            static_assert(X::injection_id == Y::injection_id,
            "can't order the result of two different injections");
            return hana::less(x.args, y.args);
        }
    };


    //////////////////////////////////////////////////////////////////////////
    // Integer
    //////////////////////////////////////////////////////////////////////////
    namespace test {
        enum class Policy : int {
            // One of those is mandatory
              Constant   = 1 << 0
            , Constexpr  = 1 << 1
            , Runtime    = 1 << 2

            // Those are optional
            , Tracked    = 1 << 3
            , Comparable = 1 << 4
            , Orderable  = 1 << 5
        };

        constexpr bool operator&&(Policy a, Policy b) {
            return static_cast<int>(a) && static_cast<int>(b);
        }

        constexpr bool operator&&(Policy a, bool b) {
            return static_cast<int>(a) && b;
        }

        constexpr bool operator&&(bool a, Policy b) {
            return a && static_cast<int>(b);
        }

        constexpr bool operator||(Policy a, Policy b) {
            return static_cast<int>(a) || static_cast<int>(b);
        }

        constexpr bool operator||(Policy a, bool b) {
            return static_cast<int>(a) || b;
        }

        constexpr bool operator||(bool a, Policy b) {
            return a || static_cast<int>(b);
        }

        constexpr bool operator!(Policy a) {
            return !static_cast<int>(a);
        }

        constexpr Policy operator|(Policy a, Policy b) {
            return static_cast<Policy>(static_cast<int>(a) | static_cast<int>(b));
        }

        constexpr Policy operator&(Policy a, Policy b) {
            return static_cast<Policy>(static_cast<int>(a) & static_cast<int>(b));
        }

        template <Policy policy, typename = void>
        struct Integer { };

        template <Policy policy>
        struct Integer<policy, std::enable_if_t<!!(policy & Policy::Constant)>> {
            using value_type = int;
        };

        template <int i, Policy policy>
        struct integer {
            static_assert(
            !!(policy & (Policy::Constant | Policy::Constexpr | Policy::Runtime))
            , "You must choose at least one of Constant, Constexpr and Runtime.");

            static constexpr int value = i;
            constexpr operator int() const { return value; }
            struct hana { using datatype = Integer<policy>; };

            struct notrack { constexpr notrack(int) { } };
            //! @todo Fix broken usages of `value` and then enable this.
            std::conditional_t<!!(policy & Policy::Tracked), Tracked, notrack> tracker{i};
        };

        template <int i>
        using eq = integer<i, Policy::Comparable | Policy::Runtime>;

        template <int i>
        using ct_eq = integer<i, Policy::Comparable | Policy::Constant>;

        template <int i>
        using cx_eq = integer<i, Policy::Comparable | Policy::Constexpr>;

        template <int i>
        using ord = integer<i, Policy::Orderable | Policy::Runtime>;

        template <int i>
        using ct_ord = integer<i, Policy::Orderable | Policy::Constant>;

        template <int i>
        using cx_ord = integer<i, Policy::Orderable | Policy::Constexpr>;

        template <int i>
        using _constant = integer<i,
              Policy::Constant
            | Policy::Comparable
            | Policy::Orderable
        >;
    }

    //////////////////////////////////////////////////////////////////////////
    // Model of Constant
    //////////////////////////////////////////////////////////////////////////
    template <test::Policy policy>
    struct value_impl<test::Integer<policy>, when<
        !!(policy & test::Policy::Constant)
    >> {
        template <typename C>
        static constexpr auto apply()
        { return C::value; }
    };

    template <test::Policy policy, typename C>
    struct to_impl<test::Integer<policy>, C, when<
        (policy & test::Policy::Constant) &&
        models<Constant(C)>{} &&
        detail::std::is_integral<typename C::value_type>{}
    >>
        : embedding<is_embedded<typename C::value_type, int>{}>
    {
        template <typename X>
        static constexpr auto apply(X const&) {
            constexpr int v = hana::value<X>();
            return test::integer<v, policy>{};
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model of Comparable
    //////////////////////////////////////////////////////////////////////////
    template <test::Policy p1, test::Policy p2>
    struct equal_impl<test::Integer<p1>, test::Integer<p2>, when<
        // both Comparable or Orderable
        (p1 & (test::Policy::Comparable | test::Policy::Orderable)) &&
        (p2 & (test::Policy::Comparable | test::Policy::Orderable)) &&

        // one Constexpr and the other Constant, or both Constexpr
        (((p1 & test::Policy::Constant) && (p2 & test::Policy::Constexpr)) ||
        ((p1 & test::Policy::Constexpr) && (p2 & test::Policy::Constant))  ||
        ((p1 & test::Policy::Constexpr) && (p2 & test::Policy::Constexpr)))
    >> {
        template <typename X, typename Y>
        static constexpr bool apply(X const&, Y const&)
        { return X::value == Y::value; }
    };

    template <test::Policy p1, test::Policy p2>
    struct equal_impl<test::Integer<p1>, test::Integer<p2>, when<
        // both Comparable or Orderable
        (p1 & (test::Policy::Comparable | test::Policy::Orderable)) &&
        (p2 & (test::Policy::Comparable | test::Policy::Orderable)) &&

        // either one is Runtime
        ((p1 & test::Policy::Runtime) || (p2 & test::Policy::Runtime))
    >> {
        template <typename X, typename Y>
        static bool apply(X const&, Y const&)
        { return X::value == Y::value; }
    };


    //////////////////////////////////////////////////////////////////////////
    // Model of Orderable
    //////////////////////////////////////////////////////////////////////////
    template <test::Policy p1, test::Policy p2>
    struct less_impl<test::Integer<p1>, test::Integer<p2>, when<
        // both Orderable
        (p1 & test::Policy::Orderable) && (p2 & test::Policy::Orderable) &&

        // one Constexpr and the other Constant, or both Constexpr
        (((p1 & test::Policy::Constant) && (p2 & test::Policy::Constexpr)) ||
        ((p1 & test::Policy::Constexpr) && (p2 & test::Policy::Constant))  ||
        ((p1 & test::Policy::Constexpr) && (p2 & test::Policy::Constexpr)))
    >> {
        template <typename X, typename Y>
        static constexpr bool apply(X const&, Y const&)
        { return X::value < Y::value; }
    };

    template <test::Policy p1, test::Policy p2>
    struct less_impl<test::Integer<p1>, test::Integer<p2>, when<
        // both Orderable
        (p1 & test::Policy::Orderable) && (p2 & test::Policy::Orderable) &&

        // either one is Runtime
        ((p1 & test::Policy::Runtime) || (p2 & test::Policy::Runtime))
    >> {
        template <typename X, typename Y>
        static bool apply(X const&, Y const&)
        { return X::value < Y::value; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TEST_LAWS_BASE_HPP
