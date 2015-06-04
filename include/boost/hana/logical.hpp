/*!
@file
Defines `boost::hana::Logical`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_LOGICAL_HPP
#define BOOST_HANA_LOGICAL_HPP

#include <boost/hana/fwd/logical.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/canonical_constant.hpp>
#include <boost/hana/detail/dispatch_if.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/detail/std/is_arithmetic.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/lazy.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            _has_operator<datatype_t<X>, decltype(and_)>{}() ||
            _has_operator<datatype_t<Y>, decltype(and_)>{}()
        >>
        constexpr decltype(auto) operator&&(X&& x, Y&& y)
        { return hana::and_(static_cast<X&&>(x), static_cast<Y&&>(y)); }

        template <typename X, typename Y, typename = detail::std::enable_if_t<
            _has_operator<datatype_t<X>, decltype(or_)>{}() ||
            _has_operator<datatype_t<Y>, decltype(or_)>{}()
        >>
        constexpr decltype(auto) operator||(X&& x, Y&& y)
        { return hana::or_(static_cast<X&&>(x), static_cast<Y&&>(y)); }

        template <typename X, typename = detail::std::enable_if_t<
            _has_operator<datatype_t<X>, decltype(not_)>{}()
        >>
        constexpr decltype(auto) operator!(X&& x)
        { return hana::not_(static_cast<X&&>(x)); }
    }

    //////////////////////////////////////////////////////////////////////////
    // if_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct if_impl : if_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct if_impl<L, when<condition>> : default_ {
        //! @todo By using `always` here, we create a copy of both `t` and `e`,
        //! which is not very smart.
        template <typename C, typename T, typename E>
        static constexpr decltype(auto) apply(C&& c, T&& t, E&& e) {
            return hana::eval_if(static_cast<C&&>(c),
                hana::always(static_cast<T&&>(t)),
                hana::always(static_cast<E&&>(e))
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // eval_if
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct eval_if_impl : eval_if_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct eval_if_impl<L, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // while_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct while_impl : while_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct while_impl<L, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // until
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct until_impl : until_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct until_impl<L, when<condition>> : default_ {
        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto) apply(Pred&& pred, State&& state, F&& f) {
            return hana::while_(
                    hana::compose(not_, static_cast<Pred&&>(pred)),
                    static_cast<State&&>(state),
                    static_cast<F&&>(f));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // not_
    //////////////////////////////////////////////////////////////////////////
    template <typename L, typename>
    struct not_impl : not_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct not_impl<L, when<condition>> : default_ {
        static void apply(...) { }
    };

    //////////////////////////////////////////////////////////////////////////
    // and_
    //////////////////////////////////////////////////////////////////////////
    // Remove warnings generated by poor confused Doxygen
    //! @cond
    template <typename X, typename Y>
    constexpr decltype(auto) _and::operator()(X&& x, Y&& y) const {
        using Bool = typename datatype<X>::type;
        using And = BOOST_HANA_DISPATCH_IF(
            and_impl<Bool>,
            _models<Logical, Bool>{}()
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Logical, Bool>{},
        "hana::and_(x, y) requires x to be a Logical");
    #endif

        return And::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
    }

    template <typename X, typename ...Y>
    constexpr decltype(auto) _and::operator()(X&& x, Y&& ...y) const {
        return detail::variadic::foldl1(
            *this,
            static_cast<X&&>(x),
            static_cast<Y&&>(y)...
        );
    }
    //! @endcond

    template <typename L, typename>
    struct and_impl : and_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct and_impl<L, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::if_(x, static_cast<Y&&>(y), x);
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // or_
    //////////////////////////////////////////////////////////////////////////
    // Remove warnings generated by poor confused Doxygen
    //! @cond
    template <typename X, typename Y>
    constexpr decltype(auto) _or::operator()(X&& x, Y&& y) const {
        using Bool = typename datatype<X>::type;
        using Or = BOOST_HANA_DISPATCH_IF(
            or_impl<Bool>,
            _models<Logical, Bool>{}()
        );

    #ifndef BOOST_HANA_CONFIG_DISABLE_CONCEPT_CHECKS
        static_assert(_models<Logical, Bool>{},
        "hana::or_(x, y) requires x to be a Logical");
    #endif

        return Or::apply(static_cast<X&&>(x), static_cast<Y&&>(y));
    }

    template <typename X, typename ...Y>
    constexpr decltype(auto) _or::operator()(X&& x, Y&& ...y) const {
        return detail::variadic::foldl1(
            *this,
            static_cast<X&&>(x),
            static_cast<Y&&>(y)...
        );
    }
    //! @endcond

    template <typename L, typename>
    struct or_impl : or_impl<L, when<true>> { };

    template <typename L, bool condition>
    struct or_impl<L, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            //! @todo How to forward `x` here? Since the arguments to `if_`
            //! can be evaluated in any order, we have to be careful not to
            //! use `x` in a moved-from state.
            return hana::if_(x, x, static_cast<Y&&>(y));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename L>
    struct models_impl<Logical, L>
        : _integral_constant<bool,
            !is_default<eval_if_impl<L>>{}() &&
            !is_default<not_impl<L>>{}() &&
            !is_default<while_impl<L>>{}()
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for arithmetic data types
    //////////////////////////////////////////////////////////////////////////
    template <typename L>
    struct eval_if_impl<L, when<detail::std::is_arithmetic<L>{}()>> {
        template <typename Cond, typename T, typename E>
        static constexpr auto apply(Cond const& cond, T&& t, E&& e) {
            return cond ? hana::eval(static_cast<T&&>(t))
                        : hana::eval(static_cast<E&&>(e));
        }
    };

    template <typename L>
    struct not_impl<L, when<detail::std::is_arithmetic<L>{}()>> {
        template <typename Cond>
        static constexpr Cond apply(Cond const& cond)
        { return static_cast<Cond>(cond ? false : true); }
    };

    template <typename L>
    struct while_impl<L, when<detail::std::is_arithmetic<L>{}()>> {
        template <typename Pred, typename State, typename F>
        static auto apply(Pred&& pred, State&& state, F&& f)
            -> decltype(
                true ? f(static_cast<State&&>(state))
                     : static_cast<State&&>(state)
            )
        {
            if (pred(state)) {
                decltype(auto) r = f(static_cast<State&&>(state));
                return hana::while_(static_cast<Pred&&>(pred),
                                    static_cast<decltype(r)&&>(r),
                                    static_cast<F&&>(f));
            }
            else {
                return static_cast<State&&>(state);
            }
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants over a Logical
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct eval_if_impl<C, when<
        _models<Constant, C>{}() &&
        _models<Logical, typename C::value_type>{}()
    >> {
        template <typename Then, typename Else>
        static constexpr decltype(auto)
        eval_if_helper(decltype(true_), Then&& t, Else&&)
        { return hana::eval(static_cast<Then&&>(t)); }

        template <typename Then, typename Else>
        static constexpr decltype(auto)
        eval_if_helper(decltype(false_), Then&&, Else&& e)
        { return hana::eval(static_cast<Else&&>(e)); }

        template <typename Cond, typename Then, typename Else>
        static constexpr decltype(auto) apply(Cond const&, Then&& t, Else&& e) {
            constexpr auto cond = hana::value<Cond>();
            constexpr bool truth_value = hana::if_(cond, true, false);
            return eval_if_helper(bool_<truth_value>,
                                            static_cast<Then&&>(t),
                                            static_cast<Else&&>(e));
        }
    };

    template <typename C>
    struct not_impl<C, when<
        _models<Constant, C>{}() &&
        _models<Logical, typename C::value_type>{}()
    >> {
        using T = typename C::value_type;
        template <typename Cond>
        struct _constant {
            static constexpr decltype(auto) get()
            { return boost::hana::not_(boost::hana::value<Cond>()); }
            struct hana { using datatype = detail::CanonicalConstant<T>; };
        };
        template <typename Cond>
        static constexpr auto apply(Cond const&)
        { return to<C>(_constant<Cond>{}); }
    };

    template <typename C>
    struct while_impl<C, when<
        _models<Constant, C>{}() &&
        _models<Logical, typename C::value_type>{}()
    >> {
        template <typename Pred, typename State, typename F>
        static constexpr State
        while_helper(decltype(false_), Pred&& /*pred*/, State&& state, F&& /*f*/) {
            return static_cast<State&&>(state);
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        while_helper(decltype(true_), Pred&& pred, State&& state, F&& f) {
            decltype(auto) r = f(static_cast<State&&>(state));
            return hana::while_(static_cast<Pred&&>(pred),
                                static_cast<decltype(r)&&>(r),
                                static_cast<F&&>(f));
        }

        template <typename Pred, typename State, typename F>
        static constexpr decltype(auto)
        apply(Pred&& pred, State&& state, F&& f) {
            // Since `pred(state)` returns a `Constant`, we do not actually
            // need to call it; we only need its decltype. However, we still
            // call it to run potential side effects. I'm not sure whether
            // that is desirable, since we pretty much take for granted that
            // functions are pure, but we'll do it like this for now. Also, I
            // think there is something rather deep hidden behind this, and
            // understanding what must be done here should give us a better
            // understanding of something non-trivial.
            auto cond_ = pred(state);
            constexpr auto cond = hana::value(cond_);
            constexpr bool truth_value = hana::if_(cond, true, false);
            return while_helper(bool_<truth_value>,
                                static_cast<Pred&&>(pred),
                                static_cast<State&&>(state),
                                static_cast<F&&>(f));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_LOGICAL_HPP
