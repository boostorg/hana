/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>

#include <cxxabi.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>
namespace hana = boost::hana;


// Boilerplate for the `print` method
template <typename T, typename = void>
struct print_impl : print_impl<T, hana::when<true>> { };

template <typename T, bool condition>
struct print_impl<T, hana::when<condition>> : hana::default_ {
    template <typename ...Args>
    static constexpr auto apply(Args&& ...) = delete;
};

template <typename T>
constexpr auto print(T const& t) {
    using Print = print_impl<typename hana::tag_of<T>::type>;
    return Print::apply(t);
}

// Define the `Printable` concept
template <typename T>
struct Printable {
    using Tag = typename hana::tag_of<T>::type;
    static constexpr bool value = !hana::is_default<print_impl<Tag>>::value;
};


// model for Sequences
template <typename S>
struct print_impl<S, hana::when<hana::Sequence<S>::value>> {
    template <typename Xs>
    static auto apply(Xs const& xs) {
        std::string result = "(";
        auto comma_separated = hana::intersperse(xs, ", ");
        hana::for_each(comma_separated, [&result](auto const& x) {
            result += print(x);
        });
        result += ")";
        return result;
    }
};

template <typename ...>
struct is_valid {
    static constexpr bool value = true;
};

// model for OutputStreamable types
template <typename S>
struct print_impl<S, hana::when<is_valid<decltype(
    std::declval<std::ostringstream&>() << std::declval<S const&>()
)>::value>> {
    template <typename T>
    static auto apply(T const& t) {
        std::ostringstream os;
        os << t;
        return os.str();
    }
};

// model for hana::maps
template <>
struct print_impl<hana::map_tag> {
    template <typename M>
    static auto apply(M const& map) {
        std::string result = "{";
        auto pairs = hana::transform(hana::to<hana::tuple_tag>(map),
            [](auto const& pair) {
                return print(hana::first(pair)) + " => " + print(hana::second(pair));
            });
        auto comma_separated = hana::intersperse(pairs, ", ");
        hana::for_each(comma_separated, [&result](auto const& element) {
            result += element;
        });
        result += "}";
        return result;
    }
};

// model for Constants holding a `Printable`
template <typename C>
struct print_impl<C, hana::when<
    hana::Constant<C>::value &&
    Printable<typename C::value_type>::value
>> {
    template <typename T>
    static auto apply(T const&) {
        constexpr auto value = hana::value<T>();
        return print(value);
    }
};

// model for Products
template <typename P>
struct print_impl<P, hana::when<hana::Product<P>::value>> {
    template <typename T>
    static auto apply(T const& t) {
        return '(' + print(hana::first(t)) + ", " + print(hana::second(t)) + ')';
    }
};

// model for hana::strings
template <>
struct print_impl<hana::string_tag> {
    template <typename S>
    static auto apply(S const& s) {
        return '"' + std::string{hana::to<char const*>(s)} + '"';
    }
};

// model for hana::sets
template <>
struct print_impl<hana::set_tag> {
    template <typename S>
    static auto apply(S const& set) {
        std::string result = "{";
        auto as_tuple = hana::transform(hana::to<hana::tuple_tag>(set), [](auto const& x) { return print(x); });
        auto comma_separated = hana::intersperse(as_tuple, ", ");
        hana::for_each(comma_separated, [&result](auto const& element) {
            result += element;
        });
        result += "}";
        return result;
    }
};

// model for hana::types
template <>
struct print_impl<hana::type_tag> {
    static std::string demangle(char const* mangled) {
        int status;
        std::unique_ptr<char[], void(*)(void*)> result(
            abi::__cxa_demangle(mangled, 0, 0, &status), std::free
        );
        return result.get() ? result.get() : mangled;
    }

    template <typename T>
    static auto apply(T const&) {
        return "type<" + demangle(typeid(typename T::type).name()) + '>';
    }
};


int main() {
    auto tuple = hana::make_tuple(1, '2', BOOST_HANA_STRING("3456"));

    auto set = hana::make_set(hana::int_c<1>, hana::int_c<2>);

    auto map = hana::make_map(
        hana::make_pair(hana::int_c<1>, hana::type_c<void>),
        hana::make_pair(hana::type_c<decltype(tuple)>, "int")
    );

    auto pair = hana::make_pair(1, hana::int_c<1>);

    std::cout << print(tuple) << std::endl
              << print(set) << std::endl
              << print(map) << std::endl
              << print(pair) << std::endl;
}
