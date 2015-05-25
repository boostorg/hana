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
#include <boost/hana/applicative.hpp>
#include <boost/hana/comonad.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/monad_plus.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/product.hpp>
#include <boost/hana/ring.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/sequence.hpp>
#include <boost/hana/struct.hpp>
#include <boost/hana/traversable.hpp>

// Data types
#include <boost/hana/bool.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
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


// This is a bit stupid, but putting the documentation in the `boost::hana`
// namespace makes Doxygen resolve references like `Iterable` properly.
// Otherwise, we would need to write `boost::hana::Iterable` every time.
namespace boost { namespace hana {

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

//! @defgroup group-config Configuration options
//! Configurable options to tweak the global behavior of the library.

//! @defgroup group-assertions Assertions
//! Macros to perform different kinds of assertions.

//! @defgroup group-details Details
//! Implementation details.

/*!
@mainpage User Manual

@tableofcontents


@section tutorial-preface Preface

------------------------------------------------------------------------------
The seed that became this library was planted in late 2012, when I first
started to reimplement the [Boost.MPL][] using C++11 in a project named
[MPL11][]. In spring 2014, I applied to [Google Summer of Code][GSoC] with
that project for the Boost organization and got in. The goal was to polish
the MPL11 and get it in Boost by the end of the summer. In May, before GSoC
was started full steam, I presented the project at [C++Now][] and had
insightful conversations with several attendees. The idea that it was
possible to unify the [Boost.Fusion][] and the [Boost.MPL][] libraries
made its way and I became convinced of it after writing the first prototype
for what is now Boost.Hana. After working on Hana and polishing many rough
edges during several months, Hana will be going through formal review for
inclusion in Boost from June 10 2015 to June 24 2015.

Let the fun begin!










@section tutorial-introduction Introduction

------------------------------------------------------------------------------
Hana is a header-only library for C++ metaprogramming suited for computations
on both types and values. The functionality it provides is a superset of what
is provided by the well established Boost.MPL and Boost.Fusion libraries. By
leveraging C++11/14 implementation techniques and idioms, Hana boasts faster
compilation times and runtime performance on par or better than previous
metaprogramming libraries, while noticeably increasing the level of
expressiveness in the process. Hana is easy to extend in a ad-hoc manner
and it provides out-of-the-box inter-operation with Boost.Fusion, Boost.MPL
and the standard library.

__Motivation__\n
When Boost.MPL first appeared, it provided C++ programmers with a huge relief
by abstracting tons of template hackery behind a workable interface. This
breakthrough greatly contributed to making C++ template metaprogramming more
mainstream, and today the discipline is deeply rooted in many serious projects.
Recently, C++11 and C++14 brought many major changes to the language, some of
which make metaprogramming much easier, while others drastically widen the
design space for libraries. A natural question then arises: is it still
desirable to have abstractions for metaprogramming, and if so, which ones?
After investigating different options like the [MPL11][], the answer eventually
came by itself in the form of a library; Hana. The key insight to Hana is that
the manipulation of types and values are nothing but two sides of the same
coin. By unifying both concepts, metaprogramming becomes easier and new
exciting possibilities open before us.

__Warning: functional programming ahead__\n
Programming with heterogeneous objects is inherently functional -- since it is
impossible to modify the type of an object, a new object must be introduced
instead, which rules out mutation. Unlike previous metaprogramming libraries
whose design was modeled on the STL, Hana uses a functional style of
programming which is the source for a good portion of its expressiveness.
However, as a result, many concepts used in Hana will be unfamiliar to C++
programmers without a knowledge of FP. The documentation attempts to make
these concepts approachable by using intuition whenever possible, but bear
in mind that the highest rewards are usually the fruit of some effort.










@section tutorial-quickstart Quick start

------------------------------------------------------------------------------
This tutorial assumes the reader is already familiar with basic metaprogramming
and the [C++14 standard][Wikipedia.C++14]. First, let's include the library:

@snippet example/tutorial/quickstart.cpp includes

Unless specified otherwise, the documentation assumes that the above lines
are present before examples and code snippets. Also note that finer grained
headers are provided and will be explained in the [Header organization]
(@ref tutorial-header_organization) section. The goal of this section is to
introduce the main concepts of the library from a very high level. To do so,
we will proceed by example and implement a kind of `switch` statement able to
process `boost::any`s. Given a `boost::any`, the goal is to dispatch to the
function associated to the dynamic type of the `any`:

@snippet example/tutorial/quickstart.cpp usage

@note
In the documentation, we will often use the `s` suffix on string literals to
create `std::string`s without syntactic overhead. This is a standard-defined
[C++14 user-defined literal][Wikipedia.cxx14_udl].


Since the any holds a `char`, the second function is called with the `char`
inside it. If the `any` had held an `int` instead, the first function would
have been called with the `int` inside it. When the dynamic type of the `any`
does not match any of the covered cases, the `%default_` function is called
instead. Finally, the result of the `switch` is the result of calling the
function associated to the `any`'s dynamic type. The type of that result is
inferred to be the common type of the result of all the provided functions:

@snippet example/tutorial/quickstart.cpp result_inference

We'll now look at how this utility can be implemented using Hana. The first
step is to associate each type to a function. To do so, we represent each
`case_` as a `std::pair` whose first element is a type and whose second element
is a function. Furthermore, we (arbitrarily) decide to represent the `%default_`
case as a `std::pair` mapping a dummy type to a function:

@snippet example/tutorial/quickstart.cpp cases

@note
`type<...>` is not a type! It is a [C++14 variable template]
[Wikipedia.variable_template] yielding an object representing a type for Hana.
The details are explained in the section on [type computations](@ref tutorial-type).

To provide the interface we showed above, `switch_` will have to return a
function taking the cases. In other words, `switch_(a)` must be a function
taking any number of cases (which are `std::pair`s), and performing the logic
to dispatch `a` to the right function. This can easily be achieved by having
`switch_` return a C++14 generic lambda:

@code
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    // ...
  };
}
@endcode

Since parameter packs are not very flexible, we put the cases into a tuple. If
you are reading this documentation, chances are you already know `std::tuple`
and `std::make_tuple`. Hana provides its own `tuple` and `make_tuple`, and
this is what we're using here:

@code
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = make_tuple(cases_...);
    // ...
  };
}
@endcode

Notice how the `auto` keyword is used when defining `cases`; it is often
easier to let the compiler deduce the type of the tuple and use `make_tuple`
instead of working out the types manually. The next step is to separate the
default case from the rest of the cases. This is where things start to get
interesting. To do so, we use Hana's `find_if` algorithm, which works a bit
like `std::find_if`:

@code
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = make_tuple(cases_...);

    auto default_ = find_if(cases, [](auto const& c) {
      return c.first == type<_default>;
    });

    // ...
  };
}
@endcode

`find_if` takes a `tuple` and a predicate, and returns the first element of
the tuple which satisfies the predicate. The result is returned as an optional
value called a `Maybe`, which is very similar to a `std::optional`, except
whether that optional value is empty or not is known at compile-time. If the
predicate is not satisfied for any element of the `tuple`, `find_if` returns
`nothing` (an empty value). Otherwise, it returns `just(x)` (a non-empty value),
where `x` is the first element satisfying the predicate. Unlike predicates
used in STL algorithms, the predicate used here must be generic because the
tuple's elements are heterogeneous. Furthermore, that predicate must return
what Hana calls an `IntegralConstant`, which means that the predicate's result
must be known at compile-time. These details are explained in the section on
[amphibian algorithms](@ref tutorial-amphi). Inside the predicate, we simply
compare the type of the case's first element to `type<_default>`. If you recall
that we were using `std::pair`s to encode cases, this simply means that we're
finding the default case among all of the provided cases. But what if no default
case was provided? We should fail at compile-time, of course!

@code
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = make_tuple(cases_...);

    auto default_ = find_if(cases, [](auto const& c) {
      return c.first == type<_default>;
    });
    static_assert(default_ != nothing,
      "switch is missing a default_ case");

    // ...
  };
}
@endcode

Notice how we can use `static_assert` on the result of the comparison with
`nothing`, even though `%default_` is a non-`constexpr` object? Boldly, Hana
makes sure that no information that's known at compile-time is lost to the
runtime, which is clearly the case of the presence of a `%default_` case.
The details are explained in the section on [amphibian algorithms]
(@ref tutorial-amphi). The next step is to gather the set of non-default
cases. To achieve this, we use the `filter` algorithm, effectively filters
a sequence with a given predicate, keeping only the elements satisfying the
predicate:

@code
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = make_tuple(cases_...);

    auto default_ = find_if(cases, [](auto const& c) {
      return c.first == type<_default>;
    });
    static_assert(default_ != nothing,
      "switch is missing a default_ case");

    auto rest = filter(cases, [](auto const& c) {
      return c.first != type<_default>;
    });

    // ...
  };
}
@endcode

The next step is to find the first case matching the dynamic type of the `any`,
and then call the function associated to that case. The simplest way to do this
is to use classic recursion with variadic parameter packs. Of course, we could
probably intertwine Hana algorithms in a convoluted way to achieve this, but
sometimes the best way to do something is to write it from scratch using basic
techniques. To do so, we'll call an implementation function with the contents
of the `rest` tuple by using the `unpack` function:

@snippet example/tutorial/quickstart.cpp switch_

`unpack` takes a `tuple` and a function, and calls the function with the content
of the `tuple` as arguments. The result of `unpack` is the result of calling that
function. In our case, the function is a generic lambda which in turn calls the
`process` function. Our reason for using `unpack` here was to turn the `rest`
tuple into a parameter pack of arguments, which are easier to process recursively
than tuples. Before we move on to the `process` function, it is worthwhile to
explain what `%default_->second` is all about. As we explained earlier, `%default_`
is an optional value. Like `std::optional`, this optional value overloads the
dereference operator and the arrow operator to allow accessing the value inside
the `optional`. If the optional is empty (`nothing`), a compile-time error is
triggered. Since we know `%default_` is not empty (we checked that just above),
what we're doing is simply pass the function associated to the default case to
the `process` function. We're now ready for the final step, which is the
implementation of the `process` function:

@snippet example/tutorial/quickstart.cpp process

There are two overloads of this function: an overload for when there is at least
one case to process, and the base case overload for when there's only the default
case. As we would expect, the base case simply calls the default function and
returns that result. The other overload is slightly more interesting. First,
we retrieve the type associated to that case and store it in `T`. This
`decltype(...)::%type` dance might seem convoluted, but it is actually quite
simple. Roughly speaking, this takes a type represented as an object (a `type<T>`)
and pulls it back down to the type level (a `T`). The details are explained in
the section on [type-level computations](@ref tutorial-type). Then, we compare
whether the dynamic type of the `any` matches this case, and if so we call the
function associated to this case with the `any` casted to the proper type.
Otherwise, we simply call `process` recursively with the rest of the cases.
Pretty simple, wasn't it? Here's the final solution, which spans a big total
of 42 lines:

@snippet example/tutorial/quickstart.cpp full

That's it for the quick start! This example only introduced a couple of useful
algorithms (`find_if`, `filter`, `unpack`) and heterogeneous containers (`Tuple`,
`Maybe`), but rest assured that there is much more. All the algorithms are
documented by the concept to which they belong (`Foldable`, `Iterable`,
`Searchable`, `Sequence`, etc...), which is similar to the way most generic
libraries are documented (e.g. the [SGI STL][SGI.Container]). Apart from tuples
and optional values, there are also other kinds of containers provided by Hana;
they are documented in their respective page (`Range`, `Set`, `Map`, etc..). The
next sections of the tutorial gradually introduce general concepts pertaining to
Hana in a friendly way, but you may use the following cheatsheet for quick
reference if you want to start coding right away. For the record, always keep
in mind that the algorithms return their result as a new container and no
in-place mutation is ever performed, which is detailed in the section on
[algorithm semantics](@ref tutorial-sem).


@subsection tutorial-quickstart-cheatsheet Cheatsheet

function                                     |  concept   | description
:------------------------------------------  | :--------  | :----------
`transform(sequence, f)`                     | Functor    | Apply a function to each element of a sequence and return the result.
`adjust_if(sequence, predicate, f)`          | Functor    | Apply a function to each element of a sequence satisfying some predicate and return the result.
`adjust(sequence, value, f)`                 | Functor    | Apply a function to each element of a sequence that compares equal to some value and return the result.
`replace_if(sequence, predicate, newval)`    | Functor    | Replace the elements of a sequence that satisfy some predicate by some value.
`replace(sequence, oldval, newval)`          | Functor    | Replace the elements of a sequence that compare equal to some value by some other value.
`fill(sequence, value)`                      | Functor    | Replace all the elements of a sequence with some value.
`fold.{left,right}(sequence[, state], f)`    | Foldable   | Accumulates the elements of a sequence from the left or right, and optionally with a provided initial state.
`for_each(sequence, f)`                      | Foldable   | Call a function on each element of a sequence. Returns `void`.
`{length, size}(sequence)`                   | Foldable   | Returns the length of a sequence as an IntegralConstant.
`{minimum, maximum}(sequence[, predicate])`  | Foldable   | Returns the smallest/greatest element of a sequence, optionally according to a predicate. The elements must be Orderable if no predicate is provided.
`count_if(sequence, predicate)`              | Foldable   | Returns the number of elements that satisfy the predicate.
`count(sequence, value)`                     | Foldable   | Returns the number of elements that compare equal to the given value.
`unpack(sequence, f)`                        | Foldable   | Calls a function with the contents of a sequence. Equivalent to `f(x1, ..., xN)`.
`head(sequence)`                             | Iterable   | Returns the first element of a sequence.
`tail(sequence)`                             | Iterable   | Returns all the elements except the first one. Analogous to `pop_front`.
`is_empty(sequence)`                         | Iterable   | Returns whether a sequence is empty as an IntegralConstant.
`at(index, sequence)`                        | Iterable   | Returns the n-th element of a sequence. The index must be an IntegralConstant.
`last(sequence)`                             | Iterable   | Returns the last element of a sequence.
`drop(number, sequence)`                     | Iterable   | Drops the n first elements from a sequence and returns the rest. `n` must be an IntegralConstant.
`drop_{while,until}(sequence, predicate)`    | Iterable   | Drops elements from a sequence while/until a predicate is satisfied. The predicate must return an IntegralConstant.
`flatten(sequence)`                          | Monad      | Flatten a sequence of sequences, a bit like `std::tuple_cat`.
`prepend(value, sequence)`                   | MonadPlus  | Prepend an element to a sequence.
`append(sequence, value)`                    | MonadPlus  | Append an element to a sequence.
`concat(sequence1, sequence2)`               | MonadPlus  | Concatenate two sequences.
`filter(sequence, predicate)`                | MonadPlus  | Remove all the elements that do not satisfy a predicate. The predicate must return an IntegralConstant.
`remove_if(sequence, predicate)`             | MonadPlus  | Remove all the elements that satisfy a predicate. The predicate must return an IntegralConstant.
`remove(sequence, value)`                    | MonadPlus  | Remove all the elements that are equal to a given value.
`{any,none,all}_of(sequence, predicate)`     | Searchable | Returns whether any/none/all of the elements of the sequence satisfy some predicate.
`{any,none,all}(sequence)`                   | Searchable | Returns whether any/non/all of the elements of a sequence are true-valued.
`contains(sequence, value)`                  | Searchable | Returns whether a sequence contains the given object.
`find_if(sequence, predicate)`               | Searchable | Find the first element of a sequence satisfying the predicate and return `just` it, or return `nothing`. See Maybe.
`find(sequence, value)`                      | Searchable | Find the first element of a sequence which compares equal to some value and return `just` it, or return nothing. See Maybe.
`group(sequence[, predicate])`               | Sequence   | %Group adjacent elements of a sequence which all satisfy (or all do not satisfy) some predicate. The predicate defaults to equality, in which case the elements must be Comparable.
`init(sequence)`                             | Sequence   | Returns all the elements of a sequence, except the last one. Analogous to `pop_back`.
`partition(sequence, predicate)`             | Sequence   | Partition a sequence into a pair of elements that satisfy some predicate, and elements that do not satisfy it.
`remove_at(index, sequence)`                 | Sequence   | Remove the element at the given index. The index must be an `integral_constant`.
`reverse(sequence)`                          | Sequence   | Reverse the order of the elements in a sequence.
`slice(sequence, from, to)`                  | Sequence   | Returns the elements of a sequence at indices contained in `[from, to)`.
`subsequence(sequence, indices)`             | Sequence   | Returns the elements of a sequence at the `indices` in the given sequence.
`sort(sequence[, predicate])`                | Sequence   | Sort (stably) the elements of a sequence, optionally according to a predicate. The elements must be Orderable if no predicate is provided.
`take(number, sequence)`                     | Sequence   | Take the first n elements of a sequence. n must be an `integral_constant`.
`take_{while,until}(sequence, predicate)`    | Sequence   | Take elements of a sequence while/until some predicate is satisfied, and return that.
`zip(sequence1, ..., sequenceN)`             | Sequence   | Zip `N` sequences into a sequence of tuples.
`zip.with(f, sequence1, ..., sequenceN)`     | Sequence   | Zip `N` sequences with a `N`-ary function.


<!-- Links -->
[SGI.Container]: https://www.sgi.com/tech/stl/Container.html
[Wikipedia.cxx14_udl]: http://en.wikipedia.org/wiki/C%2B%2B14#Standard_user-defined_literals










@section tutorial-quadrants C++ computational quadrants

------------------------------------------------------------------------------
To really understand what is Hana all about, it is essential to understand the
different types of computations in C++. We will focus our attention on four
different kinds of computations, even though a finer grained separation would
be possible. First, we have runtime computations, which are the usual
computations we use in C++. In that world, we have runtime containers,
runtime functions and runtime algorithms:

@snippet example/tutorial/quadrants.cpp runtime

The usual toolbox for programming within this quadrant is the C++ standard
library, which provides reusable algorithms and containers operating at
runtime. Since C++11, a second kind of computation is possible: `constexpr`
computations. There, we have `constexpr` containers, `constexpr` functions
and `constexpr` algorithms:

@code
constexpr int factorial(int n) {
  return n == 0 ? 1 : n * factorial(n - 1);
}

template <typename T, std::size_t N, typename F>
  constexpr std::array<std::result_of_t<F(T)>, N>
transform(std::array<T, N> array, F f) {
  // ...
}

constexpr std::array<int, 4> ints{{1, 2, 3, 4}};
constexpr std::array<int, 4> facts = transform(ints, factorial);
static_assert(facts == std::array<int, 4>{{1, 2, 6, 24}}, "");
@endcode

@note
For the above code to actually work, `std::array`'s `operator==` would have to
be marked `constexpr`, which is not the case (even in C++14).

Basically, a `constexpr` computation is different from a runtime computation
in that it is simple enough to be evaluated (interpreted, really) by the
compiler. In general, any function that does not perform anything too
_unfriendly_ to the compiler's evaluator (like throwing or allocating memory)
can be marked `constexpr` without any further change. This makes `constexpr`
computations very similar to runtime computations, except `constexpr`
computations are more restricted and they gain the ability to be evaluated
at compile-time. Unfortunately, there is no commonly used toolbox for
`constexpr`-programming, i.e. there is no widely adopted "standard library"
for `constexpr` programming. However, the [Sprout][] library may be worth
checking out for those with some interest in `constexpr` computations.

The third kind of computations are heterogeneous computations. Heterogeneous
computations differ from normal computations in that instead of having
containers holding homogeneous objects (all objects having the same type),
the containers may hold objects with different types. Furthermore, functions
in this quadrant of computation are _heterogeneous_ functions, which is a
complicated way of talking about template functions. Similarly, we have
heterogeneous algorithms that manipulate heterogeneous containers and
functions:

@snippet example/tutorial/quadrants.cpp heterogeneous

If manipulating heterogeneous containers seems overly weird to you, just think
of it as glorified `std::tuple` manipulation. In a C++03 world, the go-to
library for doing this kind of computation is [Boost.Fusion][], which provides
several data structures and algorithms to manipulate heterogeneous collections
of data. The fourth and last quadrant of computation that we'll be considering
here is the quadrant of type-level computations. In this quadrant, we have
type-level containers, type-level functions (usually called metafunctions)
and type-level algorithms. Here, everything operates on types: containers hold
types and metafunctions take types as arguments and return types as results.

@snippet example/tutorial/quadrants.cpp type-level

The realm of type-level computations has been explored quite extensively, and
the de-facto solution for type-level computations in C++03 is a library named
[Boost.MPL][], which provides type-level containers and algorithms. For
low-level type transformations, the metafunctions provided by the
`<type_traits>` standard header can also be used since C++11.


@subsection tutorial-quadrants-about What is this library about?

So all is good, but what is this library actually about? Now that we have set
the table by clarifying the kinds of computations available to us in C++, the
answer might strike you as very simple. __The purpose of Hana is to merge the
3rd and the 4th quadrants of computation__. More specifically, Hana is a
(long-winded) constructive proof that heterogeneous computations are strictly
more powerful than type-level computations, and that we can therefore express
any type-level computation by an equivalent heterogeneous computation. This
construction is done in two steps. First, Hana is a fully featured library of
heterogeneous algorithms and containers, a bit like a modernized Boost.Fusion.
Second, Hana provides a way of translating any type-level computation into its
equivalent heterogeneous computation and back, which allows the full machinery
of heterogeneous computations to be reused for type-level computations without
any code duplication. Of course, the biggest advantage of this unification is
seen by the user, as you will witness by yourself.

<!-- Links -->
[Sprout]: https://github.com/bolero-MURAKAMI/Sprout










@section tutorial-assert Assertions

------------------------------------------------------------------------------
In the rest of this tutorial, you will come across code snippets where different
kinds of assertions like `BOOST_HANA_RUNTIME_CHECK` and `BOOST_HANA_CONSTANT_CHECK`
are used. Like any sensible `assert` macro, they basically check that the
condition they are given is satisfied. However, in the context of heterogeneous
programming, some informations are known at compile-time, while others are known
only at runtime. The exact type of assertion that's used in a context tells you
whether the condition that's asserted upon can be known at compile-time or if it
must be computed at runtime, which is a very precious piece of information. Here
are the different kinds of assertions used in the tutorial, with a small
description of their particularities. For more details, you should check
the [reference on assertions](@ref group-assertions).

assertion                    | description
:--------------------------- | :----------
`BOOST_HANA_RUNTIME_CHECK`   | Assertion on a condition that is not known until runtime. This assertion provides the weakest form of guarantee.
`BOOST_HANA_CONSTEXPR_CHECK` | Assertion on a condition that would be `constexpr` if lambdas were allowed inside constant expressions. In other words, the only reason for it not being a `static_assert` is the language limitation that lambdas can't appear in constant expressions, which [might be lifted][N4487] in C++17.
`BOOST_HANA_CONSTANT_CHECK`  | Assertion on what Hana calls a compile-time `Logical`. Basically, this means that the truth value of the expression is known at compile-time even if the value of that expression might only be known at runtime. The details are explained in the section on [IntegralConstants](@ref tutorial-constexpr-constants), but here's a hint about how this works: the truth value of the expression is encoded in the _type_ of that expression, not in its value (which might be runtime). This assertion provides the strongest form of guarantee.

<!-- Links -->
[N4487]: https://isocpp.org/files/papers/N4487.pdf










@section tutorial-create Creating sequences

------------------------------------------------------------------------------
Like we saw in the quick start, a tuple can be created with `make_tuple`. In
general, sequences in Hana may be created with the `make` function:

@snippet example/tutorial/create.cpp make<Tuple>

Actually, `make_tuple` is just a shortcut for `make<Tuple>` so you don't
have to type `boost::hana::make<boost::hana::Tuple>` when you are out of
Hana's namespace. Simply put, `make<...>` is is used all around the
library to create different types of objects, thus generalizing the
`std::make_xxx` family of functions. For example, one can create
a Range of compile-time integers with `make<Range>`:

@snippet example/tutorial/create.cpp make<Range>

@note
`int_<...>` is not a type! It is a [C++14 variable template]
[Wikipedia.variable_template] yielding what Hana calls an IntegralConstant.

For convenience, whenever a component of Hana provides a `make<XXX>` function,
it also provides the `make_xxx` shortcut to reduce typing. Also, an
interesting point that can be raised in this example is the fact that
`r` is `constexpr`. In general, whenever a Hana sequence is initialized
only with constant expressions (which is the case for `int_<...>`), that
sequence may be marked as `constexpr`.










@section tutorial-sem Algorithm semantics

------------------------------------------------------------------------------
By default, all the sequences in Hana hold their elements by value and hence
they own them. For example, when creating a tuple, the tuple will make copies
of the elements it is initialized with:

@snippet example/tutorial/sem.cpp copy_initialize

When given the chance, the tuple will move the values in:

@snippet example/tutorial/sem.cpp move_initialize

Algorithms in Hana always return a new sequence containing the result.
This allows one to easily chain algorithms by simply using the result of the
first as the input of the second. For example, to apply a function to every
element of a tuple and then reverse the result, one simply has to connect the
`reverse` and `transform` algorithms:

@snippet example/tutorial/sem.cpp reverse_transform

This is different from the algorithms of the standard library, where one has
to provide iterators to the underlying sequence. Iterator based designs
have their own merits like low coupling and performance, but they also have
drawbacks like reducing the composability of algorithms. In the context
of heterogeneous sequences, iterators are also less useful. For example,
incrementing an iterator would have to return a new iterator with a different
type, because the type of the new object it is pointing to in the sequence
might be different. To deal with complexity, Hana uses different abstractions
that are composable and happen to be efficient given the heterogeneous setting.

Algorithms in Hana are not lazy. When an algorithm is called, it does its
job and returns a new sequence containing the result, end of the story.
For example, calling the `permutations` algorithm on a large sequence is
a stupid idea, because Hana will actually compute all the permutations:

@code
    auto perms = permutations(make_tuple(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
    // perms has 3 628 800 elements, and your compiler just crashed
@endcode

To contrast, algorithms in Boost.Fusion return views which hold the original
sequence by reference and apply the algorithm on demand, as the elements of
the sequence are accessed. This leads to subtle lifetime issues, like having
a view that refers to a sequence that was destroyed. Hana's design assumes
that most of the time, we want to access all or almost all the elements in a
sequence anyway, and hence performance is not a big argument in favor of
laziness.

One might think that returning full sequences that own their elements from an
algorithm would lead to tons of undesirable copies. For example, when using
`reverse` and `transform`, one could think that an intermediate copy is made
after the call to `transform`:

@snippet example/tutorial/sem.cpp reverse_transform_copy

To make sure this does not happen, Hana uses perfect forwarding and move
semantics heavily so it can provide an almost optimal runtime performance.
So instead of doing a copy, a move occurs between `reverse` and `transform`:

@snippet example/tutorial/sem.cpp reverse_transform_move

Ultimately, the goal is that code written using Hana should be equivalent to
clever hand-written code, except it should be enjoyable to write. Performance
considerations are explained in depth in their own [section](@ref tutorial-perf).










@section tutorial-amphi Amphibian algorithms

------------------------------------------------------------------------------
Like we saw in the quick start, some functions are able to return something
that can be used in a constant expression even when they are called on a
non-`constexpr` object. For example, let's consider the `length` function
applied to a non-`constexpr` tuple:

@snippet example/tutorial/amphi.cpp setup

Obviously, the tuple can't be made `constexpr`, since it contains `std::string`s.
Still, even though it is not called on a constant expression, `length` returns
something that can be used inside one. If you think of it, the size of the
tuple is known at compile-time regardless of its content, and hence it only
makes sense that Hana does not throw away this information. If that seems
surprising, think about `std::tuple`:

@snippet example/tutorial/amphi.cpp std_tuple_size

Since the size of the tuple is encoded in it's type, it's always available
at compile-time regardless of whether the tuple is `constexpr` or not. In
Hana, this is implemented by having `length` return what we call an
IntegralConstant. Since an IntegralConstant is convertible to the integral
value it represents at compile-time, it can be used in a constant expression.
There are subtleties that could be highlighted, but this is left to the more
hardcore [section](@ref tutorial-constexpr) on the limitations of `constexpr`.
`length` is not the only function that returns an IntegralConstant; for
example, `is_empty` does that too:

@snippet example/tutorial/amphi.cpp is_empty

More generally, any algorithm that queries something that can be known at
compile-time will be able of returning such an IntegralConstant. To illustrate,
let's take a look at the `all` algorithm, which is analogous to `std::all_of`:

@snippet example/tutorial/amphi.cpp all_of_runtime

Given a sequence and a predicate, `all` returns whether the predicate is
satisfied by all the elements of the sequence. In this example, the result
can't be known at compile-time, because our predicate returns a `bool` that's
the result of comparing two `std::string`s. Since `std::string`s can't be
compared at compile-time, our predicate must operate at runtime, and the
overall result of the algorithm can then only be known at runtime too.
However, let's say we used `all` with the following predicate instead:

@snippet example/tutorial/amphi.cpp all_of_compile_time

@note
For this to work, the external adapters for `std::integral_constant` contained
in `boost/hana/ext/std/integral_constant.hpp` have to be included.

First, since the predicate is only querying information about the type of the
`.name` member of an element of the tuple, it is clear that its result can be
known at compile-time. Since the number of elements in the tuple is also known
at compile-time, the overall result of the algorithm can, in theory, be known
at compile-time. More precisely, what happens is that the predicate returns a
default constructed `std::is_same<...>`, which inherits from
`std::integral_constant`. Hana recognizes these objects, and `all` is written
in such a way that it preserves the fact that the predicate's result is known
at compile-time. In the end, `all` returns an IntegralConstant holding the
result of the algorithm, and we use the compiler's type deduction in a clever
way to make it look easy. Hence, it would be equivalent to write (but then
you would need to already know the result of the algorithm!):

@snippet example/tutorial/amphi.cpp all_of_compile_time_integral_constant

We just saw how some algorithms are able to return `IntegralConstant`s when
their inputs satisfy some constraints with respect to `compile-time`ness.
However, other algorithms are more restrictive and they _require_ their inputs
to satisfy some constraints regarding `compile-time`ness, without which they
are not able to operate at all. An example of this is `filter`, which takes a
sequence and a predicate, and returns a new sequence containing only those
elements for which the predicate is satisfied. `filter` requires the predicate
to return an IntegralConstant (actually, a Constant holding a Logical). While
this requirement may seem stringent, it really makes sense if you think about
it. Indeed, since we're removing some elements from the tuple, the type of the
object that's going to be returned by `filter` depends on the result of the
predicate. Hence, the result of the predicate has to be known by the compiler
to fix the type of the returned sequence. For example, consider what happens
when we try to filter our sequence as follows:

@code
    auto result = filter(stuff, [](auto x) {
        return x.name == "Louis";
    });
@endcode

Clearly, we know that the predicate will only return true on the first
element, and hence the result _should be_ a `_tuple<Person>`. However,
the compiler has no way of knowing it since the predicate's result is the
result of a runtime computation, which happens long after the compiler
has finished its job. However, we could filter our sequence with any
predicate whose result is available at compile-time:

@snippet example/tutorial/amphi.cpp filter

Since the predicate returns a Constant, the compiler is able to figure out
the return type of the algorithm. Other algorithms like `partition` and `sort`
work similarly; special requirements are always documented by the functions
they apply to. While this constitutes a fairly complete explanation of the
interaction between runtime and compile-time inside algorithms, a deeper
insight can be gained by reading the [section](@ref tutorial-constexpr) on
the limitations of `constexpr` and the reference for Constant and
IntegralConstant.











@section tutorial-type Type computations

------------------------------------------------------------------------------
At this point, if you are interested in doing Boost.MPL-like computations on
types, you might be wondering how is Hana going to help you. Do not despair.
Hana provides a way to perform type-level computations with a great deal of
expressiveness by representing types as values. This is a completely new way
of metaprogramming, and you should try to set your MPL habits aside for a bit
if you want to become proficient with Hana. Basically, Hana provides a way of
representing a type `T` as an object, and it also provides a way of applying
type transformations to those objects as-if they were functions, by wrapping
them properly:

@snippet example/tutorial/type.cpp type

@note
The `type<int>` expression is _not_ a type! It is an object, more precisely a
[variable template][Wikipedia.variable_template] defined roughly as
@code
    template <typename T>
    constexpr the-type-of-the-object type{};
@endcode

Now, since `type<...>` is just an object, we can store it in a heterogeneous
sequence like a tuple. This also means that all the algorithms that apply to
usual heterogeneous sequences are available to us, which is nice:

@snippet example/tutorial/type.cpp type_sequence

Also, since typing `type<...>` can be annoying at the end of the day, Hana
provides a variable template called `tuple_t`, which creates a tuple of
`type<...>`s:

@snippet example/tutorial/type.cpp tuple_t

I won't say much more about `type` and `metafunction` (see the [reference]
(@ref Type)), but the last essential thing to know is that `decltype(type<T>)`
is a MPL nullary metafunction. In other words, `decltype(type<T>)::%type` is
an alias to `T`:

@snippet example/tutorial/type.cpp type_as_nullary_metafunction

This way, you can recover the result of a type computation by unwrapping it
with `decltype(...)::%type`. Hence, doing type-level metaprogramming with Hana
is usually a three step process:
1. Wrap the types with `type<...>` so they become values
2. Apply whatever type transformation `F` by using `metafunction<F>`
3. Unwrap the result with `decltype(...)::%type`

At this point, you must be thinking this is incredibly cumbersome. Why on
earth would you want to write

@snippet example/tutorial/type.cpp type_three_step_cumbersome

instead of simply writing

@snippet example/tutorial/type.cpp type_three_step_alternative

The answer is that of course you don't! For simple type computations such as
this one, where you know the type transformation and the type itself, just
use the most straightforward way of doing it. However, for more complex
type computations, the syntactic noise of this three step process becomes
negligible in light of the expressiveness gain of working with values instead
of types inside that computation. Indeed, since we're working inside real
functions instead of clunky structs, we can use variables, lambdas and a
reasonable syntax. It also means that we don't need `typename` all around
the place, which is nice (but can be avoided in MPL world with aliases).
But this syntactic unification is not only some sugar for our eyes; it also
means that some of our "metafunctions" will actually also work on normal
values. Hence, any piece of generic enough code will work with both types
and values out-of-the-box, without any extra work on our side. With previous
approaches to type-level static metaprogramming, we had to reimplement type
sequences and basically everything else from the ground up. For example,
consider the simple problem of applying a transformation to each element in
a sequence of sequences. For some metafunction `f` and sequence

@code
    [
        [x1, x2, ..., xN],
        [y1, y2, ..., yM],
        ...
        [z1, z2, ..., zK]
    ]
@endcode

we want to produce a sequence

@code
    [
        [f<x1>::type, f<x2>::type, ..., f<xN>::type],
        [f<y1>::type, f<y2>::type, ..., f<yM>::type],
        ...
        [f<z1>::type, f<z2>::type, ..., f<zK>::type]
    ]
@endcode

With Hana, this is very straightforward, and in fact the resulting algorithm
will even work when given a regular function and values instead of a
metafunction and types:

@snippet example/tutorial/type.cpp apply_to_all

However, expressing the same algorithm using Boost.MPL requires using lambda
expressions, which are much more limited than plain lambdas. It also requires
being at class or global scope, which means that you can't create this
algorithm on the fly (e.g. inside a function), and it will only work on types!

@snippet example/tutorial/type.cpp apply_to_all_mpl


That's it for the introduction to type computations with Hana, but there are
a couple of interesting examples scattered in the documentation if you want
more. There's also a minimal reimplementation of the MPL using Hana under
the hood in `example/misc/mini_mpl.cpp`.

@todo
- Provide links to the scattered examples, and also to example/misc/mini_mpl.
  For some reason, I can't get Doxygen to generate a link.
- Expand this section to explain the philosophy behind Hana's metaprogramming
  paradigm, i.e. that you only retrieve the types at the end and that you do
  not try to represent everything as a type.
- Write a cheatsheet mapping common MPL/Fusion idioms to idiomatic Hana code.
  %Maybe this should go in some Appendix?
- Introduce the mini-MPL
- Introduce the integration with `<type_traits>`










@section tutorial-perf Performance considerations

------------------------------------------------------------------------------
C++ programmers love performance, so here's a whole section dedicated to it.
Since Hana lives on the frontier between runtime and compile-time computations,
we are not only interested in runtime performance, but also compile-time
performance. Since both topics are pretty much disjoint, we treat them
separately below.

@note
The benchmarks presented in this section are updated automatically when we
push to the repository. If you notice results that do not withstand the
claims made here, open a [GitHub issue][GitHub.issues]; it could
be a performance regression.


@subsection tutorial-perf-compile Compile-time performance

C++ metaprogramming brings its share of awful things. One of the most annoying
and well-known problem associated to it is interminable compilation times.
Hana claims to be more compile-time efficient than its predecessors; this is
a bold claim and we will now try to back it. Of course, Hana can't do miracles;
metaprogramming is a byproduct of the C++ template system and the compiler is
not meant to be used as an interpreter for some meta language. However, by
using cutting edge and intensely benchmarked techniques, Hana is able to
minimize the strain on the compiler.

Before we dive, let me make a quick note on the methodology used to measure
compile-time performance in Hana. Previous metaprogramming libraries measured
the compile-time complexity of their meta-algorithms and meta-sequences by
looking at the number of instantiations the compiler had to perform. While
easy to understand, this way of measuring the compile-time complexity actually
does not give us a lot of information regarding the compilation time, which
is what we're interested in minimizing at the end of the day. Basically, the
reason for this is that template metaprogramming is such a twisted model of
computation that it's very hard to find a standard way of measuring the
performance of algorithms. Hence, instead of presenting meaningless complexity
analyses, we prefer to benchmark everything on every supported compiler and to
pick the best implementation on that compiler. Now, let's dive.

First, Hana minimizes its dependency on the preprocessor. In addition to
yielding cleaner error messages in many cases, this reduces the overall
parsing and preprocessing time for header files. Also, because Hana only
supports cutting edge compilers, there are very few workarounds in the
library, which results in a cleaner and smaller library. Finally, Hana
minimizes reliance on any kind of external dependencies. In particular,
it only uses other Boost libraries in a few specific cases, and it does
not rely on the standard library for the largest part. There are several
reasons (other than include times) for doing so; they are documented in
the [rationales](@ref tutorial-rationales).

Below is a chart showing the time required to include different libraries. The
chart shows the time for including everything in the (non-external) public API
of each library. For example, for Hana this means the `<boost/hana.hpp>` header,
which excludes the external adapters. For other libraries like Boost.Fusion,
this means including all the public headers in the `boost/fusion/` directory,
but not the adapters for external libraries like the MPL.

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.including.compile.json">
</div>

In addition to reduced preprocessing times, Hana uses modern techniques to
implement heterogeneous sequences and algorithms in the most compile-time
efficient way possible. Before jumping to the compile-time performance of
the algorithms, we will have a look at the compile-time cost of creating
heterogeneous sequences. Indeed, since we will be presenting algorithms that
work on sequences, we must be aware of the cost of creating the sequences
themselves, since that will influence the benchmarks for the algorithms.
The following chart presents the compile-time cost of creating sequences
of `n` elements.

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.make.compile.json">
</div>

@note
You can zoom on the chart by selecting an area to zoom into. Also, you can
hide a series of points by clicking on it in the legend on the right.

The benchmark methodology is to always create the sequences in the most
efficient way possible. For Hana, this simply means using the `make<Tuple>`
function. However, for the MPL, this means creating a `mpl::vectorN` of size
up to 20, and then using `mpl::push_back` to create larger vectors. We use a
similar technique for Fusion sequences. The reason for doing so is that
Fusion and MPL sequences have fixed size limits, and the techniques used
here have been found to be the fastest way to create longer sequences.

As you can see, Hana's compile-time _complexity_ is better than the alternatives.
However, if you look closer at the curves, you will see that the MPL has lower
compile-times for small numbers of elements. This is because including Hana's
`Tuple` takes more time than including `mpl::vector`, and you are witnessing
that slowdown by a constant amount. The reason why including Hana's `Tuple` is
slower than including `mpl::vector` is that Hana's `Tuple` includes all the
algorithms it can be used with, while `mpl::vector` only includes the strict
minimum. Considering you need to include most of them manually when using the
MPL, this constant slowdown will be nonexistent in real code and Hana's
approach just makes it less painful for the programmer.

You can also see that creating sequences has a non-negligible cost. Actually,
this is really the most expensive part, as you will see in the following charts
showing the compile-time performance of algorithms. When you look at the charts
here and elsewhere in the library, keep in mind the cost of merely creating the
sequences. Also note that only the most important algorithms will be presented
here, but micro benchmarks for compile-time performance are scattered in the
reference documentation. Also, the benchmarks we present compare several
different libraries. However, since Hana and Fusion can work with values and
not only types, comparing their algorithms with type-only libraries like MPL
is not really fair. Indeed, Hana and Fusion algorithms are more powerful since
they also allow runtime effects to be performed. However, the comparison
between Fusion and Hana is fair, because both libraries are just as powerful
(strictly speaking).

The first algorithm which is ubiquitous in metaprogramming is `transform`.
It takes a sequence and a function, and returns a new sequence containing the
result of applying the function to each element. The following chart presents
the compile-time performance of applying `transform` to a sequence of `n`
elements. The `x` axis represents the number of elements in the sequence, and
the `y` axis represents the compilation time in seconds. Also note that we're
using the `transform` equivalent in each library; we're not using the
`transform` algorithm from Hana through the Boost.Fusion adapters, for
example, which would likely be less efficient.

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.transform.compile.json">
</div>

You probably also noticed how there are two slightly different curves for
Hana's `Tuple`. Those curves measure slightly different usage patterns of
the `transform` algorithm. We benchmark the algorithm both on a standard Hana
`Tuple` and on a `Tuple` created through `hana::tuple_t`, and mapping a Hana
`Metafunction` instead of a regular function. The reason is that it is
possible to optimize some algorithms (like `transform`) when we know we're
actually mapping a metafunction on a type sequence. Basically, we can do the
whole algorithm at the type level behind the scenes, which is more efficient,
but you still get the nice value-level interface.

@note
The representation of `tuple_t` is not really optimized right now, so there is
no difference between using it and not using it. You should still use `tuple_t`
to create type-only `Tuple`s, as some nice optimizations will be implemented.

The second important class of algorithms are folds. Folds can be used to
implement many other algorithms like `count_if`, `minimum` and so on.
Hence, a good compile-time performance for fold algorithms ensures a good
compile-time performance for those derived algorithms, which is why we're
only presenting folds here. Also note that all the non-monadic fold variants
are somewhat equivalent in terms of compile-time, so we only present the left
folds. The following chart presents the compile-time performance of applying
`fold.left` to a sequence of `n` elements. The `x` axis represents the number
of elements in the sequence, and the `y` axis represents the compilation time
in seconds. The function used for folding is a dummy function that does nothing.
In real code, you would likely fold with a nontrivial operation, so the curves
would be worse than that. However, these are micro benchmarks and hence they
only show the performance of the algorithm itself.

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.fold_left.compile.json">
</div>

The third and last algorithm that we present here is the `find_if` algorithm.
This algorithm is difficult to implement efficiently, because it requires
stopping at the first element which satisfies the given predicate. For the
same reason, modern techniques don't really help us here, so this algorithm
constitutes a good test of the implementation quality of Hana disregarding
the metaprogramming free lunch given to us by C++14.

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.find_if.compile.json">
</div>

As you can see, Hana performs better than Fusion, and as well as MPL, yet
Hana's `find_if` can be used with values too, unlike MPL's. This concludes
the section on compile-time performance, but there are micro benchmarks of
compile-time performance scattered around the documentation if you want to
see the compile-time behavior of a particular algorithm.


@subsection tutorial-perf-runtime Runtime performance

@todo Write this section.










@section tutorial-constexpr The limitations of constexpr

------------------------------------------------------------------------------
In C++, the border between compile-time and runtime is hazy, a fact that is
even more true with the introduction of [generalized constant expressions]
[Wikipedia.generalized_constexpr] in C++11. However, being able to manipulate
heterogeneous objects is all about understanding that border and then crossing
it at one's will. The goal of this section is to set things straight with
`constexpr`; to understand which problems it can solve and which ones it
can't. Let's start off with a [GOTW][] style question: do you think the
following code should compile, and why?

@code
    template <typename T>
    constexpr T assert_positive(T i) {
        static_assert(i > 0, "");
        return i;
    }

    int main() {
        constexpr int i = 2;
        assert_positive(i);
    }
@endcode

The right answer is "no". The error given by Clang goes like

@code
    error: static_assert expression is not an integral constant expression
        static_assert(i > 0, "");
                      ^~~~~
@endcode

The problem is that when you are in a function (`constexpr` or otherwise), you
can't use an argument as a constant expression, even if the argument turns out
to be a constant expression when you call the function. If you are surprised,
consider the following code; the function is not a template anymore, which
doesn't change anything with respect to the `constexpr`-ness of the argument
if you think about it. Yet, the answer is now obvious:

@code
    constexpr int assert_positive(int i) {
        static_assert(i > 0, "");
        return i;
    }

    int main() {
        constexpr int i = 2;
        assert_positive(i);
    }
@endcode

Actually, since the body of `assert_positive` is not dependent anymore, Clang
does not even reach `main` before giving the same error as before. Now, this
is not a big problem for static assertions because we have [another way]
[constexpr_throw] of reporting errors inside `constexpr` functions. However,
it also means that we can't use an argument as a non-type template parameter,
since that requires a constant expression. In other words, we can't create
types that are dependent on the _value_ of an argument in C++, which is
nothing new if you think about it:

@code
    template <int i>
    struct foo { };

    void f(int i) {
        foo<i> x; // obviously won't work
    }
@endcode

In particular, this means that the return type of a function can't depend on
the value of its arguments; it may only depend on their type, and `constexpr`
can't change this fact. This is of utmost importance to us, because we're
interested in manipulating heterogeneous objects and eventually returning them
from functions. Some of these functions might want to return an object of type
`T` in one case and an object of type `U` in the other; from our little
analysis, we now know that these "cases" will have to depend on information
encoded in the _types_ of the arguments, not in their _values_.


@subsection tutorial-constexpr-constants Constants

To represent this fact, Hana defines the concept of a `Constant`, which is an
object from which a constant expression may always be obtained, regardless of
the `constexpr`-ness of the object. `Constant`s provide a way to obtain that
constant expression through the use of the `value` function. Specifically, for
any `Constant` `c`, the following must be valid:

@code
    constexpr auto x = value<decltype(c)>();
@endcode

This requirement that must be respected by `Constant`s expresses the minimal
requirement that we're able to retrieve a constant expression from an object.
There is no restriction on what the type of the constant expression might be,
but it should be documented. Hana provides a model of this concept called an
`IntegralConstant`; it encodes a compile-time value of an integral type, and
you can think of it as a `std::integral_constant`. Before going on to the next
section, you probably want to take a look at the [reference documentation]
(@ref IntegralConstant) for `IntegralConstant`, which explains how to create
these objects and what you can expect from them.


@subsection tutorial-constexpr-side_effects Side effects

@note
You should be familiar with the Constant concept before reading this section.
Also note that this section contains somewhat advanced material, and it can
safely be skipped during a first read.

Let me ask a tricky question. Is the following code valid?

@code
    template <typename X>
    auto identity(X x) { return x; }

    static_assert(value(identity(bool_<true>)), "");
@endcode

The answer is "no", but the reason might not be obvious at first. Even more
puzzling is that the following code is perfectly valid:

@snippet example/tutorial/constant_side_effects.cpp pure

To understand why the compiler can't possibly evaluate the first assertion
at compile-time, notice that `identity` was not marked `constexpr` and
consider the following alternative (but valid) definition for `identity`:

@snippet example/tutorial/constant_side_effects.cpp impure_identity

The signature of the function did not change; the function could even have
been defined in a separate source file. However, it is now obvious that the
compiler can't evaluate that expression at compile-time. On the other hand,
when we write

@snippet example/tutorial/constant_side_effects.cpp impure

we're telling the compiler to perform those potential side effects during the
dynamic initialization phase! Then, we use `value` to return the compile-time
value associated to its argument. Also note that `value` takes a `const&` to
its argument; if it tried taking it by value, we would be reading from a
non-`constexpr` variable to do the copying, and that could hide side-effects.










@section tutorial-hetero Heterogeneity and generalized types

------------------------------------------------------------------------------
The purpose of Hana is to manipulate heterogeneous objects. However, there's
a fundamental question that we have not asked yet: does it even make sense to
manipulate heterogeneous objects?

For the sake of the explanation, let me make the following claim: a function
template that compiles with an argument of every possible type must have a
trivial implementation, in the sense that it must do nothing with its argument
except perhaps return it. Hence, for a function template to do something
interesting, it must fail to compile for some set of arguments. While I won't
try to prove that claim formally -- it might be false in some corner cases --,
think about it for a moment. Let's say I want to apply a function to each
element of an heterogeneous sequence:

@code
    for_each([x, y, z], f)
@endcode

The first observation is that `f` must have a templated call operator because
`x`, `y` and `z` have different types. The second observation is that without
knowing anything specific about the types of `x`, `y` and `z`, it is impossible
for `f` to do anything meaningful. For example, could it print its argument?
Of course not, since it does not know whether `std::cout << x` is well-formed!
In order to do something meaningful, the function has to put constraints on
its arguments; it has to define a domain which is more specific that the set
of all types, and hence it can't be _fully_ polymorphic, even if we do not have
a way to express this in C++ (right now). So while we're manipulating types
that are technically heterogeneous, they still conceptually need something in
common, or it wouldn't be possible to do anything meaningful with them. We'll
still say that we're manipulating heterogeneous objects, but always keep in
mind that the objects we manipulate share something, and are hence homogeneous
in _some way_.

Pushing this to the extreme, some type families represent exactly the same
entity, except they must have a different C++ type because the language
requires them to. For example, this is the case of `_tuple<...>`. In our
context, we would like to see `_tuple<int, int>` and `_tuple<int, long, float>`
as different representations for the same data structure (a "tuple"), but the
C++ language requires us to give them different types. In Hana, we associate
what we call a _generalized type_ (we also say _data type_ and sometimes
_gtype_) to each type family. A generalized type is simply a tag (like in
MPL or Fusion) which is associated to all the types in a family through the
`datatype` metafunction. For `_tuple<...>`, this generalized type is `Tuple`;
other constructs in Hana also follow this convention of naming their
generalized type with a capital letter.

Just like C++ templates are families of types that are parameterized by some
other type, it makes sense to speak of parameterized generalized types. A
parameterized _gtype_ is simply a _gtype_ which depends on other generalized
types. You might have seen it coming, but this is actually the case for
`_tuple`, whose _gtype_ can be seen as depending on the _gtype_ of the objects
it contains. However, take good note that __parameterized generalized types in
Hana only live at the documentation level__. While enforcing proper
parametricity would make the library more mathematically correct, I fear it
would also make it less usable given the lack of language support. Given a
parametric _gtype_ `F`, we use `F(T)` to denote the "application" of `F` to
another _gtype_ `T`. While this is analogous to "applying" a C++ template to
a type, we purposefully do not use the `F<T>` notation because parametric
gtypes are not necessarily templates in Hana and that would be more confusing
than helpful.

As an example, `_tuple<int, int>` conceptually has a gtype of `Tuple(int)`,
but its actual gtype (outside of the documentation) is just `Tuple`. What
about `_tuple<int, long>`? Well, `int` and `long` are embedded in the same
mathematical universe, so we could say that it's a `Tuple(Number)`, where
`Number` is some generalized type containing all the numeric types. What
about `_tuple<int, void>`? First, that won't compile. But why would you
create a sequence of objects that have nothing in common? What can you do
with that?

These generalized types are useful for several purposes, for example creating
a tuple with `make<Tuple>` and documenting pseudo-signatures for the functions
provided in this library. Another important role is to customize algorithms;
see the section on [tag-dispatching](@ref tutorial-extending-tag_dispatching)
for more information. Finally, you can also consult the reference of the
[datatype](@ref datatype) metafunction for details on how to specify the
generalized type of a family of types.

@todo
There is obviously a connection between generalized types and concepts.
I think that generalized types are concepts whose models are unique up
to a unique isomorphism. Still, it is necessary to distinguish between
isomorphic models when we want to provide an implementation or create an
object. If you see how it all fits together better than I do right now,
let me know.










@section tutorial-ext Integration with external libraries

------------------------------------------------------------------------------

@subsection tutorial-ext-std The standard library

@subsection tutorial-ext-fusion Boost.Fusion

@subsection tutorial-ext-mpl Boost.MPL










@section tutorial-extending Extending the library

------------------------------------------------------------------------------
Because of its modular design, Hana can be extended in a ad-hoc manner very
easily. Actually, all the functionality of the library is provided through
this ad-hoc customization mechanism.


@subsection tutorial-extending-tag_dispatching Tag dispatching

Tag dispatching is a generic programming technique for picking the right
implementation of a function depending on the type of the arguments passed
to the function. The usual mechanism for overriding a function's behavior
is overloading. Unfortunately, this mechanism is not always convenient when
dealing with families of related types having different C++ types, or with
objects of unspecified types as is often the case in Hana. For example,
consider trying to overload a function for all Boost.Fusion vectors:

@code
    template <typename ...T>
    void function(boost::fusion::vector<T...> v) {
        // whatever
    }
@endcode

If you know Boost.Fusion, then you probably know that it won't work. This is
because Boost.Fusion vectors are not necessarily specializations of the
`boost::fusion::vector` template. Fusion vectors also exist in numbered
forms, which are all of different types:

@code
    boost::fusion::vector1<T>
    boost::fusion::vector2<T, U>
    boost::fusion::vector3<T, U, V>
    ...
@endcode

This is an implementation detail required by the lack of variadic templates in
C++03 that leaks into the interface. This is unfortunate, but we need a way to
work around it. To do so, we use an infrastructure with three distinct
components:

1. A metafunction associating a single tag to every type in a family of
   related types. In Hana, we use the generalized type of an object,
   which is accessible through the `datatype` metafunction.

2. A function belonging to the public interface of the library, for which
   we'd like to be able to provide a customized implementation.

3. An implementation for the function, parameterized with the data type(s)
   of the argument(s) passed to the function.

When the public interface function is called, it will use the metafunction
on its argument(s) (or a subset thereof) to obtain their data type(s) and
redirect to the implementation associated to those data type(s). For example,
a basic setup for tag dispatching of a function that prints its argument to a
stream would look like:

@snippet tutorial/tag_dispatching.cpp setup

Then, if you want to customize the behavior of the `print` function for some
user defined family of types, you only need to specialize the `print_impl`
template for the tag representing the whole family of types:

@snippet tutorial/tag_dispatching.cpp customize


@subsection tutorial-extending-creating_concepts Creating new concepts










@section tutorial-header_organization Header organization

------------------------------------------------------------------------------
The library is designed to be modular while keeping the number of headers that
must be included to get basic functionality reasonably low. The structure of
the library was also intentionally kept simple, because we all love simplicity.

- `boost/hana.hpp`\n
  This is the master header of the library. It includes the whole public
  interface of the library except adapters for external libraries, which
  must be included separately.

- `boost/hana/`\n
  This is the main directory of the library containing the definitions of
  concepts and data types. A file of the form `boost/hana/[XXX].hpp` contains
  the definition for the concept or data type named `XXX`.

  - `boost/hana/core/`\n
    This subdirectory contains the machinery for tag-dispatching and other
    related utilities like `make` and `to`.

  - `boost/hana/fwd/`\n
    This subdirectory contains the forward declaration of every concept
    and data type in the library. Basically, `boost/hana/fwd/[XXX].hpp`
    is the forward declaration for the concept or data type named `XXX`.
    Also note that forward declarations for headers in `boost/hana/ext/`
    are not provided.

  - `boost/hana/functional/`\n
    This subdirectory contains various function objects that are often useful,
    but that do not necessarily belong to a concept.

  - `boost/hana/ext/`\n
    This directory contains adapters for external libraries. Only the strict
    minimum required to adapt the external components is included in these
    headers (e.g. a forward declaration). This means that the definition of
    the external component should still be included when one wants to use it.
    For example:
    @snippet example/tutorial/include_ext.cpp main

  - `boost/hana/detail/`\n
    This directory contains utilities required internally. Nothing in `detail/`
    is guaranteed to be stable, so you should not use it.










@section tutorial-using_the_reference Using the reference

------------------------------------------------------------------------------
You now have everything you need to start using the library. From here on,
mastering the library is only a matter of understanding and knowing how to
use the general purpose concepts and data types provided with it, which
is best done by looking at the reference documentation. At some point, you
will probably also want to create your own concepts and data types that fit
your needs better; go ahead, the library was intended to be used that way.

The structure of the reference (available in the menu to the left) goes as
follow:
  - @ref group-core\n
    Documentation for the core module, which contains everything needed to
    create concepts, data types and related utilities. This is relevant
    if you need to extend the library, but otherwise you can probably
    ignore this.

  - @ref group-functional\n
    General purpose function objects that are generally useful in a purely
    functional setting. These are currently not tied to any concept or data
    type.

  - @ref group-concepts\n
    Documentation for all the concepts provided with the library. Each concept:
    - Documents laws that are internal to this concept
    - Documents the concept(s) it is derived from, if any. In the documentation,
      we usually call those base concepts _superclasses_. Sometimes, a concept
      is powerful enough to provide a model of its superclass, or at least the
      implementation for some of its methods. When this is the case, the
      concept will document which superclass methods it provides, and how
      it does so. Also, it is sometimes possible that the model for a
      superclass is unique, in which case it can be provided automatically.
      When this happens, it will be documented but you don't have to do
      anything special to get that model.
    - Documents which methods must be implemented absolutely in order to
      model that concept.

  - @ref group-datatypes\n
    Documentation for all the data types provided with the library. Each
    data type documents the concept(s) it models, and how it does so. It
    also documents the methods tied to that data type but not to any concept,
    for example `make<Tuple>`.

  - @ref group-ext\n
    Documentation for all the adapters for external libraries. Basically, we
    assign a data type to some objects in external libraries and we document
    them as if they were normal data types provided by Hana.

  - @ref group-config\n
    Macros that can be used to tweak the global behavior of the library.

  - @ref group-assertions\n
    Macros to perform various types of assertions.

  - @ref group-details\n
    Implementation details. Don't go there.



@subsection tutorial-using_the_reference-pseudo Pseudo-code glossary

In the documentation, a simplified implementation of the documented object
is sometimes provided in pseudo-code. The reason is that the actual
implementation is sometimes contrived because of unimportant details
or language limitations. Here is an explanation of some terms that
appear in the pseudo-code:

- `forwarded(x)`\n
    Means that the object is forwarded optimally. This means that if `x` is a
    parameter, it is `std::forward`ed, and if it is a captured variable, it is
    moved from whenever the enclosing lambda is an rvalue.

    Also note that when `x` can be moved from, the statement
    `return forwarded(x);` in a function with `decltype(auto)` does not mean
    that an rvalue reference to `x` will be returned, which would create a
    dangling reference. Rather, it means that `x` is returned by value, the
    value being constructed with the `std::forward`ed `x`.

- `perfect-capture`\n
    This is used in lambdas to signify that the captured variables are
    initialized using perfect forwarding, as if `[x(forwarded(x))...]() { }`
    had been used.

- `decayed(T)`\n
    Represents a type `T` after decaying. This is basically equivalent to
    `std::decay_t<T>`.

- `tag-dispatched`\n
    This means that the documented method uses tag-dispatching, and hence
    the exact implementation depends on the model of the concept associated
    to the method.

- `unspecified-type`\n
    This is used to express the fact that the return-type of a function
    is unspecified, and hence you should not rely on it being anything
    special beyond what is documented. Normally, the concepts satisfied
    by the returned object will be specified, because otherwise that
    function wouldn't be very useful.




------------------------------------------------------------------------------
This finishes the tutorial part of the documentation. I hope you enjoy using
the library, and please leave feedback on GitHub so we can improve the library!

-- Louis










@section tutorial-rationales Rationales/FAQ

------------------------------------------------------------------------------
This section documents the rationale for some design choices. It also serves
as a FAQ for some (not so) frequently asked questions. If you think something
should be added to this list, open a GitHub issue and we'll consider either
improving the documentation or adding the question here.


1. __Why restrict usage of Boost and the standard library? After all, isn't this a (proposed) Boost library?__\n
There are several reasons for doing so. First, Hana is a very fundamental
library; we are basically reimplementing the core language and the standard
library with support for heterogeneous types. When you go through the code,
you quickly realize that you very rarely need other libraries, and that
almost everything must be implemented from scratch. Also, since Hana is very
fundamental, there is even more incentive for keeping the dependencies minimal,
because those dependencies will be handed down to the users.
Finally, one big reason for using Boost is that it is highly portable. As an
exception to the rule, this library only targets very recent compilers. Hence,
we can afford to depend on modern constructs and the portability brought by
using most Boost libraries is just dead weight.










<!-- Links -->
[Boost.Fusion]: http://www.boost.org/doc/libs/release/libs/fusion/doc/html/index.html
[Boost.MPL]: http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html
[C++Now]: http://cppnow.org
[constexpr_throw]: http://stackoverflow.com/a/8626450/627587
[GitHub.issues]: https://github.com/ldionne/hana/issues
[GOTW]: http://www.gotw.ca/gotw/index.htm
[GSoC]: http://www.google-melange.com/gsoc/homepage/google/gsoc2014
[MPL11]: http://github.com/ldionne/mpl11

[Wikipedia.C++14]: http://en.wikipedia.org/wiki/C%2B%2B14
[Wikipedia.CXX14_udl]: http://en.wikipedia.org/wiki/C%2B%2B11#User-defined_literals
[Wikipedia.generic_lambda]: http://en.wikipedia.org/wiki/C%2B%2B14#Generic_lambdas
[Wikipedia.variable_template]: http://en.wikipedia.org/wiki/C%2B%2B14#Variable_templates
[Wikipedia.generalized_constexpr]: http://en.wikipedia.org/wiki/C%2B%2B11#constexpr_.E2.80.93_Generalized_constant_expressions

*/

}} // end namespace boost::hana

#endif // !BOOST_HANA_HPP
