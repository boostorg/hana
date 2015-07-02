/*!
@file
Defines operators for Monads.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_OPERATORS_MONAD_HPP
#define BOOST_HANA_DETAIL_OPERATORS_MONAD_HPP

#include <boost/hana/core/datatype.hpp>
#include <boost/hana/fwd/concept/monad.hpp>

#include <type_traits>


namespace boost { namespace hana {
    namespace detail {
        template <typename Tag>
        struct monad_operators {
            static constexpr bool value = false;
        };
    }

    namespace operators {
        template <typename Xs, typename F, typename = typename std::enable_if<
            detail::monad_operators<typename datatype<Xs>::type>::value
        >::type>
        constexpr auto operator|(Xs&& xs, F&& f)
        { return hana::chain(static_cast<Xs&&>(xs), static_cast<F&&>(f)); }
    }
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_OPERATORS_MONAD_HPP
