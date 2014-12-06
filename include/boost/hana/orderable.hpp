/*!
@file
Defines `boost::hana::Orderable`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ORDERABLE_HPP
#define BOOST_HANA_ORDERABLE_HPP

#include <boost/hana/fwd/orderable.hpp>

#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/std/declval.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    namespace operators {
        //! Equivalent to `less`.
        //! @relates boost::hana::Orderable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Orderable, datatype_t<X>>::value ||
            enable_operators<Orderable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator<(X&& x, Y&& y) {
            return less(
                detail::std::forward<decltype(x)>(x),
                detail::std::forward<decltype(y)>(y)
            );
        }

        //! Equivalent to `less_equal`.
        //! @relates boost::hana::Orderable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Orderable, datatype_t<X>>::value ||
            enable_operators<Orderable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator<=(X&& x, Y&& y) {
            return less_equal(
                detail::std::forward<decltype(x)>(x),
                detail::std::forward<decltype(y)>(y)
            );
        }

        //! Equivalent to `greater`.
        //! @relates boost::hana::Orderable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Orderable, datatype_t<X>>::value ||
            enable_operators<Orderable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator>(X&& x, Y&& y) {
            return greater(
                detail::std::forward<decltype(x)>(x),
                detail::std::forward<decltype(y)>(y)
            );
        }

        //! Equivalent to `greater_equal`.
        //! @relates boost::hana::Orderable
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            enable_operators<Orderable, datatype_t<X>>::value ||
            enable_operators<Orderable, datatype_t<Y>>::value
        >>
        constexpr decltype(auto) operator>=(X&& x, Y&& y) {
            return greater_equal(
                detail::std::forward<decltype(x)>(x),
                detail::std::forward<decltype(y)>(y)
            );
        }
    }

    //! Minimal complete definition: `less`
    struct Orderable::less_mcd {
        template <typename X, typename Y>
        static constexpr decltype(auto) less_equal_impl(X&& x, Y&& y) {
            return not_(less(
                detail::std::forward<Y>(y),
                detail::std::forward<X>(x)
            ));
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) greater_impl(X&& x, Y&& y) {
            return less(
                detail::std::forward<Y>(y),
                detail::std::forward<X>(x)
            );
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) greater_equal_impl(X x, Y y) {
            return not_(less(
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            ));
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) min_impl(X&& x, Y&& y) {
            decltype(auto) cond = less(x, y);
            return if_(detail::std::forward<decltype(cond)>(cond),
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }

        template <typename X, typename Y>
        static constexpr decltype(auto) max_impl(X&& x, Y&& y) {
            decltype(auto) cond = less(x, y);
            return if_(detail::std::forward<decltype(cond)>(cond),
                detail::std::forward<Y>(y),
                detail::std::forward<X>(x)
            );
        }
    };

    template <typename T, typename U>
    struct Orderable::default_instance
        : Orderable::instance<common_t<T, U>, common_t<T, U>>
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) less_impl(X&& x, Y&& y) {
            using C = common_t<T, U>;
            return less(
                to<C>(detail::std::forward<X>(x)),
                to<C>(detail::std::forward<Y>(y))
            );
        }
    };

    //! Instance of `Orderable` for foreign but `LessThanComparable` objects.
    //!
    //! Any two foreign objects whose types can be compared using `operator<`
    //! are automatically instances of `Orderable` by using that comparison.
    template <typename T, typename U>
    struct Orderable::instance<T, U, when_valid<
        decltype(detail::std::declval<T>() < detail::std::declval<U>())
    >>
        : Orderable::less_mcd
    {
        template <typename X, typename Y>
        static constexpr decltype(auto) less_impl(X&& x, Y&& y) {
            return detail::std::forward<X>(x) < detail::std::forward<Y>(y);
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ORDERABLE_HPP
