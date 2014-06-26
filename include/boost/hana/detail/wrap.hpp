/*!
@file
Defines `boost::hana::detail::wrap`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_WRAP_HPP
#define BOOST_HANA_DETAIL_WRAP_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana { namespace detail {
    namespace wrap_detail {
        template <typename Datatype, typename X>
        struct wrapper {
            X unwrap;
            using hana_datatype = Datatype;
        };

        template <typename Datatype>
        struct wrap_impl {
            template <typename X>
            constexpr auto operator()(X x) const {
                return wrapper<Datatype, X>{x};
            }
        };
    }

    template <typename Datatype>
    constexpr wrap_detail::wrap_impl<Datatype> wrap{};

    BOOST_HANA_CONSTEXPR_LAMBDA auto unwrap = [](auto x) {
        return x.unwrap;
    };
}}} // end namespace boost::hana::detail

#endif // !BOOST_HANA_DETAIL_WRAP_HPP
