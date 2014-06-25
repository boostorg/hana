/*!
@file
Includes all the headers of the library.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_HPP
#define BOOST_HANA_HPP

//! @defgroup details Details
//! Implementation details.

/*!
@defgroup typeclasses Type classes
General purpose type classes provided by the library.

@todo
- Provide an integrated way of checking laws for type classes.
- Provide a way to check type class requirements like
  `Functor f => Applicative f`.
- Document requirements for the existing type classes.
 */

//! @defgroup datatypes Data types
//! General purpose data types provided by the library.

#include <boost/hana/adapted/std_array.hpp>
#include <boost/hana/adapted/std_integer_sequence.hpp>
#include <boost/hana/adapted/std_integral_constant.hpp>
#include <boost/hana/adapted/std_list.hpp>
#include <boost/hana/adapted/std_pair.hpp>
#include <boost/hana/adapted/std_tuple.hpp>
#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/type.hpp>

#endif // !BOOST_HANA_HPP
