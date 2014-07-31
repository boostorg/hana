/*!
@file
Adapts `boost::mpl::list`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_BOOST_MPL_LIST_HPP
#define BOOST_HANA_EXT_BOOST_MPL_LIST_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/std/type_traits.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/equal.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/sequence_tag.hpp>
#include <boost/mpl/transform.hpp>


namespace boost { namespace hana {
    struct MplList;

    template <typename T>
    struct datatype<T, when<
        detail::std::is_same<
            typename boost::mpl::sequence_tag<T>::type,
            boost::mpl::aux::list_tag
        >::value
    >> {
        using type = MplList;
    };

    template <>
    struct Functor::instance<MplList> : Functor::fmap_mcd {
        template <typename f, typename xs>
        static constexpr auto fmap_impl(f, xs) {
            return typename ::boost::mpl::transform<
                xs, detail::type_function<f>
            >::type{};
        }
    };

    template <>
    struct Comparable::instance<MplList, MplList> : Comparable::equal_mcd {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs, Ys) {
            return bool_<boost::mpl::equal<Xs, Ys>::type::value>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_BOOST_MPL_LIST_HPP
