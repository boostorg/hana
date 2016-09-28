// Copyright Louis Dionne 2013-2016
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HANA_TEST_VIEW_AUTO_SPECS_HPP
#define BOOST_HANA_TEST_VIEW_AUTO_SPECS_HPP

#include <boost/hana/tuple.hpp>
#include <boost/hana/view.hpp>

#include <memory>
#include <vector>


// If we want to use the automatic testing, we need to allocate the containers
// over which we create views, otherwise the lifetime won't work.
template <typename ...T>
auto MAKE_TUPLE(T&& ...t) {
    using Tuple = boost::hana::tuple<typename std::decay<T>::type...>;
    static std::vector<std::unique_ptr<Tuple>> pool;
    pool.push_back(std::make_unique<Tuple>(std::forward<T>(t)...));
    return boost::hana::make_view(*pool.back().get());
}

#define MAKE_TUPLE(...) ::boost::hana::make_tuple(__VA_ARGS__)
#define TUPLE_TYPE(...) ::boost::hana::view<::boost::hana::tuple<__VA_ARGS__>>
#define TUPLE_TAG ::boost::hana::view_tag

#endif // !BOOST_HANA_TEST_VIEW_AUTO_SPECS_HPP
