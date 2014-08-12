/*!
@file
Defines custom operators for `boost::hana::Monad`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_MONAD_OPERATORS_HPP
#define BOOST_HANA_MONAD_OPERATORS_HPP

#include <boost/hana/monad/monad.hpp>

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Equivalent to `bind`.
        //! @relates boost::hana::Monad
        //!
        //! @note
        //! This was preferred over `>>=` because `>>=` is right associative
        //! in C++, which makes it impossible to chain computations.
        template <typename M, typename F, typename = detail::std::enable_if_t<
            enable_operators<Monad, datatype_t<M>>::value
        >>
        constexpr decltype(auto) operator|(M&& m, F&& f) {
            return bind(
                detail::std::forward<decltype(m)>(m),
                detail::std::forward<decltype(f)>(f)
            );
        }
    }

    using operators::operator|;
}} // end namespace boost::hana

#endif // !BOOST_HANA_MONAD_OPERATORS_HPP
