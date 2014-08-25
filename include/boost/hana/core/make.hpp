/*!
@file
Defines `boost::hana::make`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_CORE_MAKE_HPP
#define BOOST_HANA_CORE_MAKE_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/forward.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //! Create an object of the given data type with the given arguments.
    //!
    //! @todo Document me.
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Datatype, typename = optional when-based enabler>
    constexpr auto make = [](auto&& ...x) -> decltype(auto) {
        return make_impl<Datatype>::apply(
            std::forward<decltype(x)>(x)...
        );
    };
#else
    namespace core_detail {
        template <typename Datatype>
        struct default_make {
            template <typename ...X>
            static constexpr auto apply_impl(int, X&& ...x)
                -> decltype(Datatype(detail::std::forward<X>(x)...))
            { return Datatype(detail::std::forward<X>(x)...); }

            template <typename ...X>
            static constexpr auto apply_impl(long, X&& ...) {
                static_assert((sizeof...(X), false),
                "there exists no constructor for the given data type");
            }

            template <typename ...X>
            constexpr decltype(auto) operator()(X&& ...x) const
            { return apply_impl(int{}, detail::std::forward<X>(x)...); }
        };
    }

    template <typename Datatype, typename = void>
    constexpr auto make = make<Datatype, when<true>>;

    template <typename Datatype, bool condition>
    constexpr core_detail::default_make<Datatype> make<Datatype, when<condition>>{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_CORE_MAKE_HPP
