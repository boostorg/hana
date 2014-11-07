/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename T>
struct predicated_instance;

template <typename T>
struct valid_expr_instance;

template <typename T>
struct partially_specialized_instance;

template <typename T>
struct explicitly_specialized_instance;

template <typename T>
struct defaulted_instance;


struct Typeclass {
    BOOST_HANA_TYPECLASS(Typeclass);
    template <typename T>
    struct default_instance {
        using which = defaulted_instance<T>;
    };
};

template <typename T>
struct predicate { static constexpr bool value = false; };

template <typename T>
struct Typeclass::instance<T, when<predicate<T>::value>> {
    using which = predicated_instance<T>;
};

template <typename T>
struct Typeclass::instance<T, when_valid<typename T::is_valid_expr>> {
    using which = valid_expr_instance<T>;
};


template <typename T> struct PredicateAndPartialSpec;
template <typename T> struct ValidExprAndPartialSpec { struct is_valid_expr; };
template <typename T> struct PartialSpec;
struct Predicate;
struct PredicateAndFullSpec;
struct ValidExprAndFullSpec { struct is_valid_expr; };
struct FullSpec;
struct ValidExpr { struct is_valid_expr; };
struct NotInstance;


template <typename T>
struct predicate<PredicateAndPartialSpec<T>> { static constexpr bool value = true; };
template <>
struct predicate<Predicate> { static constexpr bool value = true; };
template <>
struct predicate<PredicateAndFullSpec> { static constexpr bool value = true; };


template <typename T>
struct Typeclass::instance<PredicateAndPartialSpec<T>> {
    using which = partially_specialized_instance<PredicateAndPartialSpec<T>>;
};
template <typename T>
struct Typeclass::instance<ValidExprAndPartialSpec<T>> {
    using which = partially_specialized_instance<ValidExprAndPartialSpec<T>>;
};
template <>
struct Typeclass::instance<PredicateAndFullSpec> {
    using which = explicitly_specialized_instance<PredicateAndFullSpec>;
};
template <>
struct Typeclass::instance<ValidExprAndFullSpec> {
    using which = explicitly_specialized_instance<ValidExprAndFullSpec>;
};
template <>
struct Typeclass::instance<FullSpec> {
    using which = explicitly_specialized_instance<FullSpec>;
};
template <typename T>
struct Typeclass::instance<PartialSpec<T>> {
    using which = partially_specialized_instance<PartialSpec<T>>;
};


static_assert(std::is_same<
    Typeclass::instance<PredicateAndPartialSpec<void>>::which,
    partially_specialized_instance<PredicateAndPartialSpec<void>>
>::value, "");

static_assert(std::is_same<
    Typeclass::instance<ValidExprAndPartialSpec<void>>::which,
    partially_specialized_instance<ValidExprAndPartialSpec<void>>
>::value, "");

static_assert(std::is_same<
    Typeclass::instance<PredicateAndFullSpec>::which,
    explicitly_specialized_instance<PredicateAndFullSpec>
>::value, "");

static_assert(std::is_same<
    Typeclass::instance<ValidExprAndFullSpec>::which,
    explicitly_specialized_instance<ValidExprAndFullSpec>
>::value, "");

static_assert(std::is_same<
    Typeclass::instance<Predicate>::which,
    predicated_instance<Predicate>
>::value, "");

static_assert(std::is_same<
    Typeclass::instance<ValidExpr>::which,
    valid_expr_instance<ValidExpr>
>::value, "");

static_assert(std::is_same<
    Typeclass::instance<PartialSpec<void>>::which,
    partially_specialized_instance<PartialSpec<void>>
>::value, "");

static_assert(std::is_same<
    Typeclass::instance<FullSpec>::which,
    explicitly_specialized_instance<FullSpec>
>::value, "");

static_assert(std::is_same<
    Typeclass::instance<NotInstance>::which,
    defaulted_instance<NotInstance>
>::value, "");

int main() { }
