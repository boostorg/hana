/*!
 * @file
 * Defines `boost::hana::Type`.
 *
 *
 * @copyright Louis Dionne 2014
 * Distributed under the Boost Software License, Version 1.0.
 *         (See accompanying file LICENSE.md or copy at
 *             http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_HPP
#define BOOST_HANA_TYPE_HPP

#include <boost/hana/comparable.hpp>
#include <boost/hana/integral.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //! @ingroup datatypes
    template <typename T>
    struct Type { using type = T; };

    template <typename T>
    constexpr Type<T> type{};

    template <typename T, typename U>
    struct Comparable<Type<T>, Type<U>> : defaults<Comparable> {
        static constexpr auto equal_impl(Type<T>, Type<U>)
        { return bool_<std::is_same<T, U>::value>; }
    };

    template <typename T, typename U>
    constexpr auto operator==(Type<T> t, Type<U> u) { return equal(t, u); }
    template <typename T, typename U>
    constexpr auto operator!=(Type<T> t, Type<U> u) { return not_equal(t, u); }

#define BOOST_HANA_WRAP_TYPE_TRAIT(trait)                                   \
    constexpr struct _ ## trait {                                           \
        template <typename T>                                               \
        constexpr Type<typename std::trait<T>::type>                        \
        operator()(Type<T>) const                                           \
        { return {}; }                                                      \
    } trait{};                                                              \
/**/

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

    namespace trait {
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
        BOOST_HANA_WRAP_TYPE_TRAIT(remove_cv)
        BOOST_HANA_WRAP_TYPE_TRAIT(remove_const)
        BOOST_HANA_WRAP_TYPE_TRAIT(remove_volatile)

        BOOST_HANA_WRAP_TYPE_TRAIT(add_cv)
        BOOST_HANA_WRAP_TYPE_TRAIT(add_const)
        BOOST_HANA_WRAP_TYPE_TRAIT(add_volatile)

        // References
        BOOST_HANA_WRAP_TYPE_TRAIT(remove_reference)
        BOOST_HANA_WRAP_TYPE_TRAIT(add_lvalue_reference)
        BOOST_HANA_WRAP_TYPE_TRAIT(add_rvalue_reference)

        // Pointers
        BOOST_HANA_WRAP_TYPE_TRAIT(remove_pointer)
        BOOST_HANA_WRAP_TYPE_TRAIT(add_pointer)

        // Sign modifiers
        BOOST_HANA_WRAP_TYPE_TRAIT(make_signed)
        BOOST_HANA_WRAP_TYPE_TRAIT(make_unsigned)

        // Arrays
        BOOST_HANA_WRAP_TYPE_TRAIT(remove_extent)
        BOOST_HANA_WRAP_TYPE_TRAIT(remove_all_extents)

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

        BOOST_HANA_WRAP_TYPE_TRAIT(decay)
        // enable_if
        // disable_if
        // conditional

        constexpr struct _common_type {
            template <typename ...T>
            constexpr Type<typename std::common_type<T...>::type>
            operator()(Type<T>...) const { return {}; }
        } common_type{};

        BOOST_HANA_WRAP_TYPE_TRAIT(underlying_type)
        BOOST_HANA_WRAP_TYPE_TRAIT(result_of)
    } // end namespace trait

#undef BOOST_HANA_WRAP_TYPE_TRAIT
#undef BOOST_HANA_BOOLEAN_TYPE_TRAIT
#undef BOOST_HANA_BOOLEAN_BINARY_TYPE_TRAIT
#undef BOOST_HANA_BOOLEAN_NARY_TYPE_TRAIT
#undef BOOST_HANA_INTEGRAL_TYPE_TRAIT
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_HPP
