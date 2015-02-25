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

#include <boost/hana/bool.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/core/wrong.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/detail/less_than_comparable.hpp>
#include <boost/hana/detail/std/enable_if.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/detail/std/integral_constant.hpp>
#include <boost/hana/logical.hpp>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // Operators
    //////////////////////////////////////////////////////////////////////////
    namespace operators {
        template <typename X, typename Y, typename = detail::std::enable_if_t<
            has_operator<datatype_t<X>, decltype(less)>::value ||
            has_operator<datatype_t<Y>, decltype(less)>::value
        >>
        constexpr decltype(auto) operator<(X&& x, Y&& y) {
            return hana::less(detail::std::forward<X>(x),
                              detail::std::forward<Y>(y));
        }

        template <typename X, typename Y, typename = detail::std::enable_if_t<
            has_operator<datatype_t<X>, decltype(less_equal)>::value ||
            has_operator<datatype_t<Y>, decltype(less_equal)>::value
        >>
        constexpr decltype(auto) operator<=(X&& x, Y&& y) {
            return hana::less_equal(detail::std::forward<X>(x),
                                    detail::std::forward<Y>(y));
        }

        template <typename X, typename Y, typename = detail::std::enable_if_t<
            has_operator<datatype_t<X>, decltype(greater)>::value ||
            has_operator<datatype_t<Y>, decltype(greater)>::value
        >>
        constexpr decltype(auto) operator>(X&& x, Y&& y) {
            return hana::greater(detail::std::forward<X>(x),
                                 detail::std::forward<Y>(y));
        }

        template <typename X, typename Y, typename = detail::std::enable_if_t<
            has_operator<datatype_t<X>, decltype(greater_equal)>::value ||
            has_operator<datatype_t<Y>, decltype(greater_equal)>::value
        >>
        constexpr decltype(auto) operator>=(X&& x, Y&& y) {
            return hana::greater_equal(detail::std::forward<X>(x),
                                       detail::std::forward<Y>(y));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // less
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct less_impl : less_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct less_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr void apply(X&&, Y&&) {
            static_assert(wrong<less_impl<T, U>, X, Y>{},
            "no definition of boost::hana::less for the given data types");
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct less_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Orderable, T, U>{} &&
        !detail::concept::LessThanComparable<T, U>{}
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::less(to<C>(detail::std::forward<X>(x)),
                              to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // less_equal
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct less_equal_impl : less_equal_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct less_equal_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::not_(hana::less(detail::std::forward<Y>(y),
                                         detail::std::forward<X>(x)));
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct less_equal_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Orderable, T, U>{}
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::less_equal(to<C>(detail::std::forward<X>(x)),
                                    to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // greater
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct greater_impl : greater_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct greater_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::less(detail::std::forward<Y>(y),
                              detail::std::forward<X>(x));
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct greater_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Orderable, T, U>{}
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::greater(to<C>(detail::std::forward<X>(x)),
                                 to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // greater_equal
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct greater_equal_impl : greater_equal_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct greater_equal_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X x, Y y) {
            return hana::not_(hana::less(detail::std::forward<X>(x),
                                         detail::std::forward<Y>(y)));
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct greater_equal_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Orderable, T, U>{}
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::greater_equal(to<C>(detail::std::forward<X>(x)),
                                       to<C>(detail::std::forward<Y>(y)));
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // min
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct min_impl : min_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct min_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            decltype(auto) cond = hana::less(x, y);
            return hana::if_(detail::std::forward<decltype(cond)>(cond),
                detail::std::forward<X>(x),
                detail::std::forward<Y>(y)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // max
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct max_impl : max_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct max_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            decltype(auto) cond = hana::less(x, y);
            return hana::if_(detail::std::forward<decltype(cond)>(cond),
                detail::std::forward<Y>(y),
                detail::std::forward<X>(x)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename Ord>
    struct models<Orderable(Ord)>
        : detail::std::integral_constant<bool,
            !is_default<less_impl<Ord, Ord>>{}
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for LessThanComparable data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct less_impl<T, U, when<detail::concept::LessThanComparable<T, U>{}>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return detail::std::forward<X>(x) < detail::std::forward<Y>(y); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants wrapping an Orderable
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct less_impl<C, C, when<
        models<Constant(C)>{} && models<Orderable(typename C::value_type)>{}
    >> {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y) {
            constexpr auto less = hana::less(hana::value(x), hana::value(y));
            constexpr bool truth_value = hana::if_(less, true, false);
            return bool_<truth_value>;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Orderable::equal_impl
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct Orderable::equal_impl {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X const& x, Y const& y) {
            return hana::and_(hana::not_(hana::less(x, y)),
                              hana::not_(hana::less(y, x)));
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_ORDERABLE_HPP
