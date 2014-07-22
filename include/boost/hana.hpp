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

#include <boost/hana/applicative.hpp>
#include <boost/hana/bool.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
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
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/record.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/traversable.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

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

/*!
@mainpage Boost.Hana

@tableofcontents


@section preface Preface

The seed that became this library was planted in late 2012, when I first
started to reimplement the [Boost.MPL][] using C++11 in a project named
[MPL11][]. In spring 2014, I applied to [Google Summer of Code][GSoC] with
that project for the Boost organization and got in. The goal was to polish
the MPL11 and get it in Boost by the end of the summer. In May, before GSoC
was started full steam, I presented the project at [C++Now][] and had
insightful conversations with several attendees. The idea that it was
possible to unify the [Boost.Fusion][] and the [Boost.MPL][] libraries
made its way and I became convinced of it after writing the first prototype
for what is now Boost.Hana. We are now in late July and the plan is to
request an official review to get the library in Boost by the end of GSoC;
we'll see how that works out.

Let the fun begin.


@section introduction Introduction

Boost.Hana is a library of combinators tailored towards the manipulation of
heterogeneous collections. However, the core of Hana is a powerful system for
ad-hoc polymorphism inspired by Haskell type classes; this extension system
is then used to provide all the functionality of the library in a generic way.

The library uses a purely functional style of programming, which is required
to manipulate objects of heterogeneous types -- it is impossible to modify the
type of an object, so a new object must be introduced.

Contrary to previous metaprogramming libraries like Boost.MPL and Boost.Fusion,
the design of the library is not based on that of the STL. Rather, it is
strongly inspired by several (standard and non standard) modules written for
the Haskell programming language. Through experience, the author has found
this to be much more expressive, flexible and easy to use while not sacrificing
any performance given the purely functional setting.


@section quick-start Quick start

This section assumes the reader is already familiar with `std::tuple` and
basic metaprogramming. First, let's include the library:

@dontinclude example/quickstart.cpp
@skip boost/hana.hpp
@until using namespace

> #### Note
> Unless specified otherwise, the documentation assumes that the above lines
> are present before examples and code snippets. Use your judgement!

Finer grained headers are provided and will be explained in the
@ref organization section, but for now this will do. Let's create an
heterogeneous list, which is conceptually the same as a `std::tuple`:

@skipline auto xs =

Here, `list` is a generic lambda taking a variable number of arguments and
returning an object which is a valid sequence for Boost.Hana. The actual type
of the object returned by `list` is left unspecified, as will always be the
case in this library. However, something called its "data type" is specified;
`list` returns an object of data type `List`. Data types will be explained in
detail in their own section. There are several operations that can be performed
on lists; here are a couple so you get the feeling:

@skip assert
@until for_each

An interesting observation is that `is_empty` returns a value that can be
constexpr-converted to bool even though the list contains non-constexpr
objects (a `std::string`). Indeed, the size of the sequence is known at
compile-time regardless of its content, so it only makes sense that the
library does not throw away this information. Let's take that `for_each`
for a tour and write a function that prints a list as XML:

@skip auto to_xml =
@until to_xml(

One of the initial goals of the library was to unify type level programming
with value level programming. So in principle, it should be possible to
manipulate types and sequences of types just as one would do with the
Boost.MPL. Here is how Hana does it:

@skip auto ts =
@until fmap

There is a lot going on here. First, `type` is a variable template, and
`type<T>` is an object representing the C++ type `T`. Since it's an object,
it makes perfect sense to create a list out of these guys. Second, `fmap` is
a function similar to `std::transform`: it takes a function object and a list
(actually any `Functor` -- more on this later), applies the function to every
element in the list and returns the resulting list. Now, this means that
somehow `metafunction<std::add_pointer>` is in fact a function object.
Specifically, `metafunction` is a variable template taking a template
template parameter, and `metafunction<std::add_pointer>` is a function
object which accepts a `type<T>` and returns `type<std::add_pointer<T>::%type>`.
Still with me?

So far, we can perform computations on C++ types but we can't really do
anything useful with the result if we can't get the type out, i.e. get the
`T` out of `type<T>`. This is easily done:

@skip static_assert
@until >::value

It turns out that while `type<T>` has an unspecified C++ type, that C++ type
is still guaranteed to have a nested type named `type` which is an alias to
`T`. In Boost.MPL parlance, `decltype(type<T>)` is a nullary metafunction
returning `T`.

This is it for the quick start. Of course, there is much more available like
associative sequences, sets, ranges and even an heterogeneous `std::optional`
called `Maybe`, but you can read on if you want to know more.


@section organization Organization
<!-- organization of the headers -->


@section datatypes Data types


@section type-classes Type classes
<!-- Document what are type classes -->


<!-- Links -->
[Boost.Fusion]: http://www.boost.org/doc/libs/release/libs/fusion/doc/html/index.html
[Boost.MPL]: http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html
[C++Now]: http://cppnow.org
[GSoC]: http://www.google-melange.com/gsoc/homepage/google/gsoc2014
[MPL11]: http://github.com/ldionne/mpl11

 */

#endif // !BOOST_HANA_HPP
