/*!
@file
Defines `unit test`.

@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */
template <typename C>
int basic_strcmp(C const* l, C const* r) {
    for (;*l != static_cast<C>('\0'); l++, r++) {
        if (*l != *r) {
            return *l < *r ? -1 : 1;
        }
    }
    return *r == static_cast<C>('\0') ? 0 : -1;
}

struct CharChooser {
    char a_char;
    wchar_t a_wchar;
    char16_t a_char16;
    char32_t a_char32;

    constexpr CharChooser(
        char a_char,
        wchar_t a_wchar,
        char16_t a_char16,
        char32_t a_char32
    ) : a_char{a_char},
        a_wchar{a_wchar},
        a_char16{a_char16},
        a_char32{a_char32}
    {
    }

    template <typename Ch>
    constexpr operator Ch() const noexcept;
};

template <>
constexpr CharChooser::operator char() const noexcept {
    return a_char;
}

template <>
constexpr CharChooser::operator wchar_t() const noexcept {
    return a_wchar;
}

template <>
constexpr CharChooser::operator char16_t() const noexcept {
    return a_char16;
}

template <>
constexpr CharChooser::operator char32_t() const noexcept {
    return a_char32;
}

struct LiteralChooser {
    char const* char_literal;
    wchar_t const* wchar_literal;
    char16_t const* char16_literal;
    char32_t const* char32_literal;

    constexpr LiteralChooser(
        char const* char_literal,
        wchar_t const* wchar_literal,
        char16_t const* char16_literal,
        char32_t const* char32_literal
    ) : char_literal(char_literal),
        wchar_literal(wchar_literal),
        char16_literal(char16_literal),
        char32_literal(char32_literal)
    {
    }
    template <typename Ch>
    constexpr operator Ch const *() const noexcept;
};

template <>
constexpr LiteralChooser::operator char const*() const noexcept {
    return char_literal;
}

template <>
constexpr LiteralChooser::operator wchar_t const*() const noexcept {
    return wchar_literal;
}

template <>
constexpr LiteralChooser::operator char16_t const*() const noexcept {
    return char16_literal;
}

template <>
constexpr LiteralChooser::operator char32_t const*() const noexcept {
    return char32_literal;
}

#define CNV_CHTYPE(CH, PREFIX) (PREFIX ## CH)
#define CONVERT_C(CH) \
    CharChooser{ CH, CNV_CHTYPE(CH,L), CNV_CHTYPE(CH,u), CNV_CHTYPE(CH,U) }
#define CAST_C(TYPE, CH) \
    static_cast<TYPE>(static_cast<std::make_unsigned_t<decltype(CH)>>(CH))
#define CONVERT_STR(CH) \
    LiteralChooser{ CH, CNV_CHTYPE(CH,L), CNV_CHTYPE(CH,u), CNV_CHTYPE(CH,U) }

template <typename C>
constexpr C cast_c(char c) {
    return static_cast<C>(static_cast<std::make_unsigned_t<char>>(c));
}
