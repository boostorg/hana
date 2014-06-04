/*!
@file
Defines everything in the `boost::hana::trait` namespace.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TRAIT_HPP
#define BOOST_HANA_TRAIT_HPP

#include <boost/hana/integral.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>


namespace boost { namespace hana { namespace trait {
    template <template <typename ...> class Trait>
    struct integral_trait {
        template <typename ...T>
        constexpr auto operator()(Type<T>...) const
        { return integral<decltype(Trait<T...>::value), Trait<T...>::value>; }
    };

    ///////////////////
    // Type properties
    ///////////////////
    // Primary type categories
    constexpr auto is_void = integral_trait<std::is_void>{};
    constexpr auto is_null_pointer = integral_trait<std::is_null_pointer>{};
    constexpr auto is_integral = integral_trait<std::is_integral>{};
    constexpr auto is_floating_point = integral_trait<std::is_floating_point>{};
    constexpr auto is_array = integral_trait<std::is_array>{};
    constexpr auto is_enum = integral_trait<std::is_enum>{};
    constexpr auto is_union = integral_trait<std::is_union>{};
    constexpr auto is_class = integral_trait<std::is_class>{};
    constexpr auto is_function = integral_trait<std::is_function>{};
    constexpr auto is_pointer = integral_trait<std::is_pointer>{};
    constexpr auto is_lvalue_reference = integral_trait<std::is_lvalue_reference>{};
    constexpr auto is_rvalue_reference = integral_trait<std::is_rvalue_reference>{};
    constexpr auto is_member_object_pointer = integral_trait<std::is_member_object_pointer>{};
    constexpr auto is_member_function_pointer = integral_trait<std::is_member_function_pointer>{};

    // Composite type categories
    constexpr auto is_fundamental = integral_trait<std::is_fundamental>{};
    constexpr auto is_arithmetic = integral_trait<std::is_arithmetic>{};
    constexpr auto is_scalar = integral_trait<std::is_scalar>{};
    constexpr auto is_object = integral_trait<std::is_object>{};
    constexpr auto is_compound = integral_trait<std::is_compound>{};
    constexpr auto is_reference = integral_trait<std::is_reference>{};
    constexpr auto is_member_pointer = integral_trait<std::is_member_pointer>{};

    // Type properties
    constexpr auto is_const = integral_trait<std::is_const>{};
    constexpr auto is_volatile = integral_trait<std::is_volatile>{};
    constexpr auto is_trivial = integral_trait<std::is_trivial>{};
    constexpr auto is_trivially_copyable = integral_trait<std::is_trivially_copyable>{};
    constexpr auto is_standard_layout = integral_trait<std::is_standard_layout>{};
    constexpr auto is_pod = integral_trait<std::is_pod>{};
    constexpr auto is_literal_type = integral_trait<std::is_literal_type>{};
    constexpr auto is_empty = integral_trait<std::is_empty>{};
    constexpr auto is_polymorphic = integral_trait<std::is_polymorphic>{};
    constexpr auto is_abstract = integral_trait<std::is_abstract>{};
    constexpr auto is_signed = integral_trait<std::is_signed>{};
    constexpr auto is_unsigned = integral_trait<std::is_unsigned>{};

    // Supported operations
    constexpr auto is_constructible = integral_trait<std::is_constructible>{};
    constexpr auto is_trivially_constructible = integral_trait<std::is_trivially_constructible>{};
    constexpr auto is_nothrow_constructible = integral_trait<std::is_nothrow_constructible>{};

    constexpr auto is_default_constructible = integral_trait<std::is_default_constructible>{};
    constexpr auto is_trivially_default_constructible = integral_trait<std::is_trivially_default_constructible>{};
    constexpr auto is_nothrow_default_constructible = integral_trait<std::is_nothrow_default_constructible>{};

    constexpr auto is_copy_constructible = integral_trait<std::is_copy_constructible>{};
    constexpr auto is_trivially_copy_constructible = integral_trait<std::is_trivially_copy_constructible>{};
    constexpr auto is_nothrow_copy_constructible = integral_trait<std::is_nothrow_copy_constructible>{};

    constexpr auto is_move_constructible = integral_trait<std::is_move_constructible>{};
    constexpr auto is_trivially_move_constructible = integral_trait<std::is_trivially_move_constructible>{};
    constexpr auto is_nothrow_move_constructible = integral_trait<std::is_nothrow_move_constructible>{};

    constexpr auto is_assignable = integral_trait<std::is_assignable>{};
    constexpr auto is_trivially_assignable = integral_trait<std::is_trivially_assignable>{};
    constexpr auto is_nothrow_assignable = integral_trait<std::is_nothrow_assignable>{};

    constexpr auto is_copy_assignable = integral_trait<std::is_copy_assignable>{};
    constexpr auto is_trivially_copy_assignable = integral_trait<std::is_trivially_copy_assignable>{};
    constexpr auto is_nothrow_copy_assignable = integral_trait<std::is_nothrow_copy_assignable>{};

    constexpr auto is_move_assignable = integral_trait<std::is_move_assignable>{};
    constexpr auto is_trivially_move_assignable = integral_trait<std::is_trivially_move_assignable>{};
    constexpr auto is_nothrow_move_assignable = integral_trait<std::is_nothrow_move_assignable>{};

    constexpr auto is_destructible = integral_trait<std::is_destructible>{};
    constexpr auto is_trivially_destructible = integral_trait<std::is_trivially_destructible>{};
    constexpr auto is_nothrow_destructible = integral_trait<std::is_nothrow_destructible>{};

    constexpr auto has_virtual_destructor = integral_trait<std::has_virtual_destructor>{};

    // Property queries
    constexpr auto alignment_of = integral_trait<std::alignment_of>{};
    constexpr auto rank = integral_trait<std::rank>{};
    constexpr struct _extent {
        template <typename T, typename N>
        constexpr auto operator()(Type<T>, N n) const {
            return integral<
                decltype(std::extent<T, n>::value), std::extent<T, n>::value
            >;
        }

        template <typename T>
        constexpr auto operator()(Type<T> t) const
        { return (*this)(t, uint<0>); }
    } extent{};

    // Type relationships
    constexpr auto is_same = integral_trait<std::is_same>{};
    constexpr auto is_base_of = integral_trait<std::is_base_of>{};
    constexpr auto is_convertible = integral_trait<std::is_convertible>{};

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
        template <typename Len, typename Align>
        constexpr auto operator()(Len len, Align align) const
        { return type<std::aligned_storage_t<len, align>>; }

        template <typename Len>
        constexpr auto operator()(Len len) const
        { return type<std::aligned_storage_t<len>>; }
    } aligned_storage{};

    constexpr struct _aligned_union {
        template <typename Len, typename ...T>
        constexpr auto operator()(Len len, Type<T>...) const
        { return type<std::aligned_union_t<len, T...>>; }
    } aligned_union{};

    constexpr auto decay = lift<std::decay_t>;
    // enable_if
    // disable_if
    // conditional

    constexpr auto common_type = lift<std::common_type_t>;
    constexpr auto underlying_type = lift<std::underlying_type_t>;
    constexpr auto result_of = lift<std::result_of_t>;
}}} // end namespace boost::hana::trait

#endif // !BOOST_HANA_TRAIT_HPP
