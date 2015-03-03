/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/wrap.hpp>
#include <boost/hana/tuple.hpp>
namespace hana = boost::hana;


//////////////////////////////////////////////////////////////////////////////
// core
//////////////////////////////////////////////////////////////////////////////
template <typename ...>
struct not_implemented;


//////////////////////////////////////////////////////////////////////////////
// Functor
//////////////////////////////////////////////////////////////////////////////
template <typename X, typename F, typename Enable = void>
not_implemented<X, F> fmap_impl{};

auto fmap = [](auto x, auto f) {
    return fmap_impl<
        hana::datatype_t<decltype(x)>,
        hana::datatype_t<decltype(f)>
    >(x, f);
};


//////////////////////////////////////////////////////////////////////////////
// Applicative
//////////////////////////////////////////////////////////////////////////////
template <typename F, typename X, typename Enable = void>
not_implemented<F, X> ap_impl{};

auto ap = [](auto f, auto x) {
    return ap_impl<
        hana::datatype_t<decltype(f)>,
        hana::datatype_t<decltype(x)>
    >(f, x);
};

template <typename A, typename Enable = void>
not_implemented<A> lift_impl{};

template <template <typename> class A>
auto lift = [](auto x) {
    return lift_impl<A<hana::datatype_t<decltype(x)>>>(x);
};


//////////////////////////////////////////////////////////////////////////////
// Function
//////////////////////////////////////////////////////////////////////////////
template <typename F, typename X, typename Enable = void>
not_implemented<F, X> apply_impl{};

auto apply = [](auto f, auto x) {
    return apply_impl<
        hana::datatype_t<decltype(f)>,
        hana::datatype_t<decltype(x)>
    >(f, x);
};

template <typename Domain, typename Codomain>
struct Function;

template <typename Domain, typename Codomain>
auto function = [](auto f) {
    return hana::detail::wrap<Function<Domain, Codomain>>(f);
};

template <typename X, typename Y>
auto apply_impl<Function<X, Y>, X> = [](auto f, auto x) {
    return hana::detail::unwrap(f)(x);
};


//////////////////////////////////////////////////////////////////////////////
// List
//////////////////////////////////////////////////////////////////////////////
template <typename T>
struct List;

template <typename T>
auto list = [](auto ...xs) {
    return hana::detail::wrap<List<T>>(
        [=](auto f) { return f(xs...); }
    );
};

template <typename X, typename Y>
auto fmap_impl<List<X>, Function<X, Y>> = [](auto xs, auto f) {
    return hana::detail::unwrap(xs)([=](auto ...xs) {
        return list<Y>(apply(f, xs)...);
    });
};

template <typename X>
auto lift_impl<List<X>> = [](auto x) {
    return list<X>(x);
};

template <typename X, typename Y>
auto ap_impl<List<Function<X, Y>>, List<X>> = [](auto fs, auto xs) {
    auto hana_fs = hana::detail::unwrap(fs)([](auto ...fs) {
        return hana::make<hana::Tuple>([fs](auto x) { return apply(fs, x); }...);
    });
    auto hana_xs = hana::detail::unwrap(xs)(hana::make<hana::Tuple>);
    auto hana_result = hana::ap(hana_fs, hana_xs);

    return hana::unpack(hana_result, list<Y>);
};


//////////////////////////////////////////////////////////////////////////////
// Any
//////////////////////////////////////////////////////////////////////////////
struct Any;

auto any = [](auto x) {
    return hana::detail::wrap<Any>(x);
};


int main() {
    auto f = function<int, int>([](auto x) { return x + 1; });
    auto xs = list<int>(1, 2, 3, 4);
    fmap(xs, f);

    lift<List>(2);
    ap(list<Function<int, int>>(f, f), list<int>(1, 2));

    auto g = function<Any, int>([](auto x) {
        // We can't do anything with an Any, so there's not much choice here.
        return 1;
    });
    fmap(list<Any>(any(1), any('2'), any("345")), g);
}
