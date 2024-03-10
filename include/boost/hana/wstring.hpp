/*!
@file
Defines `boost::hana::wstring`.

@copyright Louis Dionne 2013-2018
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_WSTRING_HPP
#define BOOST_HANA_WSTRING_HPP

#include <boost/hana/basic_string.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>

BOOST_HANA_NAMESPACE_BEGIN

template <wchar_t ...s>
using wstring = basic_string<wchar_t, s...>;

using wstring_tag = basic_string_tag<wchar_t>;

constexpr auto make_wstring = make<wstring_tag>;

constexpr auto to_wstring = to<wstring_tag>;

template <wchar_t ...s>
constexpr wstring<s...> wstring_c{};

BOOST_HANA_NAMESPACE_END

#endif // !BOOST_HANA_WSTRING_HPP
