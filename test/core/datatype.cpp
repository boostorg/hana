/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/datatype.hpp>

#include <type_traits>
using namespace boost::hana;


template <typename T, typename ExpectedDatatype>
struct test {
    static_assert(std::is_same<datatype_t<T>, ExpectedDatatype>{}, "");
    static_assert(std::is_same<datatype_t<T const>, ExpectedDatatype>{}, "");
    static_assert(std::is_same<datatype_t<T volatile>, ExpectedDatatype>{}, "");
    static_assert(std::is_same<datatype_t<T const volatile>, ExpectedDatatype>{}, "");

    static_assert(std::is_same<datatype_t<T&>, ExpectedDatatype>{}, "");
    static_assert(std::is_same<datatype_t<T const&>, ExpectedDatatype>{}, "");
    static_assert(std::is_same<datatype_t<T volatile&>, ExpectedDatatype>{}, "");
    static_assert(std::is_same<datatype_t<T const volatile&>, ExpectedDatatype>{}, "");

    static_assert(std::is_same<datatype_t<T&&>, ExpectedDatatype>{}, "");
    static_assert(std::is_same<datatype_t<T const&&>, ExpectedDatatype>{}, "");
    static_assert(std::is_same<datatype_t<T volatile&&>, ExpectedDatatype>{}, "");
    static_assert(std::is_same<datatype_t<T const volatile&&>, ExpectedDatatype>{}, "");
};

struct NestedDatatype;
struct Nested { using hana_datatype = NestedDatatype; };
template struct test<Nested, NestedDatatype>;


struct NoNested { };
template struct test<NoNested, NoNested>;


struct FullySpecializedDatatype;
struct FullySpecialized;
namespace boost { namespace hana {
    template <>
    struct datatype<FullySpecialized> {
        using type = FullySpecializedDatatype;
    };
}}
template struct test<FullySpecialized, FullySpecializedDatatype>;


struct PartiallySpecializedDatatype;
template <typename> struct PartiallySpecialized;
namespace boost { namespace hana {
    template <typename T>
    struct datatype<PartiallySpecialized<T>> {
        using type = PartiallySpecializedDatatype;
    };
}}
template struct test<PartiallySpecialized<struct anything>, PartiallySpecializedDatatype>;


struct PredicatedDatatype;
struct Predicated { static constexpr bool predicate = true; };
namespace boost { namespace hana {
    template <typename T>
    struct datatype<T, when<T::predicate>> {
        using type = PredicatedDatatype;
    };
}}
template struct test<Predicated, PredicatedDatatype>;


int main() { }
