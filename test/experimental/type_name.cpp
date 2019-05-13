// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/experimental/type_name.hpp>
#include <boost/hana/string.hpp>

#include <cstdlib>
#include <iostream>
#include <regex>
#include <string>
namespace hana = boost::hana;


template <typename ...T>
struct Template { };

template <typename T>
void check_matches(std::string const& re) {
#ifdef _HANA_TN_CAN_CONSTEXPR
    std::string name = hana::to<char const*>(hana::experimental::type_name<T>());
#else
    std::string name = hana::experimental::type_name<T>();
#endif
    std::regex regex{re};
    if (!std::regex_match(name, regex)) {
        std::cerr << "type name '" << name << "' does not match regex '" << re << "'" << std::endl;
        std::abort();
    }
}

template <typename T>
void check_exact(std::string const& expected) {
#ifdef _HANA_TN_CAN_CONSTEXPR
    std::string name = hana::to<char const*>(hana::experimental::type_name<T>());
#else
    std::string name = hana::experimental::type_name<T>();
#endif
    if (name != expected) {
        std::cerr << "type name '" << name << "' does not match expected '" << expected << "'" << std::endl;
        std::abort();
    }
}


int main() {
#ifdef _HANA_TN_CAN_CONSTEXPR
    // Make sure this can be obtained at compile-time
    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::experimental::type_name<void>(),
        BOOST_HANA_STRING("void")
    ));

    BOOST_HANA_CONSTANT_CHECK(hana::equal(
        hana::experimental::type_name<int>(),
        BOOST_HANA_STRING("int")
    ));
#endif

    // Make sure we get something reasonable
    check_matches<int const>("int const|const int");
    check_matches<int&>(R"(int\s*&)");
    check_matches<int const&>(R"(const\s+int\s*&|int\s+const\s*&)");
    check_matches<int(&)[]>(R"(int\s*\(\s*&\s*\)\s*\[\s*\])");
    check_matches<int(&)[10]>(R"(int\s*\(\s*&\s*\)\s*\[\s*10\s*\])");
#ifndef _MSC_VER
    check_matches<Template<void, char const*>>(R"(Template<\s*void\s*,\s*(char const|const char)\s*\*\s*>)");
    check_matches<void(*)(int)>(R"(void\s*\(\s*\*\s*\)\s*\(\s*int\s*\))");
#else
    // MSVC adds superfluous "struct" and/or "__cdecl" keywords
    check_exact<Template<void, char const*>>("struct Template<void,char const *>");
    check_exact<void(*)(int)>("void(__cdecl *)(int)");
#endif
}
