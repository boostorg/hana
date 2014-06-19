/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>

#include <type_traits>
namespace hana = boost::hana;


//////////////////////////////////////////////////////////////////////////////
// Type class
//////////////////////////////////////////////////////////////////////////////
template <typename T>
struct Typeclass;

namespace boost { namespace hana {
    template <>
    struct instance<Typeclass> {
        template <typename T, typename Enable = void>
        struct with {
            static constexpr bool has_complimentary = false;
            static constexpr bool has_defaults = false;
            static constexpr bool has_explicit_instance = false;
        };
    };

    template <>
    struct defaults<Typeclass> {
        template <typename T, typename Enable = void>
        struct with : defaults<> {
            static constexpr bool has_complimentary = false;
            static constexpr bool has_defaults = true;
            static constexpr bool has_explicit_instance = false;
        };
    };
}}

template <typename T>
struct Typeclass : hana::instance<Typeclass>::template with<T> { };


//////////////////////////////////////////////////////////////////////////////
// Complimentary instance
//////////////////////////////////////////////////////////////////////////////
template <typename T>
struct predicate : std::false_type { };

namespace boost { namespace hana {
    template <typename T>
    struct instance<Typeclass>::with<T, std::enable_if_t<predicate<T>{}>>
        : defaults<Typeclass>::template with<T>
    {
        static constexpr bool has_complimentary = true;
    };
}}


//////////////////////////////////////////////////////////////////////////////
// Data types
//////////////////////////////////////////////////////////////////////////////
struct AugmentedComplimentary;
struct Complimentary;
struct NoComplimentary;
struct SomethingElse;

template <>
struct predicate<AugmentedComplimentary> : std::true_type { };

template <>
struct predicate<Complimentary> : std::true_type { };

template <>
struct Typeclass<AugmentedComplimentary>
    : hana::instance<Typeclass>::with<AugmentedComplimentary>
{
    static constexpr bool has_explicit_instance = true;
};

template <>
struct Typeclass<NoComplimentary>
    : hana::defaults<Typeclass>::with<NoComplimentary>
{
    static constexpr bool has_explicit_instance = true;
};


int main() {
    static_assert(Typeclass<AugmentedComplimentary>::has_explicit_instance, "");
    static_assert(Typeclass<AugmentedComplimentary>::has_complimentary, "");
    static_assert(Typeclass<AugmentedComplimentary>::has_defaults, "");

    static_assert(!Typeclass<Complimentary>::has_explicit_instance, "");
    static_assert(Typeclass<Complimentary>::has_complimentary, "");
    static_assert(Typeclass<Complimentary>::has_defaults, "");

    static_assert(Typeclass<NoComplimentary>::has_explicit_instance, "");
    static_assert(!Typeclass<NoComplimentary>::has_complimentary, "");
    static_assert(Typeclass<NoComplimentary>::has_defaults, "");

    static_assert(!Typeclass<SomethingElse>::has_explicit_instance, "");
    static_assert(!Typeclass<SomethingElse>::has_complimentary, "");
    static_assert(!Typeclass<SomethingElse>::has_defaults, "");
}
