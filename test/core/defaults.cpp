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
            static constexpr bool has_predicated_defaults = false;
        };
    };

    template <>
    struct defaults<Typeclass> {
        template <typename T, typename Enable = void>
        struct with {
            static constexpr bool has_complimentary = false;
            static constexpr bool has_defaults = true;
            static constexpr bool has_predicated_defaults = false;
        };
    };
}}

template <typename T>
struct Typeclass : hana::instance<Typeclass>::template with<T> { };


//////////////////////////////////////////////////////////////////////////////
// Predicated defaults
//////////////////////////////////////////////////////////////////////////////
template <typename T>
struct predicate : std::false_type { };

namespace boost { namespace hana {
    template <typename T>
    struct defaults<Typeclass>::with<T, std::enable_if_t<predicate<T>{}>> {
        static constexpr bool has_complimentary = false;
        static constexpr bool has_defaults = false;
        static constexpr bool has_predicated_defaults = true;
    };
}}

//////////////////////////////////////////////////////////////////////////////
// Data types
//////////////////////////////////////////////////////////////////////////////
struct PredicatedDefaults;
struct NoPredicatedDefaults;
struct SomethingElse;

template <>
struct predicate<PredicatedDefaults> : std::true_type { };

template <>
struct Typeclass<PredicatedDefaults>
    : hana::defaults<Typeclass>::with<PredicatedDefaults>
{ };

template <>
struct Typeclass<NoPredicatedDefaults>
    : hana::defaults<Typeclass>::with<NoPredicatedDefaults>
{ };


int main() {
    static_assert(!Typeclass<PredicatedDefaults>::has_complimentary, "");
    static_assert(!Typeclass<PredicatedDefaults>::has_defaults, "");
    static_assert(Typeclass<PredicatedDefaults>::has_predicated_defaults, "");

    static_assert(!Typeclass<NoPredicatedDefaults>::has_complimentary, "");
    static_assert(Typeclass<NoPredicatedDefaults>::has_defaults, "");
    static_assert(!Typeclass<NoPredicatedDefaults>::has_predicated_defaults, "");

    static_assert(!Typeclass<SomethingElse>::has_complimentary, "");
    static_assert(!Typeclass<SomethingElse>::has_defaults, "");
    static_assert(!Typeclass<SomethingElse>::has_predicated_defaults, "");
}
