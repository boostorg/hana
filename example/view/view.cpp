// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#include <boost/hana/assert.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/core/is_a.hpp>
#include <boost/hana/remove_if.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/view.hpp>

#include <string>
namespace hana = boost::hana;
using namespace hana::literals;


struct Fish { std::string name; };
struct Cat  { std::string name; };
struct Dog  { std::string name; };

int main() {
    hana::tuple<Fish, Cat, Dog> animals{{"Nemo"}, {"Garfield"}, {"Snoopy"}};

    // Create a view of the tuple
    auto view = hana::make_view(animals);

    // Apply an algorithm to the view. No work is actually done here, this
    // just returns a new filtered view.
    auto mammals = hana::remove_if(view, [](auto const& a) {
        return hana::is_a<Fish>(a);
    });

    // The work is done here, when accessing the elements of the view.
    BOOST_HANA_RUNTIME_CHECK(hana::at_c<0>(mammals).name == "Garfield");
    BOOST_HANA_RUNTIME_CHECK(hana::at_c<1>(mammals).name == "Snoopy");
}
