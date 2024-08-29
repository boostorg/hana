/*!
@file
Facility to create a compile-time abstract syntax tree (AST) that
represents a compound predicate composed of class template predicates.
Each predicate takes a type_c<T> instance and produce a compile time
boolean result when specialized.

Copyright Louis Dionne 2013-2022
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

//
// Copyright (c) 2017 by Daniel Saks and Stephen C. Dewhurst.
//
//
// Copyright (c) 2017 by Jia yue hua
//
// Permission to use, copy, modify, distribute, and sell this
// software for any purpose is hereby granted without fee, provided
// that the above copyright notice appears in all copies and
// that both that copyright notice and this permission notice
// appear in supporting documentation.
// The authors make no representation about the suitability of this
// software for any purpose. It is provided "as is" without express
// or implied warranty.
//

#ifndef INCLUDED_PRED_EXPR
#define INCLUDED_PRED_EXPR

#include <type_traits>

#include <boost/hana/traits.hpp>
#include <boost/hana/all.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

//
// Facility to create a compile-time abstract syntax tree (AST) that
// represents a compound predicate composed of class template predicates.
// Each predicate takes a type_c<T> instance and produce a compile time
// boolean result when specialized.
//
// Can be used to make "poor man's concepts" less impoverished.
//
namespace boost
{
namespace hana
{
namespace traits
{
namespace detail
{
//
// Convenience template for generating predicates.
//
template <bool c>
using truthiness = std::conditional_t<c, std::true_type, std::false_type>;

//
// Quick check to see if a series of zero or more bools all evaluate to true.
//
template <bool... bs>
struct AllTrue {
    static constexpr auto value = all(make_tuple(bs...));
};

//
// See if each Ts is derived from Base.
//
// template <typename Base, typename... Ts>
// using all_derived_from = AllTrue<std::is_base_of<Base,Ts>::value...>;

struct PAST {
};

template <class T>
struct IsTraits {
};
template <template <class... T> class TC>
struct IsTraits<hana_trait<TC>> {
    typedef int type;
};
template <class T>
using TraitsType = typename IsTraits<std::remove_cv_t<T>>::type;

auto is_traits_impl = is_valid([](auto obj) -> TraitsType<typename decltype(obj)::type> {});
template <class T>
constexpr bool isTraits = is_traits_impl(type_c<T>);
template <typename... T>
using AllPAstImpl = AllTrue<(isTraits<T> || std::is_base_of<PAST, T>::value)...>;
template <typename... T>
using AllPAst = std::enable_if_t<AllPAstImpl<T...>::value, void *>;
//
//! The language consists of type predicate leaves
//! the binary operators &, |, and ^ (and, or, and exclusive or),
//! and ! (negation).
//!
//! We use the compiler's parser to parse the expressions so (of course)
//! the precedence and associativity match those of C++.
//!
//! We leverage the compiler's symbol table to build our compile-time
//! ASTs and evaluate them with compile-time template arguments.
//!
//!  This facility produces no runtime executable code.

//====================
// AST leaf nodes:
//     a) ids that wrap a predicate template that can be specilaized with one typename
//     b) binders that bind an argument of a binary predicate to produce an id

// An identifier type that wraps a predicate template.  The template
// takes a typename and passes judgement on it at compile time.
//
// Note use of variadic template template to allow predicates that take trailing defaults.
//
template <template <typename...> class Pred>
struct Id : PAST {
    template <typename T>
    static constexpr bool eval()
    {
        return Pred<T>::value;
    }
};

template <template <typename...> class Pred>
constexpr auto pred()
{
    return Id<Pred>();
}

template <class BPred, typename First>
struct BindT1st : PAST {
    template <typename Second>
    constexpr bool operator()(Second s) const
    {
        BPred b = {};
        First f = {};
        return b(f, s);
    }
};
template <class BPred, typename First, AllPAst<BPred> = nullptr>
constexpr auto bind1st(BPred, First)
{
    return BindT1st<BPred, First>();
}
template <class BPred, typename Second>
struct BindT2nd : PAST {
    template <typename First>
    constexpr bool operator()(First f) const
    {
        BPred  b = {};
        Second s = {};
        return b(f, s);
    }
};
template <class BPred, typename Second, AllPAst<BPred> = nullptr>
constexpr auto bind2nd(BPred, Second)
{
    return BindT2nd<BPred, Second>();
}

struct Base {
};

template <typename P1, typename P2>
struct And : PAST {
    template <class T>
    constexpr bool operator()(T t) const
    {
        P1 p1 = {};
        P2 p2 = {};
        return p1(t) & p2(t);
    }
};

template <typename P1, typename P2>
struct Or : PAST {
    template <class T>
    constexpr bool operator()(T t) const
    {
        P1 p1 = {};
        P2 p2 = {};
        return p1(t) | p2(t);
    }
};
template <typename P1, typename P2>
struct Xor : PAST {
    template <class T>
    constexpr bool operator()(T t) const
    {
        P1 p1 = {};
        P2 p2 = {};
        return p1(t) ^ p2(t);
    }
};
template <typename P1, typename P2, AllPAst<P1, P2> = nullptr>
constexpr auto operator^(P1, P2)
{
    Xor<P1, P2> a = {};
    return a;
}
template <typename P>
struct Not : PAST {
    template <class T, AllPAst<P> = nullptr>
    constexpr bool operator()(T t) const
    {
        P p = {};
        return p(t) ? 0 : 1;
    }
};

template <typename P, detail::AllPAst<P> = nullptr>
constexpr auto operator!(P)
{
    return detail::Not<P>();
}

template <typename P1, typename P2, detail::AllPAst<P1, P2> = nullptr>
constexpr auto operator&(P1, P2)
{
    detail::And<P1, P2> a = {};
    return a;
}
template <typename P1, typename P2, detail::AllPAst<P1, P2> = nullptr>
constexpr auto operator|(P1, P2)
{
    detail::Or<P1, P2> a = {};
    return a;
}
}  // namespace detail

}  // namespace traits
}  // namespace hana
}  // namespace boost
#endif
