/*!
@file
Defines `boost::hana::sandbox::lazy`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_SANDBOX_LAZY_HPP
#define BOOST_HANA_DETAIL_SANDBOX_LAZY_HPP

#include <boost/hana/core.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/functional.hpp>

#include <type_traits>


namespace boost { namespace hana {
namespace sandbox {
    namespace lazy_detail {
        struct delayed;

        template <typename R, typename T>
        struct value {
            R r;

            template <typename ...>
            using underlying_type = T;

            constexpr operator T() const
            { return r(id); }
        };

        template <typename R>
        struct value<R, delayed> {
            R r;

            template <typename ...Dummy>
            using underlying_type = decltype(r(always(id)(((Dummy*)0)...)));

            // We provide a conversion only when a conversion is available
            // for our underlying type. We delay the instantiation of the
            // return type to the very end.
            template <typename T, typename = std::enable_if_t<
                std::is_convertible<underlying_type<T>, T>{}
            >>
            constexpr operator T() const
            { return r(id); }
        };
    } // end namespace lazy_detail

    //! @todo
    //! - Is there more to forward than just the data type?
    //! - Consider making the conversions explicit, or at least giving the option.
    //! - We currently support two policies to instantiate the return type;
    //!   it is either given explicitly or we wait until the very end to
    //!   instantiate it. We could also compute it ourselves, but eagerly,
    //!   i.e. `lazy<eager_policy>(invalid)(args...)` would fail even if we
    //!   don't attempt any conversion.
    template <typename T = lazy_detail::delayed>
    BOOST_HANA_CONSTEXPR_LAMBDA auto lazy = [](auto f) {
        return [=](auto ...x) {
            auto r = [=](auto _) { return _(f)(x...); };
            return lazy_detail::value<decltype(r), T>{r};
        };
    };
} // end namespace sandbox

template <typename R, typename T>
struct datatype<sandbox::lazy_detail::value<R, T>> {
    using type = typename datatype<
        typename sandbox::lazy_detail::value<R, T>::template underlying_type<>
    >::type;
};
}} // end namespace boost::hana

namespace std {
    template <typename R1, typename T1, typename R2, typename T2>
    struct common_type<
        ::boost::hana::sandbox::lazy_detail::value<R1, T1>,
        ::boost::hana::sandbox::lazy_detail::value<R2, T2>
    > {
        using V1 = ::boost::hana::sandbox::lazy_detail::value<R1, T1>;
        using V2 = ::boost::hana::sandbox::lazy_detail::value<R2, T2>;
        using type = typename common_type<
            typename V1::template underlying_type<>,
            typename V2::template underlying_type<>
        >::type;
    };
}

#endif // !BOOST_HANA_DETAIL_SANDBOX_LAZY_HPP
