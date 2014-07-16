/*!
@file
Adapts `std::list`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_LIST_HPP
#define BOOST_HANA_EXT_STD_LIST_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/iterable/foldable_instance.hpp>
#include <boost/hana/iterable/mcd.hpp>

#include <list>


namespace boost { namespace hana {
    //! @bug
    //! Right now, any method requiring `is_empty` will fail because its
    //! result is a runtime boolean and the methods depend on `is_empty`
    //! returning an `Integral`.
    struct StdList;

    template <typename T, typename Alloc>
    struct datatype<std::list<T, Alloc>> {
        using type = StdList;
    };

    template <>
    struct Iterable::instance<StdList> : Iterable::mcd {
        template <typename Xs>
        static auto head_impl(Xs xs)
        { return xs.front(); }

        template <typename Xs>
        static auto tail_impl(Xs xs)
        { return xs.pop_front(), xs; }

        template <typename Xs>
        static bool is_empty_impl(Xs xs)
        { return xs.empty(); }
    };

    template <>
    struct Functor::instance<StdList> : Functor::fmap_mcd {
        template <typename F, typename Xs>
        static constexpr auto fmap_impl(F f, Xs xs) {
            std::list<decltype(f(xs.front()))> ys{};
            for (auto x: xs)
                ys.push_back(f(x));
            return ys;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_STD_LIST_HPP
