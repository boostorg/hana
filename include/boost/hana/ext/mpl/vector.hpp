/*!
@file
Adapts `boost::mpl::vector`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_MPL_VECTOR_HPP
#define BOOST_HANA_EXT_MPL_VECTOR_HPP

#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/detail/enable_if.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/type.hpp>

#include <boost/mpl/equal.hpp>
#include <boost/mpl/sequence_tag.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <type_traits>


namespace boost { namespace hana {
    struct MplVector;

    template <typename T>
    struct datatype<T, detail::enable_if_t<
        std::is_same<
            typename boost::mpl::sequence_tag<T>::type,
            boost::mpl::aux::vector_tag
        >::value
    >> {
        using type = MplVector;
    };

    template <>
    struct Functor::instance<MplVector> : Functor::fmap_mcd {
        template <typename f, typename xs>
        static constexpr auto fmap_impl(f, xs) {
            return typename ::boost::mpl::transform<
                xs, detail::type_function<f>
            >::type{};
        }
    };

    template <>
    struct Comparable::instance<MplVector, MplVector> : Comparable::equal_mcd {
        template <typename Xs, typename Ys>
        static constexpr auto equal_impl(Xs, Ys) {
            return bool_<boost::mpl::equal<Xs, Ys>::type::value>;
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_EXT_MPL_VECTOR_HPP
