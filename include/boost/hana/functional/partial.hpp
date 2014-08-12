/*!
@file
Defines `boost::hana::partial`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_PARTIAL_HPP
#define BOOST_HANA_FUNCTIONAL_PARTIAL_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Partially apply a function to some arguments.
    //!
    //! Specifically, `partial(f, x...)` is a function such that
    //! @code
    //!     partial(f, x...)(y...) == f(x..., y...)
    //! @endcode
    //!
    //! Given the semantics, the arity of `f` must match the number of
    //! arguments passed in total, i.e. `sizeof...(x) + sizeof...(y)`.
    //!
    //! ### Example
    //! @snippet example/functional/partial.cpp main
    BOOST_HANA_CONSTEXPR_LAMBDA auto partial = [](auto f, auto ...x) {
        return [=](auto ...y) {
            return f(x..., y...);
        };
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_PARTIAL_HPP
