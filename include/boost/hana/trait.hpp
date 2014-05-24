/*!
 * @file
 * Defines everything in the `boost::hana::trait` namespace.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TRAIT_HPP
#define BOOST_HANA_TRAIT_HPP

#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>

#include <cstddef>
#include <type_traits>


namespace boost { namespace hana { namespace trait {
#define BOOST_HANA_BOOLEAN_TYPE_TRAIT(trait)                                \
    constexpr struct _ ## trait {                                           \
        template <typename T>                                               \
        constexpr Bool<std::trait<T>::value>                                \
        operator()(Type<T>) const                                           \
        { return {}; }                                                      \
    } trait{};                                                              \
/**/

#define BOOST_HANA_BOOLEAN_BINARY_TYPE_TRAIT(trait)                         \
    constexpr struct _ ## trait {                                           \
        template <typename T, typename U>                                   \
        constexpr Bool<std::trait<T, U>::value>                             \
        operator()(Type<T>, Type<U>) const                                  \
        { return {}; }                                                      \
    } trait{};                                                              \
/**/

#define BOOST_HANA_BOOLEAN_NARY_TYPE_TRAIT(trait)                           \
    constexpr struct _ ## trait {                                           \
        template <typename T, typename ...U>                                \
        constexpr Bool<std::trait<T, U...>::value>                          \
        operator()(Type<T>, Type<U>...) const                               \
        { return {}; }                                                      \
    } trait{};                                                              \
/**/

#define BOOST_HANA_INTEGRAL_TYPE_TRAIT(trait)                               \
    constexpr struct _ ## trait {                                           \
        template <typename T>                                               \
        constexpr Integral<                                                 \
            decltype(std::trait<T>::value), std::trait<T>::value            \
        > operator()(Type<T>) const                                         \
        { return {}; }                                                      \
    } trait{};                                                              \
/**/

    ///////////////////
    // Type properties
    ///////////////////
    // Primary type categories
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_void)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_null_pointer)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_integral)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_floating_point)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_array)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_enum)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_union)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_class)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_function)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_pointer)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_lvalue_reference)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_rvalue_reference)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_member_object_pointer)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_member_function_pointer)

    // Composite type categories
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_fundamental)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_arithmetic)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_scalar)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_object)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_compound)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_reference)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_member_pointer)

    // Type properties
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_const)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_volatile)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_trivial)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_trivially_copyable)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_standard_layout)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_pod)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_literal_type)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_empty)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_polymorphic)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_abstract)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_signed)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_unsigned)

    // Supported operations
    BOOST_HANA_BOOLEAN_NARY_TYPE_TRAIT(is_constructible)
    BOOST_HANA_BOOLEAN_NARY_TYPE_TRAIT(is_trivially_constructible)
    BOOST_HANA_BOOLEAN_NARY_TYPE_TRAIT(is_nothrow_constructible)

    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_default_constructible)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_trivially_default_constructible)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_nothrow_default_constructible)

    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_copy_constructible)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_trivially_copy_constructible)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_nothrow_copy_constructible)

    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_move_constructible)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_trivially_move_constructible)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_nothrow_move_constructible)

    BOOST_HANA_BOOLEAN_BINARY_TYPE_TRAIT(is_assignable)
    BOOST_HANA_BOOLEAN_BINARY_TYPE_TRAIT(is_trivially_assignable)
    BOOST_HANA_BOOLEAN_BINARY_TYPE_TRAIT(is_nothrow_assignable)

    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_copy_assignable)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_trivially_copy_assignable)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_nothrow_copy_assignable)

    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_move_assignable)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_trivially_move_assignable)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_nothrow_move_assignable)

    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_destructible)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_trivially_destructible)
    BOOST_HANA_BOOLEAN_TYPE_TRAIT(is_nothrow_destructible)

    BOOST_HANA_BOOLEAN_TYPE_TRAIT(has_virtual_destructor)

    // Property queries
    BOOST_HANA_INTEGRAL_TYPE_TRAIT(alignment_of)
    BOOST_HANA_INTEGRAL_TYPE_TRAIT(rank)
    BOOST_HANA_INTEGRAL_TYPE_TRAIT(extent)

    // Type relationships
    BOOST_HANA_BOOLEAN_BINARY_TYPE_TRAIT(is_same)
    BOOST_HANA_BOOLEAN_BINARY_TYPE_TRAIT(is_base_of)
    BOOST_HANA_BOOLEAN_BINARY_TYPE_TRAIT(is_convertible)

    ///////////////////
    // Type modifications
    ///////////////////
    // Const-volatility specifiers
    constexpr auto remove_cv = lift<std::remove_cv_t>;
    constexpr auto remove_const = lift<std::remove_const_t>;
    constexpr auto remove_volatile = lift<std::remove_volatile_t>;

    constexpr auto add_cv = lift<std::add_cv_t>;
    constexpr auto add_const = lift<std::add_const_t>;
    constexpr auto add_volatile = lift<std::add_volatile_t>;

    // References
    constexpr auto remove_reference = lift<std::remove_reference_t>;
    constexpr auto add_lvalue_reference = lift<std::add_lvalue_reference_t>;
    constexpr auto add_rvalue_reference = lift<std::add_rvalue_reference_t>;

    // Pointers
    constexpr auto remove_pointer = lift<std::remove_pointer_t>;
    constexpr auto add_pointer = lift<std::add_pointer_t>;

    // Sign modifiers
    constexpr auto make_signed = lift<std::make_signed_t>;
    constexpr auto make_unsigned = lift<std::make_unsigned_t>;

    // Arrays
    constexpr auto remove_extent = lift<std::remove_extent_t>;
    constexpr auto remove_all_extents = lift<std::remove_all_extents_t>;

    // Miscellaneous transformations
    constexpr struct _aligned_storage {
        template <std::size_t Len, std::size_t Align>
        constexpr typename std::aligned_storage<Len, Align>::type
        operator()(SizeT<Len>, SizeT<Align>) const { return {}; }

        template <std::size_t Len>
        constexpr typename std::aligned_storage<Len>::type
        operator()(SizeT<Len>) const { return {}; }
    } aligned_storage{};

    constexpr struct _aligned_union {
        template <std::size_t Len, typename ...T>
        constexpr typename std::aligned_union<Len, T...>::type
        operator()(SizeT<Len>, Type<T>...) const { return {}; }
    } aligned_union{};

    constexpr auto decay = lift<std::decay_t>;
    // enable_if
    // disable_if
    // conditional

    constexpr auto common_type = lift<std::common_type_t>;
    constexpr auto underlying_type = lift<std::underlying_type_t>;
    constexpr auto result_of = lift<std::result_of_t>;

#undef BOOST_HANA_BOOLEAN_TYPE_TRAIT
#undef BOOST_HANA_BOOLEAN_BINARY_TYPE_TRAIT
#undef BOOST_HANA_BOOLEAN_NARY_TYPE_TRAIT
#undef BOOST_HANA_INTEGRAL_TYPE_TRAIT
}}} // end namespace boost::hana::trait

#endif // !BOOST_HANA_TRAIT_HPP
