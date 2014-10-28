/*!
@file
Defines the @ref group-functional module.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FUNCTIONAL_HPP
#define BOOST_HANA_FUNCTIONAL_HPP

//! @defgroup group-functional Functional
//! General purpose function objects.
//!
//! In the documentation of these utilities, a simplified implementation is
//! often provided in pseudo-code using lambdas. The reason is that the actual
//! implementation is often contrived because of limitations with lambdas in
//! the current language. Here is an explanation of some terms that appear in
//! the pseudo-code:
//!
//! ### `forwarded(x)`
//! Means that the object is forwarded optimally. This means that if `x` is a
//! parameter, it is `std::forward`ed, and if it is a captured variable, it is
//! moved from whenever the lambda is an rvalue.
//!
//! Also note that when `x` can be moved from, the statement
//! `return forwarded(x);` in a function with `decltype(auto)` does not mean
//! that an rvalue reference to `x` will be returned, which would create a
//! dangling reference. Rather, it means that `x` is returned by value, the
//! value being constructed with the `std::forward`ed `x`.
//!
//! ### `perfect-capture`
//! Means that the captured variables are initialized using perfect
//! forwarding, as if `[x(forwarded(x))...]() { }` had been used.

#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/apply.hpp>
#include <boost/hana/functional/arg.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/demux.hpp>
#include <boost/hana/functional/fix.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/functional/lockstep.hpp>
#include <boost/hana/functional/on.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functional/placeholder.hpp>

#endif // !BOOST_HANA_FUNCTIONAL_HPP
