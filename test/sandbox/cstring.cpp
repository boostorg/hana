/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/constant/mcd.hpp>

#include <test/laws/constant.hpp>
using namespace boost::hana;


struct String { };

#define LIT(s)                                                              \
    ([] {                                                                   \
        struct tmp {                                                        \
            static constexpr decltype(s) get() { return s; }                \
            using hana_datatype = String;                                   \
        };                                                                  \
        return  tmp{};                                                      \
    }())                                                                    \
/**/

namespace boost { namespace hana {
    template <>
    struct Constant::instance<String> : Constant::mcd {
        template <typename S>
        static constexpr auto value_impl(S const&) {
            return S::get();
        }
    };
}}


int main() {
    Constant_laws(
        LIT("foo"),
        LIT("bar")
    );

    auto dyn_foo = LIT("foo");
    constexpr char const* c_foo = value(dyn_foo);
    (void)c_foo;
}
