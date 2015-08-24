/*!
@file
Includes all the library components except the adapters for external
libraries.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_HPP
#define BOOST_HANA_HPP

// Concepts
#include <boost/hana/concept/applicative.hpp>
#include <boost/hana/concept/comonad.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/hana/concept/constant.hpp>
#include <boost/hana/concept/enumerable.hpp>
#include <boost/hana/concept/foldable.hpp>
#include <boost/hana/concept/functor.hpp>
#include <boost/hana/concept/group.hpp>
#include <boost/hana/concept/integral_domain.hpp>
#include <boost/hana/concept/iterable.hpp>
#include <boost/hana/concept/logical.hpp>
#include <boost/hana/concept/monad.hpp>
#include <boost/hana/concept/monad_plus.hpp>
#include <boost/hana/concept/monoid.hpp>
#include <boost/hana/concept/orderable.hpp>
#include <boost/hana/concept/product.hpp>
#include <boost/hana/concept/ring.hpp>
#include <boost/hana/concept/searchable.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/concept/struct.hpp>

// Data types
#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

// Misc
#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/functional.hpp>

//! @defgroup group-concepts Concepts
//! Concepts provided by the library.

//! @defgroup group-datatypes Data types
//! General purpose data types provided by the library.

//! @defgroup group-functional Functional
//! General purpose function objects.

//! @defgroup group-core Core
//! Core utilities of the library.

//! @defgroup group-ext External adapters
//! Adapters for external libraries.
//!
//! @warning
//! This section of the reference documentation is not finished yet. Please
//! refer to the section of the tutorial on [external adapters](@ref tutorial-ext)
//! for a summary of what's supported right now.

//! @defgroup group-config Configuration options
//! Configurable options to tweak the global behavior of the library.

//! @defgroup group-assertions Assertions
//! Macros to perform different kinds of assertions.

//! @defgroup group-details Details
//! Implementation details.

//! Namespace containing C++14 user-defined literals provided by Hana.
namespace literals { }

#endif // !BOOST_HANA_HPP
