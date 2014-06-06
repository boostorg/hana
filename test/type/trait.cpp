/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/trait.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>
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
    BOOST_HANA_STATIC_ASSERT(decltype_(trait::is_void(s)) == decltype_(false_));
    trait::is_null_pointer(s);
    trait::is_integral(s);
    trait::is_floating_point(s);
    trait::is_array(s);
    trait::is_enum(s);
    trait::is_union(s);
    trait::is_class(s);
    trait::is_function(s);
    trait::is_pointer(s);
    trait::is_lvalue_reference(s);
    trait::is_rvalue_reference(s);
    trait::is_member_object_pointer(s);
    trait::is_member_function_pointer(s);

    // Composite type categories
    trait::is_fundamental(s);
    trait::is_arithmetic(s);
    trait::is_scalar(s);
    trait::is_object(s);
    trait::is_compound(s);
    trait::is_reference(s);
    trait::is_member_pointer(s);

    // Type properties
    trait::is_const(s);
    trait::is_volatile(s);
    trait::is_trivial(s);
    trait::is_trivially_copyable(s);
    trait::is_standard_layout(s);
    trait::is_pod(s);
    trait::is_literal_type(s);
    trait::is_empty(s);
    trait::is_polymorphic(s);
    trait::is_abstract(s);
    trait::is_signed(s);
    trait::is_unsigned(s);

    // Supported operations
    trait::is_constructible(s, s);
    trait::is_trivially_constructible(s, s);
    trait::is_nothrow_constructible(s, s);

    trait::is_default_constructible(s);
    trait::is_trivially_default_constructible(s);
    trait::is_nothrow_default_constructible(s);

    trait::is_copy_constructible(s);
    trait::is_trivially_copy_constructible(s);
    trait::is_nothrow_copy_constructible(s);

    trait::is_move_constructible(s);
    trait::is_trivially_move_constructible(s);
    trait::is_nothrow_move_constructible(s);

    trait::is_assignable(s, s);
    trait::is_trivially_assignable(s, s);
    trait::is_nothrow_assignable(s, s);

    trait::is_copy_assignable(s);
    trait::is_trivially_copy_assignable(s);
    trait::is_nothrow_copy_assignable(s);

    trait::is_move_assignable(s);
    trait::is_trivially_move_assignable(s);
    trait::is_nothrow_move_assignable(s);

    trait::is_destructible(s);
    trait::is_trivially_destructible(s);
    trait::is_nothrow_destructible(s);

    trait::has_virtual_destructor(s);

    // Property queries
    trait::alignment_of(s);
    trait::rank(s);
    trait::extent(s);
    trait::extent(type<int[2][3]>, uint<1>);

    // Type relationships
    trait::is_same(s, s);
    trait::is_base_of(s, s);
    trait::is_convertible(s, s);

    ///////////////////
    // Type modifications
    ///////////////////
    // Const-volatility specifiers
    trait::remove_cv(s);
    trait::remove_const(s);
    trait::remove_volatile(s);

    trait::add_cv(s);
    trait::add_const(s);
    trait::add_volatile(s);

    // References
    trait::remove_reference(s);
    trait::add_lvalue_reference(s);
    trait::add_rvalue_reference(s);

    // Pointers
    trait::remove_pointer(s);
    trait::add_pointer(s);

    // Sign modifiers
    trait::make_signed(type<unsigned>);
    trait::make_unsigned(type<signed>);

    // Arrays
    trait::remove_extent(s);
    trait::remove_all_extents(s);

    // Miscellaneous transformations
    trait::aligned_storage(size_t<1>);
    trait::aligned_storage(size_t<1>, size_t<1>);
    trait::aligned_union(size_t<0>, s);
    trait::decay(s);

    trait::common_type(s, s);
    trait::underlying_type(e);
    using FunctionPointer = void(*)();
    trait::result_of(type<FunctionPointer(void)>);
}
