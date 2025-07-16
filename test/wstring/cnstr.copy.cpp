// Copyright Louis Dionne 2013-2017
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/wstring.hpp>
namespace hana = boost::hana;


int main() {
    using Str = decltype(hana::wstring_c<L'a', L'b', L'c', L'd', L'\xabcd'>);
    Str s1{};
    Str s2(s1);
    Str s3 = s1;

    (void)s2; (void)s3;
}
