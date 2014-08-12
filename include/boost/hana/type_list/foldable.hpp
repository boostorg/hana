/*!
@file
Defines the instance of `boost::hana::Foldable` for `boost::hana::TypeList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_LIST_FOLDABLE_HPP
#define BOOST_HANA_TYPE_LIST_FOLDABLE_HPP

#include <boost/hana/foldable/unpack_mcd.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list/type_list.hpp>


namespace boost { namespace hana {
    template <>
    struct Foldable::instance<TypeList> : Foldable::unpack_mcd {
        //! @todo Fix the lost optimization caused by unpacking with `Type`s.
        template <typename ...Xs, typename F>
        static constexpr auto unpack_impl(detail::repr::type_list<Xs...>, F f)
        { return f(type<Xs>...); }

        template <typename Xs, typename F>
        static constexpr auto unpack_impl(Xs, F f)
        { return unpack_impl(typename Xs::storage{}, f); }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_LIST_FOLDABLE_HPP
