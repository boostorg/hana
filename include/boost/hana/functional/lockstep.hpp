/*!
@file
Defines `boost::hana::lockstep`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_LOCKSTEP_HPP
#define BOOST_HANA_FUNCTIONAL_LOCKSTEP_HPP

#include <boost/hana/detail/constexpr.hpp>


namespace boost { namespace hana {
    //! @ingroup group-functional
    //! Invoke a function with the result of invoking other functions on its
    //! arguments, in lockstep.
    //!
    //! Specifically, `lockstep(f, g1, ..., gN)` is a function such that
    //! @code
    //!     lockstep(f, g1, ..., gN)(x1, ..., xN) == f(g1(x1), ..., gN(xN))
    //! @endcode
    //!
    //! Since each `g` is invoked on its corresponding argument in lockstep,
    //! the number of arguments must match the number of `g`s.
    //!
    //! ### Example
    //! @snippet example/functional/lockstep.cpp main
    //!
    //! @todo
    //! I think this is equivalent to `<*>` for `((->) r)`.
    BOOST_HANA_CONSTEXPR_LAMBDA auto lockstep = [](auto f, auto ...g) {
        return [f, g...](auto ...x) {
            return f(g(x)...);
        };
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FUNCTIONAL_LOCKSTEP_HPP
