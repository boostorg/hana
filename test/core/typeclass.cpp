/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>

#include <type_traits>
namespace hana = boost::hana;


struct Typeclass;
namespace boost { namespace hana {
    BOOST_HANA_TYPECLASS_BOILERPLATE(::Typeclass)
}}
struct Typeclass : hana::typeclass<Typeclass> {
    struct default_ {
        static constexpr bool has_explicit_instance = false;
        static constexpr bool has_predicated_instance = false;
        static constexpr bool has_defaults = true;
    };
};

struct ExplicitInstance;
struct PredicatedInstance;
struct NotInstance;

template <>
struct Typeclass::instance<ExplicitInstance> {
    static constexpr bool has_explicit_instance = true;
    static constexpr bool has_predicated_instance = false;
    static constexpr bool has_defaults = false;
};

template <typename T>
struct Typeclass::instance<T, std::enable_if_t<std::is_same<T, PredicatedInstance>{}>> {
    static constexpr bool has_explicit_instance = false;
    static constexpr bool has_predicated_instance = true;
    static constexpr bool has_defaults = false;
};

static_assert(!Typeclass::instance<ExplicitInstance>::has_defaults, "");
static_assert(!Typeclass::instance<PredicatedInstance>::has_defaults, "");
static_assert(Typeclass::instance<NotInstance>::has_defaults, "");

static_assert(Typeclass::instance<ExplicitInstance>::has_explicit_instance, "");
static_assert(!Typeclass::instance<PredicatedInstance>::has_explicit_instance, "");
static_assert(!Typeclass::instance<NotInstance>::has_explicit_instance, "");

static_assert(!Typeclass::instance<ExplicitInstance>::has_predicated_instance, "");
static_assert(Typeclass::instance<PredicatedInstance>::has_predicated_instance, "");
static_assert(!Typeclass::instance<NotInstance>::has_predicated_instance, "");

int main() { }
