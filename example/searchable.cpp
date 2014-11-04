/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#include <type_traits>
using namespace boost::hana;


int main() {
    {
        //! [all]
        using namespace literals;

        BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
            return x % 2_c != 0_c;
        };

        BOOST_HANA_CONSTEXPR_ASSERT(all(tuple(1, 3), odd));
        BOOST_HANA_CONSTANT_ASSERT(!all(tuple(3_c, 4_c), odd));

        BOOST_HANA_CONSTANT_ASSERT(
            !all(tuple(type<void>, type<char&>), trait<std::is_void>)
        );
        BOOST_HANA_CONSTANT_ASSERT(
            all(tuple(type<int>, type<char>), trait<std::is_integral>)
        );
        //! [all]
    }

    {
        //! [all_of]
        BOOST_HANA_CONSTEXPR_ASSERT(all_of(tuple(true_, true, true_)));
        BOOST_HANA_CONSTANT_ASSERT(!all_of(tuple(true, false_, true_)));
        //! [all_of]
    }

    {
        //! [any]
        using namespace literals;

        BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
            return x % 2_c != 0_c;
        };

        BOOST_HANA_CONSTEXPR_ASSERT(any(tuple(1, 2), odd));
        BOOST_HANA_CONSTANT_ASSERT(!any(tuple(2_c, 4_c), odd));

        BOOST_HANA_CONSTANT_ASSERT(
            any(tuple(type<void>, type<char&>), trait<std::is_void>)
        );
        BOOST_HANA_CONSTANT_ASSERT(
            !any(tuple(type<void>, type<char&>), trait<std::is_integral>)
        );
        //! [any]
    }

    {
        //! [any_of]
        BOOST_HANA_CONSTANT_ASSERT(any_of(tuple(false, false_, true_)));
        BOOST_HANA_CONSTEXPR_ASSERT(any_of(tuple(false, false_, true)));
        BOOST_HANA_CONSTEXPR_ASSERT(!any_of(tuple(false, false_, false_)));
        //! [any_of]
    }

    {
        //! [elem]
        BOOST_HANA_CONSTANT_ASSERT(elem(tuple(2, int_<2>, int_<3>, 'x'), int_<3>));
        BOOST_HANA_CONSTANT_ASSERT(elem(set(1, '2', type<int>, "foobar"), type<int>));
        //! [elem]
    }

    {
        //! [find]
        BOOST_HANA_CONSTEXPR_ASSERT(find(tuple(1.0, 2, '3'), trait_<std::is_integral>) == just(2));
        BOOST_HANA_CONSTANT_ASSERT(find(tuple(1.0, 2, '3'), trait_<std::is_class>) == nothing);

        constexpr auto types = type_list<char, int, unsigned, long, unsigned long>;
        BOOST_HANA_CONSTANT_ASSERT(find(types, _ == type<unsigned>) == just(type<unsigned>));
        BOOST_HANA_CONSTANT_ASSERT(find(types, _ == type<void>) == nothing);
        //! [find]
    }

    {
        //! [in]
        BOOST_HANA_CONSTEXPR_LAMBDA auto xs = tuple(
            int_<1>, type<int>, int_<2>, type<float>, int_<3>, type<void>, type<char>
        );
        BOOST_HANA_CONSTANT_ASSERT(
            filter(xs, in ^ tuple(int_<3>, type<int>, type<void>))
            ==
            tuple(type<int>, int_<3>, type<void>)
        );
        //! [in]
    }

    {
        //! [lookup]
        BOOST_HANA_CONSTANT_ASSERT(lookup(tuple(int_<1>, type<int>, '3'), type<int>) == just(type<int>));
        BOOST_HANA_CONSTANT_ASSERT(lookup(tuple(int_<1>, type<int>, '3'), type<void>) == nothing);

        BOOST_HANA_CONSTEXPR_LAMBDA auto m = map(
            pair(1, 'x'),
            pair(type<float>, 3.3),
            pair(type<char>, type<int>)
        );
        BOOST_HANA_CONSTEXPR_ASSERT(lookup(m, type<float>) == just(3.3));
        //! [lookup]
    }

    {
        //! [none]
        using namespace literals;

        BOOST_HANA_CONSTEXPR_LAMBDA auto odd = [](auto x) {
            return x % 2_c != 0_c;
        };

        BOOST_HANA_CONSTANT_ASSERT(none(tuple(2_c, 4_c), odd));
        BOOST_HANA_CONSTEXPR_ASSERT(!none(tuple(1, 2), odd));

        BOOST_HANA_CONSTANT_ASSERT(
            !none(tuple(type<void>, type<char&>), trait<std::is_void>)
        );
        BOOST_HANA_CONSTANT_ASSERT(
            none(tuple(type<void>, type<char&>), trait<std::is_integral>)
        );
        //! [none]
    }

    {
        //! [none_of]
        BOOST_HANA_CONSTEXPR_ASSERT(none_of(tuple(false, false_, false_)));
        BOOST_HANA_CONSTEXPR_ASSERT(!none_of(tuple(false, false_, true)));
        BOOST_HANA_CONSTANT_ASSERT(!none_of(tuple(false, false_, true_)));
        //! [none_of]
    }

    {
        //! [subset]
        BOOST_HANA_CONSTEXPR_ASSERT(subset(tuple(1, '2', 3.3), tuple(3.3, 1, '2', nullptr)));
        //! [subset]
    }
}
