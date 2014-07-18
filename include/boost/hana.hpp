/*!
@file
Includes all the library components, excluding the adaptors for external
libraries.

@todo
- When we have functions with several variants, consider naming versions
  with the following scheme:
  @code
    foldr.lazy
    foldr.strict
    foldr == foldr.strict
  @endcode
  That would be prettier and not much more complicated. However, we would
  need a naming convention for the `_impl` versions of those.
- To consider: is it possible/desirable to eliminate partial functions?
  For example, removing `head` and `tail`, which can fail, and replace
  them by a function which returns a `Maybe`.
- Document the library's stance on perfect forwarding and move semantics.
  Make compile-time tests with non-copyable types and runtime tests with
  expensive-to-copy types.
- In the unit tests, we might want to use an injective function on
  `Comparable`s instead of `std::make_tuple`.
- Write a tutorial. In particular:
    - Document how to emulate `make_fused` and friends from Boost.Fusion.
    - Document how to write common Boost.Fusion and Boost.MPL idioms with
      Boost.Hana.
- Write runtime benchmarks.
- Setup a BJam build system.
- Implement associative containers.
- Implement user-defined type adapters for associative sequences.
- Provide a Main page for the Doxygen documentation.
- Consider making function objects automatically curriable. This could allow
  _super sexy_ stuff like:
  @code
    template <>
    struct Iterable<List> {
        static constexpr auto length_impl = foldl(some_lambda, size_t<0>);
    };
  @endcode

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_HPP
#define BOOST_HANA_HPP

//! @defgroup details Details
//! Implementation details.

//! @defgroup typeclasses Type classes
//! General purpose type classes provided by the library.
//!
//! @todo
//! - Implement better law checking and check them for all instances in the
//!   unit tests. Since we're modularized, it's OK to resort to heavy stuff
//!   in the law-checking because it's presumably only done in unit testing
//!   contexts.
//! - Provide an integrated way of checking laws for type classes.
//! - Provide a way to check type class requirements like
//!   `Functor f => Applicative f`.
//! - Document requirements for the existing type classes.
//! - Find a better way to provide instances between type classes; consider
//!   something like `Foldable::instance<Iterable>`.
//! - Consider inheriting from a base class even when no mcd is required.
//!   That would allow us to _not_ include a useless mcd.
//! - Document the include structure for type classes and the contracts
//!   between headers (what needs to be included and when). Could look like
//!     typeclass.hpp -- everything
//!     typeclass/typeclass.hpp -- forward declaration, operators, methods,
//!                             -- default-provided orphan instances (like
//!                             -- bool for Logical). This one is included by
//!                             -- everyone else in typeclass/, by contract
//!     typeclass/mcd.hpp -- definition of a mcd
//!     typeclass/laws.hpp -- laws of the type class
//! - Consider including the default provided instances in the mcds instead
//!   of the forward declaration header of a type class. However, that won't
//!   work for instances provided implicitly (e.g. Logical <- bool or
//!   Comparable <- x == y), because there is no MCD to be included for those.
//! - Document the purpose of minimal instances; they are meant to provide an
//!   easy to use archetype for testing and their tests are meant to exercice
//!   the basic dispatching code of type classes (hence it makes sense to test
//!   even the mcd of a minimal instance). In particular, they are not meant
//!   to be _the_ minimal instance, which does not exist in general
//!   (what does it even mean to be a _minimal_ instance?)

//! @defgroup datatypes Data types
//! General purpose data types provided by the library.

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integer_list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/traversable.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

#endif // !BOOST_HANA_HPP
