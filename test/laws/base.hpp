/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TEST_LAWS_BASE_HPP
#define BOOST_HANA_TEST_LAWS_BASE_HPP

#include <boost/hana.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/concept/integral_constant.hpp>
#include <boost/hana/concept/logical.hpp>
#include <boost/hana/concept/orderable.hpp>
#include <boost/hana/detail/wrong.hpp>
#include <boost/hana/functional/infix.hpp>
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

        template <int i>
        struct for_each_n_t {
            static_assert(i > 0, "can't use for_each_n with i < 0");

            template <typename Xs, typename F>
            constexpr auto operator()(Xs xs, F f) const {
                hana::for_each(xs,
                    hana::compose(
                        hana::partial(for_each_n_t<i - 1>{}, xs),
                        hana::partial(hana::partial, f)
                    )
                );
            }
        };

        template <>
        struct for_each_n_t<1> {
            template <typename Xs, typename F>
            constexpr auto operator()(Xs xs, F f) const {
                hana::for_each(xs, f);
            }
        };

        template <int i>
        constexpr for_each_n_t<i> for_each_n{};

        auto foreach = hana::for_each;
        constexpr auto foreach3 = for_each_n<3>;
        constexpr auto foreach2 = for_each_n<2>;

        struct implies_t {
            template <typename P, typename Q>
            constexpr decltype(auto) operator()(P&& p, Q&& q) const {
                return hana::or_(hana::not_(static_cast<P&&>(p)),
                                 static_cast<Q&&>(q));
            }
        };
        constexpr auto implies = hana::infix(implies_t{});

        struct iff_t {
            template <typename P, typename Q>
            constexpr decltype(auto) operator()(P&& p, Q&& q) const {
                return hana::and_(implies(p, q), implies(q, p));
            }
        };
        constexpr auto iff = hana::infix(iff_t{});

        template <typename Cond, typename F>
        constexpr decltype(auto) only_when_(Cond cond, F f) {
            return hana::eval_if(cond, f, [](auto){ });
        }

        // A type with a constructor that must not be instantiated.
        // This is to make sure we don't instantiate something else than
        // the copy-constructor of the elements inside a container when we
        // copy the container.
        struct trap_construct {
            trap_construct() = default;
            trap_construct(trap_construct const&) = default;
            trap_construct(trap_construct&) = default;
            trap_construct(trap_construct&&) = default;

            template <typename X>
            trap_construct(X&&) {
                static_assert(detail::wrong<X>{},
                "this constructor must not be instantiated");
            }

            // This type is stored inside a hana::set, which requires
            // compile-time Comparable keys.
            constexpr auto operator==(trap_construct const&) const
            { return false_c; }
            constexpr auto operator!=(trap_construct const&) const
            { return true_c; }
        };

        // A move-only type. Useful for testing containers.
        struct move_only {
            move_only() = default;
            move_only(move_only const&) = delete;
            move_only(move_only&&) = default;
        };

        //////////////////////////////////////////////////////////////////////
        // Tracked
        //////////////////////////////////////////////////////////////////////
        struct Tracked {
            enum class State { CONSTRUCTED, MOVED_FROM, DESTROYED };

            int value;
            State state;

            explicit Tracked(int k) : value{k}, state{State::CONSTRUCTED} {
                // std::cerr << "constructing " << *this << '\n';
            }

            Tracked(Tracked const& t) : value{t.value}, state{State::CONSTRUCTED} {
                BOOST_HANA_RUNTIME_CHECK(t.state != State::MOVED_FROM &&
                    "copying a moved-from object");

                BOOST_HANA_RUNTIME_CHECK(t.state != State::DESTROYED &&
                    "copying a destroyed object");

                // std::cerr << "copying " << *this << '\n';
            }

            Tracked(Tracked&& t) : value{t.value}, state{State::CONSTRUCTED} {
                BOOST_HANA_RUNTIME_CHECK(t.state != State::MOVED_FROM &&
                    "double moving from an object");

                BOOST_HANA_RUNTIME_CHECK(t.state != State::DESTROYED &&
                    "moving from a destroyed object");

                // std::cerr << "moving " << t << '\n';
                t.state = State::MOVED_FROM;
            }

            Tracked& operator=(Tracked const& other) {
                BOOST_HANA_RUNTIME_CHECK(this->state != State::DESTROYED &&
                    "assigning to a destroyed object");

                BOOST_HANA_RUNTIME_CHECK(other.state != State::MOVED_FROM &&
                    "assigning a moved-from object");

                BOOST_HANA_RUNTIME_CHECK(other.state != State::DESTROYED &&
                    "assigning a destroyed object");

                // std::cerr << "assigning " << other << " to " << *this << '\n';
                this->value = other.value;
                return *this;
            }

            Tracked& operator=(Tracked&& other) {
                BOOST_HANA_RUNTIME_CHECK(this->state != State::DESTROYED &&
                    "assigning to a destroyed object");

                BOOST_HANA_RUNTIME_CHECK(other.state != State::MOVED_FROM &&
                    "double-moving from an object");

                BOOST_HANA_RUNTIME_CHECK(other.state != State::DESTROYED &&
                    "assigning a destroyed object");

                // std::cerr << "assigning " << other << " to " << *this << '\n';
                this->value = other.value;
                other.state = State::MOVED_FROM;
                return *this;
            }

            ~Tracked() {
                BOOST_HANA_RUNTIME_CHECK(state != State::DESTROYED &&
                    "double-destroying an object");

                // std::cerr << "destructing " << *this << '\n';
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

        //////////////////////////////////////////////////////////////////////
        // InjectionResult
        //////////////////////////////////////////////////////////////////////
        struct InjectionResult { };

        template <int i, typename ...X>
        struct injection_result {
            using hana_tag = InjectionResult;
            static constexpr int injection_id = i;
            hana::tuple<X...> args;
            Tracked tracker;

            template <typename ...Y, typename = decltype(tuple<X...>{std::declval<Y>()...})>
            constexpr explicit injection_result(Y&& ...y)
                : args{static_cast<Y&&>(y)...}, tracker{i}
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
                    typename std::decay<X>::type...
                >{static_cast<X&&>(x)...};
            }
        };
    } // end namespace test

    using test::Tracked;

    template <>
    struct equal_impl<test::InjectionResult, test::InjectionResult> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y) {
            return hana::and_(
                hana::bool_c<X::injection_id == Y::injection_id>,
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

        template <int i, Policy policy, typename = void>
        struct integer {
            static_assert(
            !!(policy & (Policy::Constant | Policy::Constexpr | Policy::Runtime))
            , "You must choose at least one of Constant, Constexpr and Runtime.");

            static constexpr int value = i;
            constexpr operator int() const { return value; }
            using hana_tag = Integer<policy>;
            Tracked tracker{i};
        };

        template <int i, Policy policy>
        struct integer <i, policy, std::enable_if_t<!!(policy & Policy::Constexpr)>> {
            static constexpr int value = i;
            constexpr operator int() const { return value; }
            using hana_tag = Integer<policy>;
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
    // Model of Constant/IntegralConstant
    //////////////////////////////////////////////////////////////////////////
    template <test::Policy policy>
    struct IntegralConstant<test::Integer<policy>> {
        static constexpr bool value = static_cast<bool>(policy & test::Policy::Constant);
    };

    template <test::Policy policy, typename C>
    struct to_impl<test::Integer<policy>, C, when<
        (policy & test::Policy::Constant) &&
        hana::IntegralConstant<C>::value
    >>
        : embedding<is_embedded<typename C::value_type, int>{}>
    {
        template <typename N>
        static constexpr auto apply(N const&) {
            return test::integer<N::value, policy>{};
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
