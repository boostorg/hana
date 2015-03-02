/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/ext/std/type_traits.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


enum Enumeration { };
struct Structure { };
constexpr auto e = type<Enumeration>;
constexpr auto s = type<Structure>;

int main() {
    // We just make sure that they compile. If the forwarding to `std::` is
    // well done, it is the job of `std::` to return the right thing.

    ///////////////////
    // Type properties
    ///////////////////
    // Primary type categories
    static_assert(!traits::is_void(s), "the traits should be compile-time checkable");
    traits::is_null_pointer(s);
    traits::is_integral(s);
    traits::is_floating_point(s);
    traits::is_array(s);
    traits::is_enum(s);
    traits::is_union(s);
    traits::is_class(s);
    traits::is_function(s);
    traits::is_pointer(s);
    traits::is_lvalue_reference(s);
    traits::is_rvalue_reference(s);
    traits::is_member_object_pointer(s);
    traits::is_member_function_pointer(s);

    // Composite type categories
    traits::is_fundamental(s);
    traits::is_arithmetic(s);
    traits::is_scalar(s);
    traits::is_object(s);
    traits::is_compound(s);
    traits::is_reference(s);
    traits::is_member_pointer(s);

    // Type properties
    traits::is_const(s);
    traits::is_volatile(s);
    traits::is_trivial(s);
    traits::is_trivially_copyable(s);
    traits::is_standard_layout(s);
    traits::is_pod(s);
    traits::is_literal_type(s);
    traits::is_empty(s);
    traits::is_polymorphic(s);
    traits::is_abstract(s);
    traits::is_signed(s);
    traits::is_unsigned(s);

    // Supported operations
    traits::is_constructible(s, s);
    traits::is_trivially_constructible(s, s);
    traits::is_nothrow_constructible(s, s);

    traits::is_default_constructible(s);
    traits::is_trivially_default_constructible(s);
    traits::is_nothrow_default_constructible(s);

    traits::is_copy_constructible(s);
    traits::is_trivially_copy_constructible(s);
    traits::is_nothrow_copy_constructible(s);

    traits::is_move_constructible(s);
    traits::is_trivially_move_constructible(s);
    traits::is_nothrow_move_constructible(s);

    traits::is_assignable(s, s);
    traits::is_trivially_assignable(s, s);
    traits::is_nothrow_assignable(s, s);

    traits::is_copy_assignable(s);
    traits::is_trivially_copy_assignable(s);
    traits::is_nothrow_copy_assignable(s);

    traits::is_move_assignable(s);
    traits::is_trivially_move_assignable(s);
    traits::is_nothrow_move_assignable(s);

    traits::is_destructible(s);
    traits::is_trivially_destructible(s);
    traits::is_nothrow_destructible(s);

    traits::has_virtual_destructor(s);

    // Property queries
    traits::alignment_of(s);
    traits::rank(s);
    traits::extent(s);
    traits::extent(type<int[2][3]>, uint<1>);

    // Type relationships
    traits::is_same(s, s);
    traits::is_base_of(s, s);
    traits::is_convertible(s, s);

    ///////////////////
    // Type modifications
    ///////////////////
    // Const-volatility specifiers
    traits::remove_cv(s);
    traits::remove_const(s);
    traits::remove_volatile(s);

    traits::add_cv(s);
    traits::add_const(s);
    traits::add_volatile(s);

    // References
    traits::remove_reference(s);
    traits::add_lvalue_reference(s);
    traits::add_rvalue_reference(s);

    // Pointers
    traits::remove_pointer(s);
    traits::add_pointer(s);

    // Sign modifiers
    traits::make_signed(type<unsigned>);
    traits::make_unsigned(type<signed>);

    // Arrays
    traits::remove_extent(s);
    traits::remove_all_extents(s);

    // Miscellaneous transformations
    traits::aligned_storage(size_t<1>);
    traits::aligned_storage(size_t<1>, size_t<1>);
    traits::aligned_union(size_t<0>, s);
    traits::decay(s);

    traits::common_type(s, s);
    traits::underlying_type(e);
    using FunctionPointer = void(*)();
    traits::result_of(type<FunctionPointer(void)>);
}
