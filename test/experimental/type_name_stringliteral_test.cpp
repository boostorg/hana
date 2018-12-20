#include <boost/hana/experimental/detail/type_name_pretty_function.hpp>

#include <cstdlib>
#include <cstring>
#include <regex>
#include <iostream>

namespace type_name_details = boost::hana::experimental::type_name_details;


#ifdef _HANA_TN_CAN_CONSTEXPR
    #define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)    \
        static_assert(                                                              \
            type_name_details::stringliteral_equal_sz(                              \
                type_name_details::type_name_impl_stringliteral<type_definition>(), \
                type_string_literal),                                               \
            "RUN_ONE_TYPE_TEST_COMPILE_TIME error");
#else
    #define RUN_ONE_TYPE_TEST_COMPILE_TIME(type_definition, type_string_literal)
#endif


template<typename... T>
struct Template {
};


template <typename T>
void check_matches(std::string const& re) {
    type_name_details::stringliteral name_cs = type_name_details::type_name_impl_stringliteral<T>();
    std::string name = type_name_details::stringliteral_to_string(name_cs);
    std::regex regex{re};
    if (!std::regex_match(name, regex)) {
        std::cerr << "type name '" << name << "' does not match regex '" << re << "'" << std::endl;
        std::abort();
    }
}

template <typename T>
void check_exact(std::string const& expected) {
    type_name_details::stringliteral name_cs = type_name_details::type_name_impl_stringliteral<T>();
    std::string name = type_name_details::stringliteral_to_string(name_cs);
    if (name != expected) {
        std::cerr << "type name '" << name << "' does not match expected '" << expected << "'" << std::endl;
        std::abort();
    }
}


void compile_time_tests() {
    RUN_ONE_TYPE_TEST_COMPILE_TIME(void, "void");
    RUN_ONE_TYPE_TEST_COMPILE_TIME(char, "char");

    // __PRETTY_FUNCTION__ seems to favor west-const
    // (however its behavior is somewhat inconsistent on complex types)
    //
    // On the contrary, typeid().name() is consistently east const accross all compilers
    // Does this need to be tested ?
    RUN_ONE_TYPE_TEST_COMPILE_TIME(const char, "const char");
    RUN_ONE_TYPE_TEST_COMPILE_TIME(char const, "const char");
}


void runtime_regex_tests() {
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


int main() {
    compile_time_tests();
    runtime_regex_tests();
    return 0;
}
