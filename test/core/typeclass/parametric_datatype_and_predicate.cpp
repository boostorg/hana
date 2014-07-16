/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core.hpp>
namespace hana = boost::hana;


struct Typeclass {
    BOOST_HANA_TYPECLASS(Typeclass);
};

template <typename T>
struct predicate {
    static constexpr bool value = false;
};

template <typename T>
struct Datatype { };

template <typename T>
struct predicate<Datatype<T>> {
    static constexpr bool value = true;
};

template <typename T>
struct Typeclass::instance<Datatype<T>, hana::when<predicate<Datatype<T>>::value>> {
    struct dispatch_is_correct;
};

template <typename T>
struct Typeclass::instance<T, hana::when<predicate<T>::value>> {

};

using test = Typeclass::instance<Datatype<int>>::dispatch_is_correct;

int main() { }
