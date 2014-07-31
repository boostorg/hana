/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/typeclass.hpp>
#include <boost/hana/detail/constexpr.hpp>

#include <cassert>
#include <sstream>
#include <string>
namespace hana = boost::hana;


struct Showable {
    BOOST_HANA_TYPECLASS(Showable);
    struct mcd { };

    template <typename T>
    struct default_instance : mcd {
        template <typename X>
        static std::string show_impl(X)
        { return "<?>"; }
    };
};

BOOST_HANA_CONSTEXPR_LAMBDA auto show = [](auto x) {
    return Showable::instance<hana::datatype_t<decltype(x)>>::show_impl(x);
};

template <>
struct Showable::instance<int> : Showable::mcd {
    static auto show_impl(int i)
    { return (std::ostringstream{} << i).str(); }
};

struct unshowable { };

int main() {
    assert(show(1) == "1");
    assert(show(unshowable{}) == "<?>");
}
