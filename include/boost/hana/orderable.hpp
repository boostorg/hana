/*!
@file
Defines `boost::hana::Orderable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_ORDERABLE_HPP
#define BOOST_HANA_ORDERABLE_HPP

#include <boost/hana/fwd/orderable.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/core/common.hpp>
#include <boost/hana/core/convert.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/core/default.hpp>
#include <boost/hana/core/models.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/concepts.hpp>
#include <boost/hana/detail/has_common_embedding.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/logical.hpp>

#include <type_traits>


namespace boost { namespace hana {
    //////////////////////////////////////////////////////////////////////////
    // less
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct less_impl : less_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct less_impl<T, U, when<condition>> : default_ {
        static void apply(...);
    };

    // Cross-type overload
    template <typename T, typename U>
    struct less_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Orderable, T, U>{}() &&
        !detail::LessThanComparable<T, U>{}()
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::less(hana::to<C>(static_cast<X&&>(x)),
                              hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //! @cond
    template <typename X>
    constexpr decltype(auto) _less_than::operator()(X&& x) const
    { return hana::partial(hana::flip(less), static_cast<X&&>(x)); }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // less_equal
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct less_equal_impl : less_equal_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct less_equal_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::not_(hana::less(static_cast<Y&&>(y),
                                         static_cast<X&&>(x)));
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct less_equal_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Orderable, T, U>{}()
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::less_equal(hana::to<C>(static_cast<X&&>(x)),
                                    hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //! @cond
    template <typename X>
    constexpr decltype(auto) _less_equal_than::operator()(X&& x) const
    { return hana::partial(hana::flip(less_equal), static_cast<X&&>(x)); }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // greater
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct greater_impl : greater_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct greater_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::less(static_cast<Y&&>(y),
                              static_cast<X&&>(x));
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct greater_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Orderable, T, U>{}()
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::greater(hana::to<C>(static_cast<X&&>(x)),
                                 hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //! @cond
    template <typename X>
    constexpr decltype(auto) _greater_than::operator()(X&& x) const
    { return hana::partial(hana::flip(greater), static_cast<X&&>(x)); }
    //! @endcond

    //////////////////////////////////////////////////////////////////////////
    // greater_equal
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U, typename>
    struct greater_equal_impl : greater_equal_impl<T, U, when<true>> { };

    template <typename T, typename U, bool condition>
    struct greater_equal_impl<T, U, when<condition>> : default_ {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X x, Y y) {
            return hana::not_(hana::less(static_cast<X&&>(x),
                                         static_cast<Y&&>(y)));
        }
    };

    // Cross-type overload
    template <typename T, typename U>
    struct greater_equal_impl<T, U, when<
        detail::has_nontrivial_common_embedding<Orderable, T, U>{}()
    >> {
        using C = typename common<T, U>::type;
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return hana::greater_equal(hana::to<C>(static_cast<X&&>(x)),
                                       hana::to<C>(static_cast<Y&&>(y)));
        }
    };

    //! @cond
    template <typename X>
    constexpr decltype(auto) _greater_equal_than::operator()(X&& x) const
    { return hana::partial(hana::flip(greater_equal), static_cast<X&&>(x)); }
    //! @endcond

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
            return hana::if_(static_cast<decltype(cond)&&>(cond),
                static_cast<X&&>(x),
                static_cast<Y&&>(y)
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
            return hana::if_(static_cast<decltype(cond)&&>(cond),
                static_cast<Y&&>(y),
                static_cast<X&&>(x)
            );
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // ordering
    //////////////////////////////////////////////////////////////////////////
    template <typename F>
    struct _ordering {
        F f;

        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) const&
        { return hana::less(f(static_cast<X&&>(x)), f(static_cast<Y&&>(y))); }

#ifndef BOOST_HANA_CONFIG_CONSTEXPR_MEMBER_FUNCTION_IS_CONST
        template <typename X, typename Y>
        constexpr decltype(auto) operator()(X&& x, Y&& y) &
        { return hana::less(f(static_cast<X&&>(x)), f(static_cast<Y&&>(y))); }
#endif
    };

    //////////////////////////////////////////////////////////////////////////
    // models
    //////////////////////////////////////////////////////////////////////////
    template <typename Ord>
    struct models_impl<Orderable, Ord>
        : _integral_constant<bool,
            !is_default<less_impl<Ord, Ord>>{}()
        >
    { };

    //////////////////////////////////////////////////////////////////////////
    // Model for LessThanComparable data types
    //////////////////////////////////////////////////////////////////////////
    template <typename T, typename U>
    struct less_impl<T, U, when<detail::LessThanComparable<T, U>{}()>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return static_cast<X&&>(x) < static_cast<Y&&>(y); }
    };

    //////////////////////////////////////////////////////////////////////////
    // Model for Constants wrapping an Orderable
    //////////////////////////////////////////////////////////////////////////
    template <typename C>
    struct less_impl<C, C, when<
        _models<Constant, C>{}() &&
        _models<Orderable, typename C::value_type>{}()
    >> {
        template <typename X, typename Y>
        static constexpr auto apply(X const&, Y const&) {
            constexpr auto less = hana::less(hana::value<X>(), hana::value<Y>());
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
