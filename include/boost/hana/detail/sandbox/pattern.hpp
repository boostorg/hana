/*!
@file
Defines `boost::hana::Pattern`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_PATTERN_HPP
#define BOOST_HANA_PATTERN_HPP

#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/typeclasses.hpp>


namespace boost { namespace hana {
    //! @ingroup typeclasses
    //! ...
    //!
    //! Example usage
    //! @code
    //!     auto result = match(value)
    //!         (pattern1, action1)
    //!         (pattern2, action2)
    //!         (otherwise, action3);
    //! @endcode
    //!
    //! @code
    //!     auto result = match(list(1, 2, 3))
    //!         (_cons(_x, _xs), [](auto x, auto xs) { ... })
    //! @endcode
    //!
    //! @note
    //! A lazy action could be anything that can be called with `id`; this
    //! would allow us to create lazy lambdas on the fly and also to use
    //! `lazy()` when we want to.
    struct Pattern {
        BOOST_HANA_TYPECLASS(Pattern);
        struct mcd { };
    };

    //! ...
    //! @relates Pattern
    BOOST_HANA_CONSTEXPR_LAMBDA auto matches = [](auto value, auto pattern) {
        return Pattern::instance<datatype_t<decltype(pattern)>>::matches_impl(value, pattern);
    };

    BOOST_HANA_CONSTEXPR_LAMBDA auto match = fix(auto match, [](auto value) {
        return [=](auto pattern, auto action) {
            // If matches(...) is compile-time this is compile-time and the
            // action can return whatever type it wants.
            //
            // Otherwise, this is runtime and all the actions must agree on their
            // return type.
            //
            // Note that we would need some way of executing the whole statement,
            // since right now we only return functions.
            if (matches(value, pattern))
                return [=](...) { return action; };
            else
                return match(value);
        };
    });

    // note:
    // For two `Boolean`s `x` and `y`, `x` matches `y` iff they are logically
    // equivalent i.e. both true or both false.
    BOOST_HANA_CONSTEXPR_LAMBDA auto if_ = [](auto condition, auto then, auto else_) {
        return match(condition).with(true_, then).with(false_, else_);
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_PATTERN_HPP
