/*!
@file
Defines `boost::hana::insert` for `boost::hana::Set`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_INSERT_HPP
#define BOOST_HANA_SET_INSERT_HPP

#include <boost/hana/detail/insert_fwd.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/foldable/foldable.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/list/list.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/set/searchable.hpp>
#include <boost/hana/set/set.hpp>


namespace boost { namespace hana {
    template <>
    struct insert_impl<Set> {
        template <typename S, typename X>
        static constexpr decltype(auto) apply(S&& set, X&& x) {
            return eval_if(elem(set, x),
                [&set](auto) -> decltype(auto) {
                    return id(detail::std::forward<S>(set));
                },
                [&set, &x](auto _) -> decltype(auto) {
                    return unpack(
                        _(cons)(
                            detail::std::forward<X>(x),
                            detail::std::forward<S>(set).storage
                        ),
                        hana::set
                    );
                }
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_INSERT_HPP
