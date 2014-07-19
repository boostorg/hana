/*!
@file
Internal header to break cyclic dependencies.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_MAYBE_FWD_HPP
#define BOOST_HANA_DETAIL_MAYBE_FWD_HPP

#include <boost/hana/bool.hpp>


namespace boost { namespace hana {
    /*!
    @ingroup datatypes
    Represents an optional value.

    A `Maybe` either contains a value (represented as `just(x)`), or it is
    empty (represented as `nothing`).

    --------------------------------------------------------------------------

    ## Instance of
    `Comparable`, `Functor`, `Applicative`, `Monad` and `Foldable`
     */
    struct Maybe { };

    namespace maybe_detail {
        template <bool is_valid, typename T, typename = operators::enable>
        struct maybe {
            using hana_datatype = Maybe;

            template <typename Default, typename F>
            constexpr auto maybe_impl(Default d, F) const
            { return d; }
        };

        template <typename T>
        struct maybe<true, T> {
            using hana_datatype = Maybe;
            T val;

            template <typename Default, typename F>
            constexpr auto maybe_impl(Default, F f) const
            { return f(val); }
        };

        template <typename T>
        using just = maybe<true, T>;
        using nothing = maybe<false, void>;
    }

    //! Creates an optional value containing `x`.
    //! @relates Maybe
    //! @hideinitializer
    BOOST_HANA_CONSTEXPR_LAMBDA auto just = [](auto x) {
        return maybe_detail::just<decltype(x)>{x};
    };

    //! Creates an empty optional value.
    //! @relates Maybe
    //! @hideinitializer
    constexpr maybe_detail::nothing nothing{};

    //! Applies a function to the contents of a `Maybe`, with a fallback
    //! result.
    //! @relates Maybe
    //!
    //! Specifically, `maybe` takes a default value, a function and an
    //! optional value. If the optional value is `nothing`, the default
    //! value is returned. Otherwise, the function is applied to the
    //! content of the `just`.
    //!
    //! ### Example
    //! @snippet example/maybe/api.cpp maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto maybe = [](auto default_, auto f, auto m) {
        return m.maybe_impl(default_, f);
    };

    //! Returns whether a `Maybe` contains a value.
    //! @relates Maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_just = [](auto m) {
        return maybe(false_, [](auto) { return true_; }, m);
    };

    //! Returns whether a `Maybe` is empty.
    //! @relates Maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto is_nothing = [](auto m) {
        return maybe(true_, [](auto) { return false_; }, m);
    };

    //! Returns the contents of a `Maybe`, or a default value if the `Maybe`
    //! is `nothing`.
    //! @relates Maybe
    //!
    //! ### Example
    //! @snippet example/maybe/api.cpp from_maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto from_maybe = [](auto default_, auto m) {
        return maybe(default_, [](auto x) { return x; }, m);
    };

    //! Extract the value out of a `just`, or trigger a compile-time error
    //! if the argument is `nothing`.
    //! @relates Maybe
    BOOST_HANA_CONSTEXPR_LAMBDA auto from_just = [](auto m) {
        auto err = [](auto ...dum) {
            constexpr bool always_false = sizeof...(dum) != 0;
            static_assert(always_false,
            "trying to extract the value inside a boost::hana::nothing "
            "with boost::hana::from_just");
        };
        return maybe(err, [](auto x) { return [=] { return x; }; }, m)();
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_DETAIL_MAYBE_FWD_HPP
