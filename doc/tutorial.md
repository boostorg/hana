@mainpage User Manual

@tableofcontents










@section tutorial-description Description

------------------------------------------------------------------------------
Hana is a header-only library for C++ metaprogramming suited for computations
on both types and values. The functionality it provides is a superset of what
is provided by the well established [Boost.MPL][] and [Boost.Fusion][]
libraries. By leveraging C++11/14 implementation techniques and idioms,
Hana boasts faster compilation times and runtime performance on par or better
than previous metaprogramming libraries, while noticeably increasing the level
of expressiveness in the process. Hana is easy to extend in a ad-hoc manner
and it provides out-of-the-box inter-operation with Boost.Fusion, Boost.MPL
and the standard library.










@section tutorial-installation Prerequisites and installation

------------------------------------------------------------------------------
To use Hana in your own project, just add the `include` directory to your
compiler's header search path and you are done. The library relies on a
C++14 compiler and standard library, but nothing else is required. Here is
a table of the current C++14 compilers/toolchains with comments regarding
support for Hana:

Compiler/Toolchain | Status
------------------ | ------
Clang >= 3.5.0     | Fully working; tested on each push to `master`
Xcode >= 6.3       | Fully working; locally tested now and then
GCC >= 5.1.0       | Almost working; waiting for the GCC team to fix a couple of C++14-related bugs

More specifically, Hana requires a compiler/standard library supporting the
following C++14 features (non-exhaustively):
- Generic lambdas
- Generalized `constexpr`
- Variable templates
- Automatically deduced return type
- All the C++14 type traits from the `<type_traits>` header










@section tutorial-introduction Introduction

------------------------------------------------------------------------------
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


@subsection tutorial-introduction-quadrants C++ computational quadrants

But to really understand what is Hana all about, it is essential to understand
the different types of computations in C++. We will focus our attention on
four different kinds of computations, even though a finer grained separation
would be possible. First, we have runtime computations, which are the usual
computations we use in C++. In that world, we have runtime containers,
runtime functions and runtime algorithms:

@snippet example/tutorial/introduction.cpp runtime

The usual toolbox for programming within this quadrant is the C++ standard
library, which provides reusable algorithms and containers operating at
runtime. Since C++11, a second kind of computation is possible: `constexpr`
computations. There, we have `constexpr` containers, `constexpr` functions
and `constexpr` algorithms:

@code{cpp}
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

@snippet example/tutorial/introduction.cpp heterogeneous

If manipulating heterogeneous containers seems overly weird to you, just think
of it as glorified `std::tuple` manipulation. In a C++03 world, the go-to
library for doing this kind of computation is [Boost.Fusion][], which provides
several data structures and algorithms to manipulate heterogeneous collections
of data. The fourth and last quadrant of computation that we'll be considering
here is the quadrant of type-level computations. In this quadrant, we have
type-level containers, type-level functions (usually called metafunctions)
and type-level algorithms. Here, everything operates on types: containers hold
types and metafunctions take types as arguments and return types as results.

@snippet example/tutorial/introduction.cpp type-level

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
Secondly, Hana provides a way of translating any type-level computation into its
equivalent heterogeneous computation and back, which allows the full machinery
of heterogeneous computations to be reused for type-level computations without
any code duplication. Of course, the biggest advantage of this unification is
seen by the user, as you will witness by yourself.










@section tutorial-quickstart Quick start

------------------------------------------------------------------------------
The goal of this section is to introduce the main concepts of the library
from a very high level and at a fairly rapid pace; don't worry if you don't
understand everything that's about to be thrown at you. However, this tutorial
assumes the reader is already at least _familiar_ with basic metaprogramming
and the [C++14 standard][C++14]. First, let's include the library:

@snippet example/tutorial/quickstart.cpp includes

Unless specified otherwise, the documentation assumes the above lines to be
present before examples and code snippets. Also note that finer grained
headers are provided and will be explained in the [Header organization]
(@ref tutorial-header_organization) section. For the purpose of the
quickstart, let's now include some additional headers and define some
lovely animal types that we'll need below:

@snippet example/tutorial/quickstart.cpp additional_setup

If you are reading this documentation, chances are you already know
`std::tuple` and `std::make_tuple`. Hana provides its own tuple and
`make_tuple`:

@snippet example/tutorial/quickstart.cpp animals

This creates a tuple, which is like an array, except that it can hold elements
with different types. Containers that can hold elements with different types
such as this are called heterogeneous containers. While the standard library
provides very few operations to manipulate `std::tuple`s, Hana provides several
operations and algorithms to manipulate its own tuples:

@snippet example/tutorial/quickstart.cpp algorithms

@note
`1_c` is a [C++14 user-defined literal][C++14.udl] creating a
[compile-time number](@ref tutorial-integral). These user-defined
literals are contained in the `boost::hana::literals` namespace,
hence the `using` directive.

Notice how we pass a [C++14 generic lambda][C++14.glambda] to `transform`;
this is required because the lambda will first be called with a `Fish`, then
a `Cat`, and finally a `Dog`, which all have different types. Hana provides
most of the algorithms provided by the C++ standard library, except they work
on tuples and related heterogeneous containers instead of `std::vector` &
friends. In addition to working with heterogeneous values, Hana makes it
possible to perform type-level computations with a natural syntax, all at
compile-time and with no overhead whatsoever. This compiles and does just
what you would expect:

@snippet example/tutorial/quickstart.cpp type-level

@note
`type_c<...>` is not a type! It is a [C++14 variable template][C++14.vtemplate]
yielding an object representing a type for Hana. This is explained in the
section on [type computations](@ref tutorial-type).

In addition to heterogeneous and compile-time sequences, Hana provides several
features to make your metaprogramming nightmares a thing of the past. For
example, one can check for the existence of a struct member with one easy
line instead of relying on [clunky SFINAE hacks][SO.sfinae]:

@snippet example/tutorial/quickstart.cpp has_name

Writing a serialization library? Stop crying, we've got you covered.
Reflection can be added to user-defined types very easily. This allows
iterating over the members of a user-defined type, querying members with
a programmatic interface and much more, without any runtime overhead:

@snippet example/tutorial/quickstart.cpp serialization

That's cool, but I can already hear you complaining about incomprehensible
error messages. However, it turns out Hana was built for humans, not
professional template metaprogrammers, and this shows. Let's intentionally
screw up and see what kind of mess is thrown at us. First, the mistake:

@snippet example/tutorial/quickstart.cpp screw_up

Now, the punishment:

@code{cpp}
error: static_assert failed "hana::for_each(xs, f) requires xs to be Foldable"
            static_assert(_models<Foldable, S>{},
            ^             ~~~~~~~~~~~~~~~~~~~~~~
note: in instantiation of function template specialization
      'boost::hana::_for_each::operator()<
        std::__1::basic_ostream<char, std::__1::char_traits<char> > &,
        (lambda at [snip]/example/tutorial/quickstart.cpp:70:16)>' requested here
  for_each(os, [&](auto member) {
  ^
note: in instantiation of function template specialization
      'main()::(anonymous class)::operator()<Person>' requested here
serialize(std::cout, john);
         ^
@endcode

Not that bad, right? However, since small examples are very good to show off
without actually doing something useful, let's examine a real world example.


@subsection tutorial-quickstart-any A real world example

In this section our goal will be to implement a kind of `switch` statement
able to process `boost::any`s. Given a `boost::any`, the goal is to dispatch
to the function associated to the dynamic type of the `any`:

@snippet example/tutorial/quickstart.switchAny.cpp usage

@note
In the documentation, we will often use the `s` suffix on string literals to
create `std::string`s without syntactic overhead. This is a standard-defined
[C++14 user-defined literal][C++14.udl].

Since the any holds a `char`, the second function is called with the `char`
inside it. If the `any` had held an `int` instead, the first function would
have been called with the `int` inside it. When the dynamic type of the `any`
does not match any of the covered cases, the `%default_` function is called
instead. Finally, the result of the `switch` is the result of calling the
function associated to the `any`'s dynamic type. The type of that result is
inferred to be the common type of the result of all the provided functions:

@snippet example/tutorial/quickstart.switchAny.cpp result_inference

We'll now look at how this utility can be implemented using Hana. The first
step is to associate each type to a function. To do so, we represent each
`case_` as a `hana::pair` whose first element is a type and whose second
element is a function. Furthermore, we (arbitrarily) decide to represent the
`%default_` case as a `hana::pair` mapping a dummy type to a function:

@snippet example/tutorial/quickstart.switchAny.cpp cases

To provide the interface we showed above, `switch_` will have to return a
function taking the cases. In other words, `switch_(a)` must be a function
taking any number of cases (which are `hana::pair`s), and performing the logic
to dispatch `a` to the right function. This can easily be achieved by having
`switch_` return a C++14 generic lambda:

@code{cpp}
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    // ...
  };
}
@endcode

However, since parameter packs are not very flexible, we'll put the cases
into a tuple so we can manipulate them:

@code{cpp}
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = hana::make_tuple(cases_...);
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

@code{cpp}
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = hana::make_tuple(cases_...);

    auto default_ = hana::find_if(cases, [](auto const& c) {
      return hana::first(c) == hana::type_c<default_t>;
    });

    // ...
  };
}
@endcode

`find_if` takes a `tuple` and a predicate, and returns the first element of
the tuple which satisfies the predicate. The result is returned as a
`hana::optional`, which is very similar to a `std::optional`, except
whether that optional value is empty or not is known at compile-time. If the
predicate is not satisfied for any element of the `tuple`, `find_if` returns
`nothing` (an empty value). Otherwise, it returns `just(x)` (a non-empty value),
where `x` is the first element satisfying the predicate. Unlike predicates
used in STL algorithms, the predicate used here must be generic because the
tuple's elements are heterogeneous. Furthermore, that predicate must return
what Hana calls an `IntegralConstant`, which means that the predicate's result
must be known at compile-time. These details are explained in the section on
[cross-phase algorithms](@ref tutorial-algorithms-cross_phase). Inside the
predicate, we simply compare the type of the case's first element to
`type_c<default_t>`. If you recall that we were using `hana::pair`s to encode
cases, this simply means that we're finding the default case among all of the
provided cases. But what if no default case was provided? We should fail at
compile-time, of course!

@code{cpp}
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = hana::make_tuple(cases_...);

    auto default_ = hana::find_if(cases, [](auto const& c) {
      return hana::first(c) == hana::type_c<default_t>;
    });
    static_assert(default_ != hana::nothing,
      "switch is missing a default_ case");

    // ...
  };
}
@endcode

Notice how we can use `static_assert` on the result of the comparison with
`nothing`, even though `%default_` is a non-`constexpr` object? Boldly, Hana
makes sure that no information that's known at compile-time is lost to the
runtime, which is clearly the case of the presence of a `%default_` case.
The next step is to gather the set of non-default cases. To achieve this, we
use the `filter` algorithm, which keeps only the elements of the sequence
satisfying the predicate:

@code{cpp}
template <typename Any>
auto switch_(Any& a) {
  return [&a](auto ...cases_) {
    auto cases = hana::make_tuple(cases_...);

    auto default_ = hana::find_if(cases, [](auto const& c) {
      return hana::first(c) == hana::type_c<default_t>;
    });
    static_assert(default_ != hana::nothing,
      "switch is missing a default_ case");

    auto rest = hana::filter(cases, [](auto const& c) {
      return hana::first(c) != hana::type_c<default_t>;
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

@snippet example/tutorial/quickstart.switchAny.cpp switch_

`unpack` takes a `tuple` and a function, and calls the function with the content
of the `tuple` as arguments. The result of `unpack` is the result of calling that
function. In our case, the function is a generic lambda which in turn calls the
`process` function. Our reason for using `unpack` here was to turn the `rest`
tuple into a parameter pack of arguments, which are easier to process recursively
than tuples. Before we move on to the `process` function, it is worthwhile to
explain what `second(*%default_)` is all about. As we explained earlier,
`%default_` is an optional value. Like `std::optional`, this optional value
overloads the dereference operator (and the arrow operator) to allow accessing
the value inside the `optional`. If the optional is empty (`nothing`), a
compile-time error is triggered. Since we know `%default_` is not empty (we
checked that just above), what we're doing is simply pass the function
associated to the default case to the `process` function. We're now ready
for the final step, which is the implementation of the `process` function:

@snippet example/tutorial/quickstart.switchAny.cpp process

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
Pretty simple, wasn't it? Here's the final solution:

@snippet example/tutorial/quickstart.switchAny.cpp full

That's it for the quick start! This example only introduced a couple of useful
algorithms (`find_if`, `filter`, `unpack`) and heterogeneous containers
(`tuple`, `optional`), but rest assured that there is much more. The next
sections of the tutorial gradually introduce general concepts pertaining to
Hana in a friendly way, but you may use the following cheatsheet for quick
reference if you want to start coding right away. This cheatsheet contains
the most frequently used algorithms and containers, along with a short
description of what each of them does.


@subsection tutorial-quickstart-cheatsheet Cheatsheet

### Remarks
- Most algorithms work on both types and values (see the section on
  [type computations](@ref tutorial-type))
- Algorithms always return their result as a new container; no in-place
  mutation is ever performed (see the section on [algorithms]
  (@ref tutorial-algorithms))
- All algorithms are `constexpr` function objects


container          | description
:----------------- | :----------
<code>[tuple](@ref boost::hana::tuple)</code>                         | General purpose index-based heterogeneous sequence with a fixed length. Use this as a `std::vector` for heterogeneous objects.
<code>[optional](@ref boost::hana::optional)</code>                   | Represents an optional value, i.e. a value that can be empty. This is a bit like `std::optional`, except that the emptiness is known at compile-time.
<code>[map](@ref boost::hana::map)</code>                             | Unordered associative array mapping (unique) compile-time entities to arbitrary objects. This is like `std::unordered_map` for heterogeneous objects.
<code>[set](@ref boost::hana::set)</code>                             | Unordered container holding unique keys that must be compile-time entities. This is like `std::unordered_set` for heterogeneous objects.
<code>[range](@ref boost::hana::range)</code>                         | Represents an interval of compile-time numbers. This is like `std::integer_sequence`, but better.
<code>[pair](@ref boost::hana::pair)</code>                           | Container holding two heterogeneous objects. Like `std::pair`, but compresses the storage of empty types.
<code>[string](@ref boost::hana::string)</code>                       | Compile-time string.
<code>[type](@ref boost::hana::type)</code>                           | Container representing a C++ type. This is the root of the unification between types and values, and is of interest for MPL-style computations (type-level computations).
<code>[integral_constant](@ref boost::hana::integral_constant)</code> | Represents a compile-time number. This is very similar to `std::integral_constant`, except that `hana::integral_constant` also defines operators and more syntactic sugar.
<code>[lazy](@ref boost::hana::lazy)</code>                           | Encapsulates a lazy value or computation.
<code>[basic_tuple](@ref boost::hana::basic_tuple)</code>             | Stripped-down version of `hana::tuple`. Not standards conforming, but more compile-time efficient.


function                                                                                         | description
:------------------------------------------                                                      | :----------
<code>[adjust](@ref boost::hana::Functor::adjust)(sequence, value, f)</code>                     | Apply a function to each element of a sequence that compares equal to some value and return the result.
<code>[adjust_if](@ref boost::hana::Functor::adjust_if)(sequence, predicate, f)</code>           | Apply a function to each element of a sequence satisfying some predicate and return the result.
<code>{[all](@ref boost::hana::Searchable::all),[any](@ref boost::hana::Searchable::any),[none](@ref boost::hana::Searchable::none)}(sequence)</code> | Returns whether all/any/none of the elements of a sequence are true-valued.
<code>{[all](@ref boost::hana::Searchable::all_of),[any](@ref boost::hana::Searchable::any_of),[none](@ref boost::hana::Searchable::none_of)}_of(sequence, predicate)</code> | Returns whether all/any/none of the elements of the sequence satisfy some predicate.
<code>[append](@ref boost::hana::MonadPlus::append)(sequence, value)</code>                      | Append an element to a sequence.
<code>[at](@ref boost::hana::Iterable::at)(sequence, index)</code>                               | Returns the n-th element of a sequence. The index must be an `IntegralConstant`.
<code>[back](@ref boost::hana::Iterable::back)(sequence)</code>                                  | Returns the last element of a non-empty sequence.
<code>[concat](@ref boost::hana::MonadPlus::concat)(sequence1, sequence2)</code>                 | Concatenate two sequences.
<code>[contains](@ref boost::hana::Searchable::contains)(sequence, value)</code>                 | Returns whether a sequence contains the given object.
<code>[count](@ref boost::hana::Foldable::count)(sequence, value)</code>                         | Returns the number of elements that compare equal to the given value.
<code>[count_if](@ref boost::hana::Foldable::count_if)(sequence, predicate)</code>               | Returns the number of elements that satisfy the predicate.
<code>[drop_front](@ref boost::hana::Iterable::drop_front)(sequence[, n])</code>                 | Drop the first `n` elements from a sequence, or the whole sequence if `length(sequence) <= n`. `n` must be an `IntegralConstant`. When not provided, `n` defaults to 1.
<code>[drop_front_exactly](@ref boost::hana::Iterable::drop_front_exactly)(sequence[, n])</code> | Drop the first `n` elements from a sequence. `n` must be an `IntegralConstant` and the sequence must have at least `n` elements. When not provided, `n` defaults to 1.
<code>[drop_back](@ref boost::hana::Sequence::drop_back)(sequence[, n])</code>                   | Drop the last `n` elements from a sequence, or the whole sequence if `length(sequence) <= n`. `n` must be an `IntegralConstant`. When not provided, `n` defaults to 1.
<code>[drop_while](@ref boost::hana::Iterable::drop_while)(sequence, predicate)</code>           | Drops elements from a sequence while a predicate is satisfied. The predicate must return an `IntegralConstant`.
<code>[fill](@ref boost::hana::Functor::fill)(sequence, value)</code>                            | Replace all the elements of a sequence with some value.
<code>[filter](@ref boost::hana::MonadPlus::filter)(sequence, predicate)</code>                  | Remove all the elements that do not satisfy a predicate. The predicate must return an `IntegralConstant`.
<code>[find](@ref boost::hana::Searchable::find)(sequence, value)</code>                         | Find the first element of a sequence which compares equal to some value and return `just` it, or return `nothing`. See `hana::optional`.
<code>[find_if](@ref boost::hana::Searchable::find_if)(sequence, predicate)</code>               | Find the first element of a sequence satisfying the predicate and return `just` it, or return `nothing`. See `hana::optional`.
<code>[flatten](@ref boost::hana::Monad::flatten)(sequence)</code>                               | Flatten a sequence of sequences, a bit like `std::tuple_cat`.
<code>[fold_left](@ref boost::hana::Foldable::fold_left)(sequence[, state], f)</code>            | Accumulates the elements of a sequence from the left, optionally with a provided initial state.
<code>[fold_right](@ref boost::hana::Foldable::fold_right)(sequence[, state], f)</code>          | Accumulates the elements of a sequence from the right, optionally with a provided initial state.
<code>[fold](@ref boost::hana::Foldable::fold)(sequence[, state], f)</code>                      | Equivalent to `fold_left`; provided for consistency with Boost.MPL and Boost.Fusion.
<code>[for_each](@ref boost::hana::Foldable::for_each)(sequence, f)</code>                       | Call a function on each element of a sequence. Returns `void`.
<code>[front](@ref boost::hana::Iterable::front)(sequence)</code>                                | Returns the first element of a non-empty sequence.
<code>[group](@ref boost::hana::Sequence::group)(sequence[, predicate])</code>                   | %Group adjacent elements of a sequence which all satisfy (or all do not satisfy) some predicate. The predicate defaults to equality, in which case the elements must be `Comparable`.
<code>[insert](@ref boost::hana::Sequence::insert)(sequence, index, element)</code>              | Insert an element at a given index. The index must be an `IntegralConstant`.
<code>[insert_range](@ref boost::hana::Sequence::insert_range)(sequence, index, elements)</code> | Insert a sequence of elements at a given index. The index must be an `IntegralConstant`.
<code>[is_empty](@ref boost::hana::Iterable::is_empty)(sequence)</code>                          | Returns whether a sequence is empty as an `IntegralConstant`.
<code>[length](@ref boost::hana::Foldable::length)(sequence)</code>                              | Returns the length of a sequence as an `IntegralConstant`.
<code>[lexicographical_compare](@ref boost::hana::Iterable::lexicographical_compare)(sequence1, sequence2[, predicate])</code> | Performs a lexicographical comparison of two sequences, optionally with a custom predicate, by default with `hana::less`.
<code>[maximum](@ref boost::hana::Foldable::maximum)(sequence[, predicate])</code>               | Returns the greatest element of a sequence, optionally according to a predicate. The elements must be `Orderable` if no predicate is provided.
<code>[minimum](@ref boost::hana::Foldable::minimum)(sequence[, predicate])</code>               | Returns the smallest element of a sequence, optionally according to a predicate. The elements must be `Orderable` if no predicate is provided.
<code>[partition](@ref boost::hana::Sequence::partition)(sequence, predicate)</code>             | Partition a sequence into a pair of elements that satisfy some predicate, and elements that do not satisfy it.
<code>[prepend](@ref boost::hana::MonadPlus::prepend)(sequence, value)</code>                    | Prepend an element to a sequence.
<code>[remove](@ref boost::hana::MonadPlus::remove)(sequence, value)</code>                      | Remove all the elements that are equal to a given value.
<code>[remove_at](@ref boost::hana::Sequence::remove_at)(sequence, index)</code>                 | Remove the element at the given index. The index must be an `IntegralConstant`.
<code>[remove_if](@ref boost::hana::MonadPlus::remove_if)(sequence, predicate)</code>            | Remove all the elements that satisfy a predicate. The predicate must return an `IntegralConstant`.
<code>[remove_range](@ref boost::hana::Sequence::remove_range)(sequence, from, to)</code>        | Remove the elements at indices in the given `[from, to)` half-open interval. The indices must be `IntegralConstant`s.
<code>[replace](@ref boost::hana::Functor::replace)(sequence, oldval, newval)</code>             | Replace the elements of a sequence that compare equal to some value by some other value.
<code>[replace_if](@ref boost::hana::Functor::replace_if)(sequence, predicate, newval)</code>    | Replace the elements of a sequence that satisfy some predicate by some value.
<code>[reverse](@ref boost::hana::Sequence::reverse)(sequence)</code>                            | Reverse the order of the elements in a sequence.
<code>[reverse_fold](@ref boost::hana::Foldable::reverse_fold)(sequence[, state], f)</code>      | Equivalent to `fold_right`; provided for consistency with Boost.MPL and Boost.Fusion.
<code>[size](@ref boost::hana::Foldable::size)(sequence)</code>                                  | Equivalent to `length`; provided for consistency with the C++ standard library.
<code>[slice](@ref boost::hana::Sequence::slice)(sequence, from, to)</code>                      | Returns the elements of a sequence at indices contained in `[from, to)`.
<code>[sort](@ref boost::hana::Sequence::sort)(sequence[, predicate])</code>                     | Sort (stably) the elements of a sequence, optionally according to a predicate. The elements must be `Orderable` if no predicate is provided.
<code>[subsequence](@ref boost::hana::Sequence::subsequence)(sequence, indices)</code>           | Returns the elements of a sequence at the `indices` in the given sequence.
<code>[tail](@ref boost::hana::Iterable::tail)(sequence)</code>                                  | Returns all the elements except the first one. Analogous to `pop_front`.
<code>[take](@ref boost::hana::Sequence::take)(sequence, number)</code>                          | Take the first n elements of a sequence, or the whole sequence if `length(sequence) <= n`. n must be an `IntegralConstant`.
<code>[take_while](@ref boost::hana::Sequence::take_while)(sequence, predicate)</code>           | Take elements of a sequence while some predicate is satisfied, and return that.
<code>[transform](@ref boost::hana::Functor::transform)(sequence, f)</code>                      | Apply a function to each element of a sequence and return the result.
<code>[unique](@ref boost::hana::Sequence::unique)(sequence[, predicate])</code>                 | Removes all consecutive duplicates from a sequence. The predicate defaults to equality, in which case the elements must be `Comparable`.
<code>[unpack](@ref boost::hana::Foldable::unpack)(sequence, f)</code>                           | Calls a function with the contents of a sequence. Equivalent to `f(x1, ..., xN)`.
<code>[zip](@ref boost::hana::Sequence::zip)(s1, ..., sN)</code>                                 | Zip `N` sequences into a sequence of tuples. All the sequences must have the same length.
<code>[zip_shortest](@ref boost::hana::Sequence::zip_shortest)(s1, ..., sN)</code>               | Zip `N` sequences into a sequence of tuples. The resulting sequence has the length of the shortest input sequence.
<code>[zip_with](@ref boost::hana::Sequence::zip_with)(f, s1, ..., sN)</code>                    | Zip `N` sequences with a `N`-ary function. All the sequences must have the same length.
<code>[zip_shortest_with](@ref boost::hana::Sequence::zip_shortest_with)(f, s1, ..., sN)</code>  | Zip `N` sequences with a `N`-ary function. The resulting sequence has the length of the shortest input sequence.










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
`static_assert`              | Assertion on a `constexpr` condition. This is stronger than `BOOST_HANA_CONSTEXPR_CHECK` in that it requires the condition to be a constant expression, and it hence assures that the algorithms used in the expression are `constexpr`-friendly.
`BOOST_HANA_CONSTANT_CHECK`  | Assertion on a boolean `IntegralConstant`. This assertion provides the strongest form of guarantee, because an `IntegralConstant` can be converted to a `constexpr` value even if it is not `constexpr` itself. Note that in reality, any compile-time `Logical` is accepted by this macro, which is more general than requiring a boolean `IntegralConstant`.










@section tutorial-integral Compile-time numbers

------------------------------------------------------------------------------
This section introduces the important notion of `IntegralConstant` and the
philosophy behind Hana's metaprogramming paradigm. Let's start with a rather
odd question. What is an `integral_constant`?

@code{cpp}
template<class T, T v>
struct integral_constant {
  static constexpr T value = v;
  typedef T value_type;
  typedef integral_constant type;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};
@endcode

@note
If this is totally new to you, you might want to take a look at the
[documentation][C++14.ice] for `std::integral_constant`.

One valid answer is that `integral_constant` represents a type-level
encoding of a number, or more generally any object of an integral type.
For illustration, we could define a successor function on numbers in that
representation very easily by using a template alias:

@code{cpp}
template <typename N>
using succ = integral_constant<int, N::value + 1>;

using one = integral_constant<int, 1>;
using two = succ<one>;
using three = succ<two>;
// ...
@endcode

This is the way `integral_constant`s are usually thought of; as _type-level_
entities that can be used for template metaprogramming. Another way to see
an `integral_constant` is as a runtime object representing a `constexpr` value
of an integral type:

@code{cpp}
auto one = integral_constant<int, 1>{};
@endcode

Here, while `one` is not marked as `constexpr`, the abstract value it holds
(a `constexpr 1`) is still available at compile-time, because that value is
encoded in the type of `one`. Indeed, even if `one` is not `constexpr`, we
can use `decltype` to retrieve the compile-time value it represents:

@code{cpp}
auto one = integral_constant<int, 1>{};
constexpr int one_constexpr = decltype(one)::value;
@endcode

But why on earth would we want to consider `integral_constant`s as objects
instead of type-level entities? To see why, consider how we could now
implement the same successor function as before:

@code{cpp}
template <typename N>
auto succ(N) {
  return integral_constant<int, N::value + 1>{};
}

auto one = integral_constant<int, 1>{};
auto two = succ(one);
auto three = succ(two);
// ...
@endcode

Did you notice anything new? The difference is that instead of implementing
`succ` at the type-level with a template alias, we're now implementing it at
the value-level with a template function. Furthermore, we can now perform
compile-time arithmetic using the same syntax as that of normal C++. This
way of seeing compile-time entities as objects instead of types is the key
to Hana's expressive power.


@subsection tutorial-integral-arithmetic Compile-time arithmetic

The MPL defines [arithmetic operators][MPL.arithmetic] that can be used to do
compile-time computations with `integral_constant`s. A typical example of such
an operation is `plus`, which is implemented roughly as:

@code{cpp}
template <typename X, typename Y>
struct plus {
  using type = integral_constant<
    decltype(X::value + Y::value),
    X::value + Y::value
  >;
};

using three = plus<integral_constant<int, 1>,
                   integral_constant<int, 2>>::type;
@endcode

By viewing `integral_constant`s as objects instead of types, the translation
from a metafunction to a function is very straightforward:

@code{cpp}
template <typename V, V v, typename U, U u>
constexpr auto
operator+(integral_constant<V, v>, integral_constant<U, u>)
{ return integral_constant<decltype(v + u), v + u>{}; }

auto three = integral_constant<int, 1>{} + integral_constant<int, 2>{};
@endcode

It is very important to emphasize the fact that this operator does not return
a normal integer. Instead, it returns a value-initialized object whose type
contains the result of the addition. The only useful information contained in
that object is actually in its type, and we're creating an object because it
allows us to use this nice value-level syntax. It turns out that we can make
this syntax even better by using a [C++14 variable template][C++14.vtemplate]
to simplify the creation of an `integral_constant`:

@code{cpp}
template <int i>
constexpr integral_constant<int, i> int_c{};

auto three = int_c<1> + int_c<2>;
@endcode

Now we're talking about a visible gain in expressiveness over the initial
type-level approach, aren't we? But there's more; we can also use
[C++14 user defined literals][C++14.udl] to make this process even simpler:

@code{cpp}
template <char ...digits>
constexpr auto operator"" _c() {
  // parse the digits and return an integral_constant
}

auto three = 1_c + 3_c;
@endcode

Hana provides its own `integral_constant`s, which define arithmetic operators
just like we showed above. Hana also provides variable templates to easily
create different kinds of `integral_constant`s: `int_c`, `long_c`, `bool_c`,
etc... This allows you to omit the trailing `{}` braces otherwise required to
value-initialize these objects. Of course, the `_c` suffix is also provided;
it is part of the `hana::literals` namespace, and you must import it into
your namespace before using it:

@code{cpp}
using namespace hana::literals;

auto three = 1_c + 3_c;
@endcode

This way, you may do compile-time arithmetic without having to struggle with
awkward type-level idiosyncrasies, and your coworkers will now be able to
understand what's going on.


@subsection tutorial-integral-distance Example: Euclidean distance

To illustrate how good it gets, let's implement a function computing a 2-D
euclidean distance at compile-time. As a reminder, the euclidean distance of
two points in the 2-D plane is given by

@f[
  \mathrm{distance}\left((x_1, y_1), (x_2, y_2)\right)
      := \sqrt{(x_1 - x_2)^2 + (y_1 - y_2)^2}
@f]

First, here's how it looks like with a type-level approach (using the MPL):

@snippet example/tutorial/integral.cpp distance-mpl

Yeah... Now, let's implement it with the value-level approach presented above:

@snippet example/tutorial/integral.cpp distance-hana

This version looks arguably cleaner. However, this is not all. Notice how the
`distance` function looks exactly as the one you would have written for
computing the euclidean distance on dynamic values? Indeed, because we're
using the same syntax for dynamic and compile-time arithmetic, generic
functions written for one will work for both!

@snippet example/tutorial/integral.cpp distance-dynamic

__Without changing any code__, we can use our `distance` function on runtime
values and everything just works. Now that's DRY.


@subsection tutorial-integral-more Why stop here?

Why should we limit ourselves to arithmetic operations? When you start
considering `IntegralConstant`s as objects, it becomes sensible to augment
their interface with more functions that are generally useful. For example,
Hana's `IntegralConstant`s define a `times` member function that can be used
to invoke a function a certain number of times, which is especially useful
for loop unrolling:

@code{cpp}
__attribute__((noinline)) void f() { }

int main() {
    hana::int_c<10>.times(f);
}
@endcode

In the above code, the 10 calls to `f` are expanded at compile-time. In other
words, this is equivalent to writing

@code{cpp}
f(); f(); ... f(); // 10 times
@endcode

Another nice use of `IntegralConstant`s is to define good-looking operators
for indexing heterogeneous sequences. Whereas `std::tuple` must be accessed
with `std::get`, `hana::tuple` can be accessed using the familiar `operator[]`
used for standard library containers:

@code{cpp}
auto values = hana::make_tuple(1, 'x', 3.4f);
char x = values[1_c];
@endcode

How this works is very simple. Basically, `hana::tuple` defines an `operator[]`
taking an `IntegralConstant` instead of a normal integer, in a way similar to

@code{cpp}
template <typename N>
constexpr decltype(auto) operator[](N const&) {
  return std::get<N::value>(*this);
}
@endcode

This is the end of the section on `IntegralConstant`s. This section introduced
the feel behind Hana's new way of metaprogramming; if you liked what you've
seen so far, the rest of this tutorial should feel just like home.










@section tutorial-type Type computations

------------------------------------------------------------------------------
At this point, if you are interested in doing type-level computations as with
the MPL, you might be wondering how is Hana going to help you. Do not despair.
Hana provides a way to perform type-level computations with a great deal of
expressiveness by representing types as values, just like we represented
compile-time numbers as values. This is a completely new way of approaching
metaprogramming, and you should try to set your old MPL habits aside for a bit
if you want to become proficient with Hana.

However, please be aware that modern C++ features like [auto-deduced return type]
[C++14.auto_rt] remove the need for type computations in many cases. Hence,
before even considering to do a type computation, you should ask yourself
whether there's a simpler way to achieve what you're trying to achieve. In
most cases, the answer will be yes. However, when the answer is no, Hana will
provide you with nuclear-strength facilities to do what needs to be done.


@subsection tutorial-type-objects Types as objects

The key behind Hana's approach to type-level computations is essentially the
same as the approach to compile-time arithmetic. Basically, the idea is to
represent compile-time entities as objects by wrapping them into some kind of
container. For `IntegralConstant`s, the compile-time entities were constant
expressions of an integral type and the wrapper we used was `integral_constant`.
In this section, the compile-time entities will be types and the wrapper we'll
be using is called `type`. Just like we did for `IntegralConstant`s, let's
start by defining a dummy template that could be used to represent a type:

@code{cpp}
template <typename T>
struct basic_type {
  // empty (for now)
};

basic_type<int> Int{};
basic_type<char> Char{};
// ...
@endcode

@note
We're using the name `basic_type` here because we're only building a naive
version of the actual functionality provided by Hana.

While this may seem completely useless, it is actually enough to start writing
metafunctions that look like functions. Indeed, consider the following
alternate implementations of `std::add_pointer` and `std::is_pointer`:

@code{cpp}
template <typename T>
constexpr basic_type<T*> add_pointer(basic_type<T> const&)
{ return {}; }

template <typename T>
constexpr auto is_pointer(basic_type<T> const&)
{ return hana::bool_c<false>; }

template <typename T>
constexpr auto is_pointer(basic_type<T*> const&)
{ return hana::bool_c<true>; }
@endcode

We've just written metafunctions that look like functions, just like we wrote
compile-time arithmetic metafunctions as heterogeneous C++ operators in the
previous section. Here's how we can use them:

@code{cpp}
basic_type<int> t{};
auto p = add_pointer(t);
BOOST_HANA_CONSTANT_CHECK(is_pointer(p));
@endcode

Notice how we can now use a normal function call syntax to perform type-level
computations? This is analogous to how using values for compile-time numbers
allowed us to use normal C++ operators to perform compile-time computations.
Like we did for `integral_constant`, we can also go one step further and use
C++14 variable templates to provide syntactic sugar for creating types:

@code{cpp}
template <typename T>
constexpr basic_type<T> type_c{};

auto t = type_c<int>;
auto p = add_pointer(t);
BOOST_HANA_CONSTANT_CHECK(is_pointer(p));
@endcode

@note
This is not exactly how the `hana::type_c` variable template is implemented
because of some subtleties; things were dumbed down here for the sake of the
explanation. Please check the reference for `hana::type` to know exactly
what you can expect from a `hana::type_c<...>`.


@subsection tutorial-type-benefits Benefits of this representation

But what does that buy us? Well, since a `type_c<...>` is just an object, we
can store it in a heterogeneous sequence like a tuple, we can move it around
and pass it to (or return it from) functions, and we can do basically anything
else that requires an object:

@snippet example/tutorial/type.cpp tuple

@note
Writing `make_tuple(type_c<T>...)` can be annoying when there are several types.
For this reason, Hana provides the `tuple_t<T...>` variable template, which is
syntactic sugar for `make_tuple(type_c<T>...)`.

Also, notice that since the above tuple is really just a normal heterogeneous
sequence, we can apply heterogeneous algorithms on that sequence just like we
could on a tuple of `int`s, for example. Furthermore, since we're just
manipulating objects, we can now use the full language instead of just the
small subset available at the type-level. For example, consider the task of
removing all the types that are not a reference or a pointer from a sequence
of types. With the MPL, we would have to use a placeholder expression to
express the predicate, which is clunky:

@snippet example/tutorial/type.cpp filter.MPL

Now, since we're manipulating objects, we can use the full language and use a
generic lambda instead, which leads to much more readable code:

@snippet example/tutorial/type.cpp filter.Hana

Since Hana handles all heterogeneous containers uniformly, this approach of
representing types as values also has the benefit that a single library is
now needed for both heterogeneous computations and type level computations.
Indeed, whereas we would normally need two different libraries to perform
almost identical tasks, we now need a single library. Again, consider the
task of filtering a sequence with a predicate. With MPL and Fusion, this
is what we must do:

@snippet example/tutorial/type.cpp single_library.then

With Hana, a single library is required (notice how we use the same `filter`
algorithm and the same container):

@snippet example/tutorial/type.cpp single_library.Hana

But that is not all. Indeed, having a unified syntax for type-level and
value-level computations allows us to achieve greater consistency in the
interface of heterogeneous containers. For example, consider the simple
task of creating a heterogeneous map associating types to values, and then
accessing an element of it. With Fusion, what's happening is far from obvious
to the untrained eye:

@snippet example/tutorial/type.cpp make_map.Fusion

However, with a unified syntax for types and values, the same thing becomes
much clearer:

@snippet example/tutorial/type.cpp make_map.Hana


@subsection tutorial-type-working Working with this representation

So far, we can represent types as values and perform type-level computations
on those objects using the usual C++ syntax. This is nice, but it is not very
useful because we have no way to get back a normal C++ type from an object
representation. For example, how could we declare a variable whose type is the
result of a type computation?

@code{cpp}
auto t = add_pointer(hana::type_c<int>); // could be a complex type computation
using T = the-type-represented-by-t;

T var = ...;
@endcode

Right now, there is no easy way to do it. To make this easier to achieve, we
enrich the interface of the `basic_type` container that we defined above.
Instead of being an empty `struct`, we now define it as

@code{cpp}
template <typename T>
struct basic_type {
  using type = T;
};
@endcode

@note
This is equivalent to making `basic_type` a metafunction in the MPL sense.

This way, we can use `decltype` to easily access the actual C++ type
represented by a `type_c<...>` object:

@code{cpp}
auto t = add_pointer(hana::type_c<int>);
using T = decltype(t)::type; // fetches basic_type<T>::type

T var = ...;
@endcode

In general, doing type-level metaprogramming with Hana is a three step process:

1. Represent types as objects by wrapping them with `hana::type_c<...>`
2. Perform type transformations with value syntax
3. Unwrap the result with `decltype(...)::%type`

Now, you must be thinking that this is incredibly cumbersome. In reality, it
is very manageable for several reasons. First, this wrapping and unwrapping
only needs to happen at some very thin boundaries.

@code{cpp}
auto t = hana::type_c<T>;
auto result = huge_type_computation(t);
using Result = decltype(result)::type;
@endcode

Furthermore, since you get the advantage of working with objects (without
having to wrap/unwrap) inside the computation, the cost of wrapping and
unwrapping is amortized on the whole computation. Hence, for complex type
computations, the syntactic noise of this three step process quickly becomes
negligible in light of the expressiveness gain of working with values inside
that computation. Also, using values instead of types means that we can avoid
typing `typename` and `template` all around the place, which accounted for a
lot of syntactic noise in classic metaprogramming.

Another point is that the three full steps are not always required. Indeed,
sometimes one just needs to do a type-level computation and query something
about the result, without necessarily fetching the result as a normal C++ type:

@code{cpp}
auto t = hana::type_c<T>;
auto result = type_computation(t);
BOOST_HANA_CONSTANT_CHECK(is_pointer(result)); // third step skipped
@endcode

In this case, we were able to skip the third step because we did not need to
access the actual type represented by `result`. In other cases, the first step
can be avoided, like when using `tuple_t`, which has no more syntactic noise
than any other pure type-level approach:

@snippet example/tutorial/type.cpp skip_first_step

For skeptical readers, let's consider the task of finding the smallest type
in a sequence of types. This is a very good example of a short type-only
computation, which is where we would expect the new paradigm to suffer the
most. As you will see, things stay manageable even for small computations.
First, let's implement it with the MPL:

@snippet example/tutorial/type.cpp smallest.MPL

The result is quite readable (for anyone familiar with the MPL). Let's now
implement the same thing using Hana:

@snippet example/tutorial/type.cpp smallest.Hana

As you can witness, the syntactic noise of the 3-step process is almost
completely hidden by the rest of the computation.


@subsection tutorial-type-lifting The generic lifting process

The first type-level computation that we introduced in the form of a function
looked like:

@code{cpp}
template <typename T>
constexpr auto add_pointer(hana::basic_type<T> const&) {
  return hana::type<T*>;
}
@endcode

While it looks more complicated, we could also write it as:

@code{cpp}
template <typename T>
constexpr auto add_pointer(hana::basic_type<T> const&) {
  return hana::type_c<typename std::add_pointer<T>::type>;
}
@endcode

However, this implementation emphasizes the fact that we're really emulating
an existing metafunction and simply representing it as a function. In other
words, we're _lifting_ a metafunction (`std::add_pointer`) to the world of
values by creating our own `add_pointer` function. It turns out that this
lifting process is a generic one. Indeed, given any metafunction, we could
write almost the same thing:

@code{cpp}
template <typename T>
constexpr auto add_const(hana::basic_type<T> const&)
{ return hana::type_c<typename std::add_const<T>::type>; }

template <typename T>
constexpr auto add_volatile(hana::basic_type<T> const&)
{ return hana::type_c<typename std::add_volatile<T>::type>; }

template <typename T>
constexpr auto add_lvalue_reference(hana::basic_type<T> const&)
{ return hana::type_c<typename std::add_lvalue_reference<T>::type>; }

// etc...
@endcode

This mechanical transformation is easy to abstract into a generic lifter
that can handle any [MPL Metafunction][MPL.metafunction] as follows:

@snippet example/tutorial/type.cpp metafunction1

More generally, we'll want to allow metafunctions with any number of
arguments, which brings us to the following less naive implementation:

@snippet example/tutorial/type.cpp metafunction2

Hana provides a similar generic metafunction lifter called `hana::metafunction`.
One small improvement is that `hana::metafunction<F>` is a function object
instead of an overloaded function, so one can pass it to higher-order
algorithms. It is also a model of the slightly more powerful concept of
`Metafunction`, but this can safely be ignored for now. The process we
explored in this section does not only apply to metafunctions; it also
applies to templates. Indeed, we could define:

@snippet example/tutorial/type.cpp template_

Hana provides a generic lifter for templates named `hana::template_`, and it
also provides a generic lifter for [MPL MetafunctionClasses][MPL.mfc] named
`hana::metafunction_class`. This gives us a way to uniformly represent "legacy"
type-level computations as functions, so that any code written using a classic
type-level metaprogramming library can almost trivially be used with Hana. For
example, say you have a large chunk of MPL-based code and you'd like to
interface with Hana. The process of doing so is no harder than wrapping your
metafunctions with the lifter provided by Hana:

@code{cpp}
template <typename T>
struct legacy {
  using type = ...; // something you really don't want to mess with
};

auto types = hana::make_tuple(...);
auto use = hana::transform(types, hana::metafunction<legacy>);
@endcode

However, note that not all type-level computations can be lifted as-is with
the tools provided by Hana. For example, `std::extent` can't be lifted because
it requires non-type template parameters. Since there is no way to deal with
non-type template parameters uniformly in C++, one must resort to using a
hand-written function object specific to that type-level computation:

@snippet example/tutorial/type.cpp extent

In practice, however, this should not be a problem since the vast majority
of type-level computations can be lifted easily. Finally, since metafunctions
provided by the `<type_traits>` header are used so frequently, Hana provides
a lifted version for every one of them. Those lifted traits are in the
`hana::traits` namespace, and they live in the
`<boost/hana/ext/std/type_traits.hpp>` header:

@snippet example/tutorial/type.cpp traits

This is the end of the section on type computations. While this new paradigm
for type level programming might be difficult to grok at first, it will make
more sense as you use it more and more. You will also come to appreciate how
it blurs the line between types and values, opening new exciting possibilities
and simplifying many tasks.

@note
Curious or skeptical readers should consider checking the minimal
reimplementation of the MPL presented in the [appendices]
(@ref tutorial-appendix-MPL).










@section tutorial-introspection Introspection

------------------------------------------------------------------------------
Static introspection, as we will discuss it here, is the ability of a program
to examine the type of an object at compile-time. In other words, it is a
programmatic interface to interact with types at compile-time. For example,
have you ever wanted to check whether some unknown type has a member named
`foo`? Or perhaps at some point you have needed to iterate on the members
of a `struct`?

@code{cpp}
struct Person {
  std::string name;
  int age;
};

Person john{"John", 30};
for (auto& member : john)
  std::cout << member.name << ": " << member.value << std::endl;

// name: John
// age: 30
@endcode

If you have written a bit of templates in your life, chances are very high
that you came across the first problem of checking for a member. Also, anyone
having tried to implement object serialization or even just pretty printing
has come across the second problem. In most dynamic languages like Python,
Ruby or JavaScript, these problems are completely solved and introspection is
used every day by programmers to make a lot of tasks simpler. However, as a
C++ programmer, we do not have language support for those things, which makes
several tasks much harder than they should be. While language support would
likely be needed to properly tackle this problem, Hana makes some common
introspection patterns much more accessible.


@subsection tutorial-introspection-is_valid Checking expression validity

Given an object of an unknown type, it is sometimes desirable to check
whether this object has a member (or member function) with some name.
This can be used to perform sophisticated flavors of overloading. For
example, consider the problem of calling a `toString` method on objects
that support it, but providing another default implementation for objects
that do not support it:

@code{cpp}
template <typename T>
std::string optionalToString(T const& obj) {
  if (obj.toString() is a valid expression)
    return obj.toString();
  else
    return "toString not defined";
}
@endcode

@note
While most use cases for this technique will be addressed by [concepts lite]
[C++17.clite] in future revisions of the standard, there will still be cases
where a quick and dirty check is more convenient than creating a full blown
concept.

How could we implement a check for the validity of `obj.toString()` as above
in a generic fashion (so it can be reused in other functions, for example)?
Normally, we would be stuck writing some kind of SFINAE-based detection:

@snippet example/tutorial/introspection.cpp has_toString.then

This works, but the intent is not very clear and most people without a deep
knowledge of template metaprogramming would think this is black magic. Then,
we could implement `optionalToString` as

@code{cpp}
template <typename T>
std::string optionalToString(T const& obj) {
  if (has_toString<T>::value)
    return obj.toString();
  else
    return "toString not defined";
}
@endcode

@note
Of course, this implementation won't actually work because both branches of
the `if` statement will be compiled. If `obj` does not have a `toString`
method, the compilation of the `if` branch will fail. We will address
this issue in a moment.

Instead of the above SFINAE trick, Hana provides a `is_valid` function that
can be combined with [C++14 generic lambdas][C++14.glambda] to obtain a much
cleaner implementation of the same thing:

@snippet example/tutorial/introspection.cpp has_toString.now

This leaves us with a function object `has_toString` which returns whether the
given expression is valid on the argument we pass to it. The result is returned
as an `IntegralConstant`, so `constexpr`-ness is not an issue here because the
result of the function is represented as a type anyway. Now, in addition to
being less verbose (that's a one liner!), the intent is much clearer. Other
benefits are the fact that `has_toString` can be passed to higher order
algorithms and it can also be defined at function scope, so there is no need
to pollute the namespace scope with implementation details. Here is how we
would now write `optionalToString`:

@code{cpp}
template <typename T>
std::string optionalToString(T const& obj) {
  if (has_toString(obj))
    return obj.toString();
  else
    return "toString not defined";
}
@endcode

Much cleaner, right? However, as we said earlier, this implementation won't
actually work because both branches of the `if` always have to be compiled,
regardless of whether `obj` has a `toString` method. There are several
possible options, but the most classical one is to use `std::enable_if`:

@snippet example/tutorial/introspection.cpp optionalToString.then

@note
We're using the fact that `has_toString` returns an `IntegralConstant` to
write `decltype(...)::%value`, which is a constant expression. For some
reason, `has_toString(obj)` is not considered a constant expression, even
though I think it should be one because we never read from `obj` (see the
section on [advanced constexpr](@ref tutorial-appendix-constexpr)).

While this implementation is perfectly valid, it is still pretty cumbersome
because it requires writing two different functions and going through the
hoops of SFINAE explicitly by using `std::enable_if`. Since this pattern of
branching at compile-time is so widely used, Hana provides an `if_` function
that can be used to emulate a `static_if` functionality. Here is how we could
write `optionalToString` with Hana's `if_`:

@snippet example/tutorial/introspection.cpp optionalToString

`if_` works a bit like a normal `if` statement, except it takes a condition
that can be an `IntegralConstant` and returns the one of two values (which may
have different types) chosen by the condition. If the condition is true, the
first value is returned, and otherwise the second value is returned. In our
case, the first value is a generic lambda representing the branch we want to
execute if `obj` has a `toString` method, and the second one is the branch we
want to execute otherwise. `if_` therefore returns the branch chosen by the
condition, and we call that branch (which is a generic lambda) immediately
with `obj`. The reason why this approach works is because the body of the
first branch (where we call `x.toString()`) can only be compiled when the
type of `x` is known. Indeed, since the branch is a generic lambda, the type
of the argument is not known until the lambda is called, and the compiler must
wait for `x`'s type to be known before type-checking the lambda's body. Since
the lambda is never called when the condition is not satisfied (Hana's `if_`
takes care of that), the body of the lambda is never type-checked and no
compilation error happens. There are many different ways of branching in
Hana; you may take a look at `eval_if` and `lazy` for details.

@note
The branches inside the `if_` are lambdas. As such, they really are different
functions from the `optionalToString` function. The variables appearing inside
those branches have to be either captured by the lambdas or passed to them as
arguments, and so they are affected by the way they are captured or passed
(by value, by reference, etc..).

Now, the previous example covered only the specific case of checking for the
presence of a non-static member function. However, `is_valid` can be used to
detect the validity of almost any kind of expression. We now present a list
of common use cases for validity checking along with how to use `is_valid` to
implement them.


@subsubsection tutorial-introspection-is_valid-non_static Non-static members

The previous example presented the case of a non-static member function. For
completeness, we show how to check the presence of a non-static member. First,
we can do it in a similar way as we did for the previous example:

@snippet example/tutorial/introspection.cpp non_static_member_from_object

Notice how we cast the result of `x.member` to `void`? This is to make sure
that our detection also works for types that can't be returned from functions,
like array types. Also, it is important to use a reference as the parameter to
our generic lambda, because that would otherwise require `x` to be
[CopyConstructible][], which is not what we're trying to check. This approach
is simple and the most convenient when an object is available. However, when
the checker is intended to be used with no object around, the following
alternate implementation can be better suited:

@snippet example/tutorial/introspection.cpp non_static_member_from_type

This validity checker is different from what we saw earlier because the
generic lambda is not expecting an usual object anymore; it is now expecting
a `type` (which is an object, but still represents a type). We then use the
`hana::traits::declval` _lifted metafunction_ from the
`<boost/hana/ext/std/utility.hpp>` header to create an rvalue of the type
represented by `t`, which we can then use to check for a non-static member.
Finally, instead of passing an actual object to `has_member` (like `Foo{}`
or `Bar{}`), we now pass a `type_c<...>`. This implementation is ideal for
when no object is lying around.


@subsubsection tutorial-introspection-is_valid-static Static members

Checking for a static member is easy, and it is provided for completeness:

@snippet example/tutorial/introspection.cpp static_member

Again, we expect a `type` to be passed to the checker. Inside the generic
lambda, we use `decltype(t)::%type` to fetch the actual C++ type represented
by the `t` object, as explained in the section on [type computations]
(@ref tutorial-type-working). Then, we fetch the static member inside
that type and cast it to `void`, for the same reason as we did for non-static
members.


@subsubsection tutorial-introspection-is_valid-typename Nested type names

Checking for a nested type name is not hard, but it is slightly more
convoluted than the previous cases:

@snippet example/tutorial/introspection.cpp nested_type_name

One might wonder why we use `-> hana::type<typename-expression>` instead
of simply `-> typename-expression`. Again, the reason is that we want to
support types that can't be returned from functions, like array types or
incomplete types.


@subsubsection tutorial-introspection-is_valid-template Nested templates

Checking for a nested template name is similar to checking for a nested type
name, except we use the `template_<...>` variable template instead of
`type<...>` in the generic lambda:

@snippet example/tutorial/introspection.cpp nested_template


@subsection tutorial-introspection-sfinae Taking control of SFINAE

Doing something only if an expression is well-formed is a very common pattern
in C++. Indeed, the `optionalToString` function is just one instance of the
following pattern, which is very general:

@code{cpp}
template <typename T>
auto f(T x) {
  if (some expression involving x is well-formed)
    return something involving x;
  else
    return something else;
}
@endcode

To encapsulate this pattern, Hana provides the `sfinae` function, which allows
executing an expression, but only if it is well-formed:

@snippet example/tutorial/introspection.sfinae.cpp maybe_add

Here, we create a `maybe_add` function, which is simply a generic lambda
wrapped with Hana's `sfinae` function. `maybe_add` is a function which takes
two inputs and returns `just` the result of the generic lambda if that call
is well-formed, and `nothing` otherwise. `just(...)` and `nothing` both belong
to a type of container called `hana::optional`, which is essentially a
compile-time `std::optional`. All in all, `maybe_add` is morally equivalent
to the following function returning a `std::optional`, except that the check
is done at compile-time:

@code{cpp}
auto maybe_add = [](auto x, auto y) {
  if (x + y is well formed)
    return std::optional<decltype(x + y)>{x + y};
  else
    return std::optional<???>{};
};
@endcode

It turns out that we can take advantage of `sfinae` and `optional` to
implement the `optionalToString` function as follows:

@snippet example/tutorial/introspection.sfinae.cpp optionalToString.sfinae

First, we wrap `toString` with the `sfinae` function. Hence, `maybe_toString`
is a function which either returns `just(x.toString())` if that is well-formed,
or `nothing` otherwise. Secondly, we use the `from_maybe` function to extract
the optional value from the container. `from_maybe` takes a default value and
an optional value. If the optional value is `nothing`, `from_maybe` returns
the default value; otherwise, it returns the value inside the `just` (here
`x.toString()`). This way of seeing SFINAE as a special case of computations
that might fail is very clean and powerful, especially since `sfinae`'d
functions can be combined through the `hana::optional` `Monad`, which is left
to the reference documentation.


@subsection tutorial-introspection-adapting Introspecting user-defined types

Have you ever wanted to iterate over the members of a user-defined type? The
goal of this section is to show you how Hana can be used to do it quite easily.
To allow working with user-defined types, Hana defines the `Struct` concept.
Once a user-defined type is a model of that concept, one can iterate over the
members of an object of that type and query other useful information. To turn
a user-defined type into a `Struct`, a couple of options are available. First,
you may define the members of your user-defined type with the
`BOOST_HANA_DEFINE_STRUCT` macro:

@snippet example/tutorial/introspection.adapt.cpp BOOST_HANA_DEFINE_STRUCT

This macro defines two members (`name` and `age`) with the given types. Then,
it defines some boilerplate inside a `Person::hana` nested `struct`, which is
required to make `Person` a model of the `Struct` concept. No constructors are
defined (so [POD-ness][POD] is retained), the members are defined in the same
order as they appear here and the macro can be used with template `struct`s
just as well, and at any scope. Also note that you are free to add more
members to the `Person` type after or before you use the macro. However,
only members defined with the macro will be picked up when introspecting the
`Person` type. Easy enough? Now, a `Person` can be accessed programmatically:

@snippet example/tutorial/introspection.adapt.cpp for_each

Iteration over a `Struct` is done as if the `Struct` was a sequence of pairs,
where the first element of a pair is the key associated to a member, and the
second element is the member itself. When a `Struct` is defined through the
`BOOST_HANA_DEFINE_STRUCT` macro, the key associated to any member is a
compile-time `hana::string` representing the name of that member. This is why
the function used with `for_each` takes a single argument `pair`, and then
uses `first` and `second` to access the subparts of the pair. Also, notice
how the `to<char const*>` function is used on the name of the member? This
converts the compile-time string to a `constexpr char const*` so it can
`cout`ed. Since it can be annoying to always use `first` and `second` to
fetch the subparts of the pair, we can also use the `fuse` function to wrap
our lambda and make it a binary lambda instead:

@snippet example/tutorial/introspection.adapt.cpp for_each.fuse

Now, it looks much cleaner. As we just mentioned, `Struct`s are seen as a kind
of sequence of pairs for the purpose of iteration. In fact, a `Struct` can
even be searched like an associative data structure whose keys are the names
of the members, and whose values are the members themselves:

@snippet example/tutorial/introspection.adapt.cpp at_key

@note
The `_s` user-defined literal creates a compile-time `hana::string`. It is
located in the `boost::hana::literals` namespace. Note that it is not part
of the standard yet, but it is supported by Clang and GCC. If you want to
stay 100% standard, you can use the `BOOST_HANA_STRING` macro instead.

The main difference between a `Struct` and a `hana::map` is that a map can be
modified (keys can be added and removed), while a `Struct` is immutable.
However, you can easily convert a `Struct` into a `hana::map` with `to<Map>`,
and then you can manipulate it in a more flexible way.

@snippet example/tutorial/introspection.adapt.cpp to<Map>

Using the `BOOST_HANA_DEFINE_STRUCT` macro to adapt a `struct` is convenient,
but sometimes one can't modify the type that needs to be adapted. In these
cases, the `BOOST_HANA_ADAPT_STRUCT` macro can be used to adapt a `struct` in
a ad-hoc manner:

@snippet example/tutorial/introspection.adapt.cpp BOOST_HANA_ADAPT_STRUCT

@note
The `BOOST_HANA_ADAPT_STRUCT` macro must be used at global scope.

The effect is exactly the same as with the `BOOST_HANA_DEFINE_STRUCT` macro,
except you do not need to modify the type you want to adapt, which is
sometimes useful. Before we move on to a concrete example of using these
introspection features, it should also be mentioned that `struct`s can be
adapted without using macros. This advanced interface for defining `Struct`s
can be used to specify keys that are not compile-time strings and to specify
custom accessors for the members of the structure. The advanced interface is
described in the documentation for the `Struct` concept.


@subsection tutorial-introspection-json Example: generating JSON

Let's now move on with a concrete example of using the introspection
capabilities we just presented for printing custom objects as JSON.
Our end goal is to have something like this:

@snippet example/tutorial/introspection.json.cpp usage

And the output, after passing it through a JSON pretty-printer,
should look like

@code{.json}
[
  {
    "name": "John",
    "last_name": "Doe",
    "age": 30
  },
  {
    "brand": "Audi",
    "model": "A4"
  },
  {
    "brand": "BMW",
    "model": "Z3"
  }
]
@endcode

First, let's define a couple of utility functions to make string manipulation
easier:

@snippet example/tutorial/introspection.json.cpp utilities

The `quote` and the `to_json` overloads are pretty self-explanatory. The
`join` function, however, might need a bit of explanation. Basically, the
`intersperse` function takes a sequence and a separator, and returns a new
sequence with the separator in between each pair of elements of the original
sequence. In other words, we take a sequence of the form `[x1, ..., xn]` and
turn it into a sequence of the form `[x1, seq, x2, sep, ..., sep, xn]`.
Finally, we fold the resulting sequence with the `_ + _` function object,
which is equivalent to `std::plus<>{}`. Since our sequence contains
`std::string`s (we assume it does), this has the effect of concatenating
all the strings of the sequence into one big string. Now, let's define
how to print a `Sequence`:

@snippet example/tutorial/introspection.json.cpp Sequence

First, we use the `transform` algorithm to turn our sequence of objects into
a sequence of `std::string`s in JSON format. Then, we join that sequence with
commas and we enclose it with `[]` to denote a sequence in JSON notation.
Simple enough? Let's now take a look at how to print user-defined types:

@snippet example/tutorial/introspection.json.cpp Struct

Here, we use the `keys` method to retrieve a `tuple` containing the names of
the members of the user-defined type. Then, we `transform` that sequence into
a sequence of `"name" : member` strings, which we then `join` and enclose with
`{}`, which is used to denote objects in JSON notation. And that's it!










@section tutorial-containers Generalities on containers

------------------------------------------------------------------------------
This section explains several important notions about Hana's containers: how
to create them, the lifetime of their elements and other concerns.


@subsection tutorial-containers-creating Container creation

Just like one can create a `std::tuple` with `std::make_tuple`, a Hana tuple
can be created with `hana::make_tuple`. However, in general, containers in
Hana may be created with the `make` function:

@snippet example/tutorial/containers.cpp make<Tuple>

Actually, `make_tuple` is just a shortcut for `make<Tuple>` so you don't
have to type `boost::hana::make<boost::hana::Tuple>` when you are out of
Hana's namespace. Simply put, `make<...>` is is used all around the library
to create different types of objects, thus generalizing the `std::make_xxx`
family of functions. For example, one can create a `hana::range` of
compile-time integers with `make<Range>`:

@snippet example/tutorial/containers.cpp make<Range>

For convenience, whenever a component of Hana provides a `make<XXX>` function,
it also provides the `make_xxx` shortcut to reduce typing. Also, an interesting
point that can be raised in this example is the fact that `r` is `constexpr`.
In general, whenever a container is initialized with constant expressions only
(which is the case for `r`), that container may be marked as `constexpr`.

But what are these types with a capital letter that have popped up a couple of
times in the tutorial? These types are simply tags __representing__ a given
container. For example, `Range` is actually an empty `struct` representing the
"conceptual type" of an object returned by `make_range`, while the actual type
of such an object is implementation-defined. These tags are very useful because
they represent families of C++ types that are strongly related, but that are
not required to have the same representation. These tags are documented in
the section on [Hana's core](@ref tutorial-core-tags).


@subsection tutorial-containers-elements Container elements

In Hana, containers own their elements. When a container is created, it makes
a _copy_ of the elements used to initialize it and stores them inside the
container. Of course, unnecessary copies are avoided by using move semantics.
Because of those owning semantics, the lifetime of the objects inside the
container is the same as that of the container.

@snippet example/tutorial/containers.cpp lifetime

Much like containers in the standard library, containers in Hana expect their
elements to be objects. For this reason, references _may not_ be stored in
them. When references must be stored inside a container, one should use a
`std::reference_wrapper` instead:

@snippet example/tutorial/containers.cpp reference_wrapper


@subsection tutorial-containers-types Container types

So far, we have only created containers with the `make_xxx` family of
functions, and we always let the compiler deduce the actual type of the
container by using `auto`. We will now clarify what can be expected from
the types of Hana's containers:

@snippet example/tutorial/containers.cpp types

The answer is that it depends. Some containers have well defined types, while
others do not specify their representation. In this example, the type of the
object returned by `make_tuple` is well-defined, while the type returned by
`make_range` is implementation-defined:

@snippet example/tutorial/containers.cpp types_maximally_specified

This is documented on a per-container basis; when a container has an
implementation-defined representation, a note explaining exactly what
can be expected from that representation is included in the container's
description. There are several reasons for leaving the representation of
a container unspecified; they are explained in the
[rationales](@ref tutorial-rationales-container_representation). However,
leaving the type of some containers unspecified makes some things very
difficult to achieve, like overloading functions on heterogeneous containers:

@code{cpp}
template <typename T>
void f(std::vector<T> xs) {
  // ...
}

template <typename ...???>
void f(unspecified-range-type<???> r) {
  // ...
}
@endcode

The `is_a` utility is provided for this reason (and others). `is_a` allows
checking whether a type is a precise kind of container, regardless of the
actual type of the container. For example, the above example could be
rewritten as

@snippet example/tutorial/containers.cpp overloading

This way, the second overload of `f` will only match when `R` is a type whose
tag is `Range`, regardless of the exact representation of that range. Of
course, `is_a` can be used with any kind of container: `Tuple`, `Map`, `Set`
and so on.










@section tutorial-algorithms Generalities on algorithms

------------------------------------------------------------------------------
Much like the previous section introduced general but important notions about
heterogeneous containers, this section introduces general notions about
heterogeneous algorithms.


@subsection tutorial-algorithms-value By-value semantics

Algorithms in Hana always return a new container holding the result. This
allows one to easily chain algorithms by simply using the result of the first
as the input of the second. For example, to apply a function to every element
of a tuple and then reverse the result, one simply has to connect the `reverse`
and `transform` algorithms:

@snippet example/tutorial/algorithms.cpp reverse_transform

This is different from the algorithms of the standard library, where one has
to provide iterators to the underlying sequence. For reasons documented in the
[rationales](@ref tutorial-rationales-iterators), an iterator-based design was
considered but was quickly dismissed in favor of composable and efficient
abstractions better suited to the very particular context of heterogeneous
programming.

One might also think that returning full sequences that own their elements
from an algorithm would lead to tons of undesirable copies. For example, when
using `reverse` and `transform`, one could think that an intermediate copy is
made after the call to `transform`:

@snippet example/tutorial/algorithms.cpp reverse_transform_copy

To make sure this does not happen, Hana uses perfect forwarding and move
semantics heavily so it can provide an almost optimal runtime performance.
So instead of doing a copy, a move occurs between `reverse` and `transform`:

@snippet example/tutorial/algorithms.cpp reverse_transform_move

Ultimately, the goal is that code written using Hana should be equivalent to
clever hand-written code, except it should be enjoyable to write. Performance
considerations are explained in depth in their own [section]
(@ref tutorial-performance).


@subsection tutorial-algorithms-laziness (Non-)Laziness

Algorithms in Hana are not lazy. When an algorithm is called, it does its
job and returns a new sequence containing the result, end of the story.
For example, calling the `permutations` algorithm on a large sequence is
a stupid idea, because Hana will actually compute all the permutations:

@code{cpp}
    auto perms = hana::permutations(hana::make_tuple(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
    // perms has 3 628 800 elements, and your compiler just crashed
@endcode

To contrast, algorithms in Boost.Fusion return views which hold the original
sequence by reference and apply the algorithm on demand, as the elements of
the sequence are accessed. This leads to subtle lifetime issues, like having
a view that refers to a sequence that was destroyed. Hana's design assumes
that most of the time, we want to access all or almost all the elements in a
sequence anyway, and hence performance is not a big argument in favor of
laziness.


@subsection tutorial-algorithms-codegen What is generated?

Algorithms in Hana are a bit special with respect to the runtime code they are
expanded into. The goal of this subsection is not to explain exactly what code
is generated, which depends on the compiler anyway, but to give a feel for
things. Basically, a Hana algorithm is like an unrolled version of an
equivalent classical algorithm. Indeed, since the bounds of the processed
sequence are known at compile-time, it makes sense that we can unroll the
loop over the sequence. For example, let's consider the `for_each` algorithm:

@code{cpp}
auto xs = hana::make_tuple(0, 1, 2, 3);
hana::for_each(xs, f);
@endcode

If `xs` was a runtime sequence instead of a tuple, its length would only be
known at runtime and the above code would have to be implemented as a loop:

@code{cpp}
for (int i = 0; i < xs.size(); ++i) {
  f(xs[i]);
}
@endcode

However, in our case, the length of the sequence is known at compile-time and
so we don't have to check the index at each iteration. Hence, we can just
write:

@code{cpp}
f(xs[0_c]);
f(xs[1_c]);
f(xs[2_c]);
f(xs[3_c]);
@endcode

The main difference here is that no bound checking and index increment is done
at each step, because there is no index anymore; the loop was effectively
unrolled. In some cases, this can be desirable for performance reasons. In
other cases, this can be detrimental to performance because it causes the
code size to grow. As always, performance is a tricky subject and whether
you actually want loop unrolling to happen should be tackled on a case-by-case
basis. As a rule of thumb, algorithms processing all (or a subset) of the
elements of a container are unrolled. In fact, if you think about it, this
unrolling is the only way to go for heterogeneous sequences, because different
elements of the sequence may have different types. As you might have noticed,
we're not using normal indices into the tuple, but compile-time indices, which
can't be generated by a normal `for` loop. In other words, the following does
not make sense:

@code{cpp}
for (??? i = 0_c; i < xs.size(); ++i) {
  f(xs[i]);
}
@endcode


@subsection tutorial-algorithms-effects Side effects and purity

By default, Hana assumes functions to be pure. A pure function is a function
that has no side-effects at all. In other words, it is a function whose effect
on the program is solely determined by its return value. In particular, such a
function may not access any state that outlives a single invocation of the
function. These functions have very nice properties, like the ability to
reason mathematically about them, to reorder or even eliminate calls, and
so on. Except where specified otherwise, all functions used with Hana (i.e.
used in higher order algorithms) should be pure. In particular, functions
passed to higher order algorithms are not guaranteed to be called any
specific number of times. Furthermore, the order of execution is generally
not specified and should therefore not be taken for granted. If this lack of
guarantees about function invocations seems crazy, consider the following use
of the `any_of` algorithm:

@snippet example/tutorial/algorithms.cpp effects

@note
For this to work, the external adapters for `std::integral_constant` contained
in `<boost/hana/ext/std/integral_constant.hpp>` must be included.

According to the previous section on unrolling, this algorithm should be
expanded into something like:

@code{cpp}
auto xs = hana::make_tuple("hello"s, 1.2, 3);
auto pred = [](auto x) { return std::is_integral<decltype(x)>{}; };

auto r = hana::bool_c<
  pred(xs[0_c]) ? true :
  pred(xs[1_c]) ? true :
  pred(xs[2_c]) ? true :
  false
>;

BOOST_HANA_CONSTANT_CHECK(r);
@endcode

Of course, the above code can't work as-is, because we're calling `pred` inside
something that would have to be a constant expression, but `pred` is a lambda
(and lambdas can't be called in constant expressions). However, whether any of
these objects has an integral type is clearly known at compile-time, and hence
we would expect that computing the answer only involves compile-time
computations. In fact, this is exactly what Hana does, and the above
algorithm is expanded into something like:

@snippet example/tutorial/algorithms.cpp effects.codegen

@note
As you will be able to deduce from the next section on cross-phase computations,
the implementation of `any_of` must actually be more general than this. However,
this [lie-to-children][] is perfect for educational purposes.

As you can see, the predicate is never even executed; only its result type on
a particular object is used. Regarding the order of evaluation, consider the
`transform` algorithm, which is specified (for tuples) as:

@code{cpp}
hana::transform(hana::make_tuple(x1, ..., xn), f) == hana::make_tuple(f(x1), ..., f(xn))
@endcode

Since `make_tuple` is a function, and since the evaluation order for the
arguments of a function is unspecified, the order in which `f` is called
on each element of the tuple is unspecified too. If one sticks to pure
functions, everything works fine and the resulting code is often easier
to understand. However, some exceptional algorithms like `for_each` do
expect impure functions, and they guarantee an order of evaluation. Indeed,
a `for_each` algorithm that would only take pure functions would be pretty
much useless. When an algorithm can accept an impure function or guarantees
some order of evaluation, the documentation for that algorithm will mention
it explicitly. However, by default, no guarantees may be taken for granted.


@subsection tutorial-algorithms-cross_phase Cross-phase algorithms

This section introduces the notion of cross-phase computations and algorithms.
In fact, we have already used cross-phase algorithms in the [quick start]
(@ref tutorial-quickstart), for example with `filter`, but we did not explain
exactly what was happening at that time. But before we introduce cross-phase
algorithms, let's define what we mean by _cross-phase_. The phases we're
referring to here are the compilation and the execution of a program. In C++
as in most statically typed languages, there is a clear distinction between
compile-time and runtime; this is called phase distinction. When we speak of
a cross-phase computation, we mean a computation that is somehow performed
across those phases; i.e. that is partly executed at compile-time and partly
executed at runtime.

Like we saw in earlier examples, some functions are able to return something
that can be used at compile-time even when they are called on a runtime value.
For example, let's consider the `length` function applied to a non-`constexpr`
container:

@snippet example/tutorial/algorithms.cpp cross_phase.setup

Obviously, the tuple can't be made `constexpr`, since it contains runtime
`std::string`s. Still, even though it is not called on a constant expression,
`length` returns something that can be used at compile-time. If you think of
it, the size of the tuple is known at compile-time regardless of its content,
and hence it would only make sense for this information to be available to us
at compile-time. If that seems surprising, think about `std::tuple` and
`std::tuple_size`:

@snippet example/tutorial/algorithms.cpp cross_phase.std::tuple_size

Since the size of the tuple is encoded in its type, it is always available
at compile-time regardless of whether the tuple is `constexpr` or not. In Hana,
this is implemented by having `length` return an `IntegralConstant`. Since an
`IntegralConstant`'s value is encoded in its type, the result of `length` is
contained in the type of the object it returns, and the length is therefore
known at compile-time. Because `length` goes from a runtime value (the
container) to a compile-time value (the `IntegralConstant`), `length` is a
trivial example of a cross-phase algorithm (trivial because it does not really
manipulate the tuple). Another algorithm that is very similar to `length` is
the `is_empty` algorithm, which returns whether a container is empty:

@snippet example/tutorial/algorithms.cpp cross_phase.is_empty

More generally, any algorithm that takes a container whose value is known at
runtime but queries something that can be known at compile-time should be able
to return an `IntegralConstant` or another similar compile-time value. Let's
make things slightly more complicated by considering the `any_of` algorithm,
which we already encountered in the previous section:

@snippet example/tutorial/algorithms.cpp cross_phase.any_of_runtime

In this example, the result can't be known at compile-time, because the
predicate returns a `bool` that is the result of comparing two `std::string`s.
Since `std::string`s can't be compared at compile-time, the predicate must
operate at runtime, and the overall result of the algorithm can then only be
known at runtime too. However, let's say we used `any_of` with the following
predicate instead:

@snippet example/tutorial/algorithms.cpp cross_phase.any_of_compile_time

@note
For this to work, the external adapters for `std::integral_constant` contained
in `<boost/hana/ext/std/integral_constant.hpp>` must be included.

First, since the predicate is only querying information about the type of each
element of the tuple, it is clear that its result can be known at compile-time.
Since the number of elements in the tuple is also known at compile-time, the
overall result of the algorithm can, in theory, be known at compile-time. More
precisely, what happens is that the predicate returns a value initialized
`std::is_same<...>`, which inherits from `std::integral_constant`. Hana
recognizes these objects, and the algorithm is written in such a way that it
preserves the `compile-time`ness of the predicate's result. In the end,
`any_of` hence returns an `IntegralConstant` holding the result of the
algorithm, and we use the compiler's type deduction in a clever way to make
it look easy. Hence, it would be equivalent to write (but then you would need
to already know the result of the algorithm!):

@snippet example/tutorial/algorithms.cpp cross_phase.any_of_explicit

Ok, so some algorithms are able to return compile-time values when their input
satisfies some constraints with respect to `compile-time`ness. However, other
algorithms are more restrictive and they _require_ their inputs to satisfy some
constraints regarding `compile-time`ness, without which they are not able to
operate at all. An example of this is `filter`, which takes a sequence and a
predicate, and returns a new sequence containing only those elements for which
the predicate is satisfied. `filter` requires the predicate to return an
`IntegralConstant`. While this requirement may seem stringent, it really makes
sense if you think about it. Indeed, since we're removing some elements from
the heterogeneous sequence, the type of the resulting sequence depends on the
result of the predicate. Hence, the result of the predicate has to be known at
compile-time for the compiler to be able to assign a type to the returned
sequence. For example, consider what happens when we try to filter a
heterogeneous sequence as follows:

@code{cpp}
auto animals = hana::make_tuple(Fish{"Nemo"}, Cat{"Garfield"}, Dog{"Snoopy"});

auto no_garfield = hana::filter(animals, [](auto animal) {
  return animal.name != "Garfield"s;
});
@endcode

Clearly, we know that the predicate will only return false on the second
element, and hence the result _should be_ a `[Fish, Dog]` tuple. However,
the compiler has no way of knowing this since the predicate's result is the
result of a runtime computation, which happens way after the compiler has
finished its job. Hence, the compiler does not have enough information to
determine the return type of the algorithm. However, we could `filter` the
same sequence with any predicate whose result is available at compile-time:

@snippet example/tutorial/algorithms.cpp cross_phase.filter

Since the predicate returns an `IntegralConstant`, we know which elements
of the heterogeneous sequence we'll be keeping at compile-time. Hence, the
compiler is able to figure out the return type of the algorithm. Other
algorithms like `partition` and `sort` work similarly; special algorithm
requirements are always documented, just read the reference documentation
of an algorithm before using it to avoid surprises.

This is the end of the section on algorithms. While this constitutes a fairly
complete explanation of phase interaction inside algorithms, a deeper
understanding can be gained by reading the [advanced section]
(@ref tutorial-appendix-constexpr) on `constexpr` and the reference
for `Constant` and `IntegralConstant`.










@section tutorial-performance Performance considerations

------------------------------------------------------------------------------
C++ programmers love performance, so here's a whole section dedicated to it.
Since Hana lives on the frontier between runtime and compile-time computations,
we are not only interested in runtime performance, but also compile-time
performance. Since both topics are pretty much disjoint, we treat them
separately below.

@note
The benchmarks presented in this section are updated automatically when we
push to the repository. If you notice results that do not withstand the
claims made here, open a [GitHub issue][Hana.issues]; it could be a
performance regression.

@warning
As of writing this, not all of Hana's containers are optimized. Implementing
Hana was a big enough challenge that containers were initially written naively
and are now in the process of being rigorously optimized. In particular, the
associative containers (`hana::map` and `hana::set`) have a pretty bad
compile-time behavior because of their naive implementation, and their runtime
behavior also seems to be problematic in some cases. Improving this situation
is in the TODO list.


@subsection tutorial-performance-compile Compile-time performance

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
pick the best implementation on that compiler. Also note that the benchmarks
we present here are quite precise. Indeed, even though we do not take multiple
measurements and take their mean or something similar to reduce incertitude,
the benchmarks are very stable when they are regenerated, which suggests a
reasonably good precision. Now, let's dive.

First, Hana minimizes its dependency on the preprocessor. In addition to
yielding cleaner error messages in many cases, this reduces the overall
parsing and preprocessing time for header files. Also, because Hana only
supports cutting edge compilers, there are very few workarounds in the
library, which results in a cleaner and smaller library. Finally, Hana
minimizes reliance on any kind of external dependencies. In particular,
it only uses other Boost libraries in a few specific cases, and it does
not rely on the standard library for the largest part. There are several
reasons (other than include times) for doing so; they are documented in
the [rationales](@ref tutorial-rationales-dependencies).

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
The following chart presents the compile-time cost of creating a sequence
of `n` heterogeneous elements.

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.make.compile.json">
</div>

@note
You can zoom on the chart by selecting an area to zoom into. Also, you can
hide a series of points by clicking on it in the legend on the right.

The benchmark methodology is to always create the sequences in the most
efficient way possible. For Hana and `std::tuple`, this simply means using
the appropriate `make_tuple` function. However, for the MPL, this means
creating a `mpl::vectorN` of size up to 20, and then using `mpl::push_back`
to create larger vectors. We use a similar technique for Fusion sequences.
The reason for doing so is that Fusion and MPL sequences have fixed size
limits, and the techniques used here have been found to be the fastest way
to create longer sequences.

For completeness, we also present the compile-time cost of creating a
`std::array` with `n` elements. However, note that `std::array` can only
hold elements with a single type, so we're comparing apples and oranges
here. As you can see, the cost of creating a `std::array` is constant and
essentially inexistent. Hence, while Hana provides improved compile-times
over other heterogeneous containers, please stick with normal homogeneous
containers if that's all you need for your application; your compile-times
will be much faster that way.

You can also see that creating sequences has a non-negligible cost. Actually,
this is really the most expensive part of doing heterogeneous computations,
as you will see in the following charts. When you look at the charts here and
elsewhere in the library, keep in mind the cost of merely creating the sequences.
Also note that only the most important algorithms will be presented here, but
micro benchmarks for compile-time performance are scattered in the reference
documentation. Also, the benchmarks we present compare several different
libraries. However, since Hana and Fusion can work with values and not only
types, comparing their algorithms with type-only libraries like MPL is not
really fair. Indeed, Hana and Fusion algorithms are more powerful since they
also allow runtime effects to be performed. However, the comparison between
Fusion and Hana is fair, because both libraries are just as powerful (strictly
speaking). Finally, we can't show benchmarks of the algorithms for `std::tuple`,
because the standard does not provide equivalent algorithms. Of course, we
could use Hana's external adapters, but that would not be a faithful comparison.

The first algorithm which is ubiquitous in metaprogramming is `transform`.
It takes a sequence and a function, and returns a new sequence containing the
result of applying the function to each element. The following chart presents
the compile-time performance of applying `transform` to a sequence of `n`
elements. The `x` axis represents the number of elements in the sequence, and
the `y` axis represents the compilation time in seconds. Also note that we're
using the `transform` equivalent in each library; we're not using the
`transform` algorithm from Hana through the Boost.Fusion adapters, for
example, which would probably be less efficient.

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.transform.compile.json">
</div>

Here, we can see that Hana's tuple performs better than all the other
alternatives. This is mainly due to the fact that we use C++11 variadic
parameter pack expansion to implement this algorithm under the hood, which
is quite efficient.

The second important class of algorithms are folds. Folds can be used to
implement many other algorithms like `count_if`, `minimum` and so on.
Hence, a good compile-time performance for fold algorithms ensures a good
compile-time performance for those derived algorithms, which is why we're
only presenting folds here. Also note that all the non-monadic fold variants
are somewhat equivalent in terms of compile-time, so we only present the left
folds. The following chart presents the compile-time performance of applying
`fold_left` to a sequence of `n` elements. The `x` axis represents the number
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
constitutes a good test of the implementation quality of Hana, without taking
into account the free lunch given to use by C++14.

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.find_if.compile.json">
</div>

As you can see, Hana performs better than Fusion, and as well as MPL, yet
Hana's `find_if` can be used with values too, unlike MPL's. This concludes
the section on compile-time performance, but there are micro benchmarks of
compile-time performance scattered around the documentation if you want to
see the compile-time behavior of a particular algorithm.


@subsection tutorial-performance-runtime Runtime performance

Hana was designed to be very efficient at runtime. But before we dive into the
details, let's clarify one thing. Hana being a metaprogramming library which
allows manipulating both types and values, it does not always make sense to
even talk about runtime performance. Indeed, for type-level computations and
computations on `IntegralConstant`s, runtime performance is simply not a
concern, because the result of the computation is contained in a _type_, which
is a purely compile-time entity. In other words, these computations involve
only compile-time work, and no code is even generated to perform these
computations at runtime. The only case where it makes sense to discuss runtime
performance is when manipulating runtime values in heterogeneous containers
and algorithms, because this is the only case where the compiler has to
generate some runtime code. It is therefore only computations of this sort
that we will be studying in the remainder of this section.

Like we did for compile-time benchmarks, the methodology used to measure
runtime performance in Hana is data driven rather than analytical. In other
words, instead of trying to determine the complexity of an algorithm by
counting the number of basic operations it does as a function of the input
size, we simply take measurements for the most interesting cases and see how
it behaves. There are a couple of reasons for doing so. First, we do not
expect Hana's algorithms to be called on large inputs since those algorithms
work on heterogeneous sequences whose length must be known at compile-time.
For example, if you tried to call the `find_if` algorithm on a sequence of
100k elements, your compiler would simply die while trying to generate the
code for this algorithm. Hence, algorithms can't be called on very large inputs
and the analytical approach then loses a lot of its attractiveness. Secondly,
processors have evolved into pretty complex beasts, and the actual performance
you'll be able to squeeze out is actually controlled by much more than the
mere number of steps your algorithm is doing. For example, bad cache behavior
or branch misprediction could turn a theoretically efficient algorithm into a
slowpoke, especially for small inputs. Since Hana causes a lot of unrolling to
happen, these factors must be considered even more carefully and any analytical
approach would probably only comfort us into thinking we're efficient. Instead,
we want hard data, and pretty charts to display it!

There are a couple of different aspects we will want to benchmark. First, we
will obviously want to benchmark the execution time of the algorithms.
Secondly, because of the by-value semantics used throughout the library, we
will also want to make sure that the minimum amount of data is copied around.
Finally, we will want to make sure that using Hana does not cause too much
code bloat because of unrolling, as explained in the [section]
(@ref tutorial-algorithms-codegen) on algorithms.

Just like we studied only a couple of key algorithms for compile-time
performance, we will focus on the runtime performance of a few algorithms.
For each benchmarked aspect, we will compare the algorithm as implemented by
different libraries. Our goal is to always be at least as efficient as
Boost.Fusion, which is near from optimality in terms of runtime performance.
For comparison, we also show the same algorithm as executed on a runtime
sequence, and on a sequence whose length is known at compile-time but whose
`transform` algorithm does not use explicit loop unrolling. All the benchmarks
presented here are done in a _Release_ CMake configuration, which takes care
of passing the proper optimization flags (usually `-O3`). Let's start with the
following chart, which shows the execution time required to `transform`
different kinds of sequences:

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.transform.execute.json">
</div>

As you can see, Hana and Fusion are pretty much on the same line. `std::array`
is slightly slower for larger collections data sets, and `std::vector` is
noticeably slower for larger collections. Since we also want to look out for
code bloat, let's take a look at the size of the executable generated for the
exact same scenario:

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.transform.bloat.json">
</div>

As you can see, code bloat does not seem to be an issue, at least not one that
can be detected in micro benchmarks such as this one. Let's now take a look at
the `fold` algorithm, which is used very frequently:

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.fold_left.execute.json">
</div>

Here, you can see that everybody is performing pretty much the same, which
is a good sign that Hana is at least not screwing things up.
Again, let's look at the executable size:

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.fold_left.bloat.json">
</div>

Here again, the code size did not explode. So at least for moderate usages of
Hana (and Fusion for that matter, since they have the same problem), code
bloat should not be a major concern. The containers in the charts we just
presented contain randomly generated `int`s, which is cheap to copy around and
lends itself well to micro benchmarks. However, what happens when we chain
multiple algorithms on a container whose elements are expensive to copy? More
generally, the question is: when an algorithm is passed a temporary object,
does it seize the opportunity to avoid unnecessary copies? Consider:

@code{cpp}
auto xs = hana::make_tuple("some"s, "huge"s, "string"s);

// No copy of xs's elements should be made: they should only be moved around.
auto ys = hana::reverse(std::move(xs));
@endcode

To answer this question, we'll look at the chart generated when benchmarking
the above code for strings of about 1k characters. However, note that it does
not really make sense to benchmark this for standard library algorithms,
because they do not return containers.

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.reverse.move.json">
</div>

As you can see, Hana is faster than Fusion, probably because of a more
consistent use of move semantics in the implementation. If we had not
provided a temporary container to `reverse`, no move could have been
performed by Hana and both libraries would have performed similarly:

<div class="benchmark-chart"
     style="min-width: 310px; height: 400px; margin: 0 auto"
     data-dataset="benchmark.reverse.nomove.json">
</div>

This concludes the section on runtime performance. Hopefully you are now
convinced that Hana was built for speed. Performance is important to us:
if you ever encounter a scenario where Hana causes bad code to be generated
(and the fault is not on the compiler), please open an [issue][Hana.issues]
so the problem can be addressed.










@section tutorial-ext Integration with external libraries

------------------------------------------------------------------------------

Hana provides out-of-the-box integration with some existing libraries.
Specifically, this means that you can use some containers from these
libraries in Hana's algorithms by simply including the appropriate header
making the bridge between Hana and the external component.

For a component named `xxx` in a namespace `ns`, the external adapter lives
in the `boost/hana/ext/ns/xxx.hpp` header. For example, the external adapter
for `std::tuple` lives in the `boost/hana/ext/std/tuple.hpp` header, while
the external adapter for `boost::mpl::vector` is in
`boost/hana/ext/boost/mpl/vector.hpp`.

@todo
This section has not been written yet for lack of time. Also note that
the reference documentation for external adapters is currently incomplete.
However, for reference and until the documentation is updated, here is a list
of the external adapters that are currently supported:
- `std::tuple`\n
  Models `Sequence`. It can basically be used like a `hana::tuple`. However,
  please be aware that libc++'s tuple has a couple of bugs (they were
  reported) that make this adapter buggy.
- `std::integral_constant`\n
  Model of `Constant`, and all the free models it provides.
- `std::ratio`\n
  Model of `Orderable`, `Comparable` and `IntegralDomain`.
- `std::integer_sequence`\n
  Model of `Comparable`, `Foldable`, `Iterable` and `Searchable`. You should
  be using `hana::range` if you want speed, though.
- `std::pair`\n
  Model of `Product`. This is essentially equivalent to `hana::pair`.
- `boost::mpl::vector`\n
  See `boost::hana::ext::boost::mpl::Vector`.
- `boost::mpl::integral_c`\n
  See `boost::hana::ext::boost::mpl::IntegralC`.
- `boost::fusion::{deque,list,tuple,vector}`\n
  They are models of `Sequence`, and hence can be used like `hana::tuple`
  in algorithms. However, Fusion has several bugs that make these adapters
  slightly explosive to use (sometimes things may fail without apparent reason).
- `boost::tuple`\n
  Model of `Sequence`. It can be used like `hana::tuple` in algorithms.


@subsection tutorial-ext-std The standard library

@subsection tutorial-ext-fusion Boost.Fusion

@subsection tutorial-ext-mpl Boost.MPL










@section tutorial-core Hana's core

------------------------------------------------------------------------------
The goal of this section is to give a high-level overview of Hana's core.
This core is based on the notion of _tag_, which is borrowed from the
Boost.Fusion and Boost.MPL libraries but taken much further by Hana. These
tags are then used for several purposes, like algorithm customization,
documentation grouping, improving error messages and converting containers
into other containers. Because of its modular design, Hana can be extended
in a ad-hoc manner very easily. In fact, all the functionality of the library
is provided through an ad-hoc customization mechanism, which is explained here.


@subsection tutorial-core-tags Tags

Heterogeneous programming is basically programming with objects having
different types. However, it is clear that some families of objects, while
having different representations (C++ types), are strongly related. For
example, the `std::integral_constant<int, n>` types are different for each
different `n`, but conceptually they all represent the same thing; a
compile-time number. The fact that `std::integral_constant<int, 1>{}` and
`std::integral_constant<int, 2>{}` have different types is just a side effect
of the fact that we're using their type to encode the _value_ of these objects.
Indeed, when manipulating a sequence of `std::integral_constant<int, ...>`s,
chances are that you actually think of it as a homogeneous sequence of an
imaginary `integral_constant` type, disregarding the actual types of the
objects and pretending they are all just `integral_constant`s with different
values.

To reflect this reality, Hana provides _tags_ representing its heterogeneous
containers and other compile-time entities. For example, all of Hana's
`integral_constant<int, ...>`s have different types, but they all share
the same tag, `IntegralConstant<int>`. This allows the programmer to think
in terms of that single tag instead of trying to think in terms of the actual
types of the objects. Furthermore, Hana adopts the convention of naming these
tags with a capital letter, to make them stand out and differentiate them from
actual types. Since we mostly reason in terms of tags instead of specific types,
we sometimes (ab)use the word _data type_ to mean the same thing as _tag_ in
the documentation, always referring to the family of related types.

@note
The tag of an object of type `T` can be obtained by using `datatype<T>::%type`,
or equivalently `datatype_t<T>`.


@subsection tutorial-core-tag_dispatching Tag dispatching

Tag dispatching is a generic programming technique for picking the right
implementation of a function depending on the type of the arguments passed
to the function. The usual mechanism for overriding a function's behavior
is overloading. Unfortunately, this mechanism is not always convenient when
dealing with families of related types having different C++ types, or with
objects of unspecified types as is often the case in Hana. For example,
consider trying to overload a function for all Boost.Fusion vectors:

@code{cpp}
    template <typename ...T>
    void function(boost::fusion::vector<T...> v) {
        // whatever
    }
@endcode

If you know Boost.Fusion, then you probably know that it won't work. This is
because Boost.Fusion vectors are not necessarily specializations of the
`boost::fusion::vector` template. Fusion vectors also exist in numbered
forms, which are all of different types:

@code{cpp}
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
   related types. In Hana, this tag can be accessed using the `datatype`
   metafunction. Specifically, for any type `T`, `datatype<T>::%type` is
   the tag used to dispatch it.

2. A function belonging to the public interface of the library, for which
   we'd like to be able to provide a customized implementation. In Hana,
   these functions are the algorithms associated to a concept, like
   `transform` or `unpack`.

3. An implementation for the function, parameterized with the tag(s) of the
   argument(s) passed to the function. In Hana, this is usually done by having
   a separate template called `xxx_impl` (for an interface function `xxx`)
   with a nested `apply` static function, as will be shown below.

When the public interface function `xxx` is called, it will get the tag of the
argument(s) it wishes to dispatch the call on, and then forward the call to
the `xxx_impl` implementation associated to those tags. For example, let's
implement a basic setup for tag dispatching of a function that prints its
argument to a stream. First, we define the public interface function and the
implementation that can be specialized:

@snippet example/tutorial/tag_dispatching.cpp setup

Now, let's define a type that needs tag dispatching to customize the behavior
of `print`. While some C++14 examples exist, they are too complicated to show
in this tutorial and we will therefore use a C++03 tuple implemented as several
different types to illustrate the technique:

@snippet example/tutorial/tag_dispatching.cpp Vector

The nested `struct hana { using datatype = Vector; }` part is a terse way of
controling the result of `datatype` metafunction, and hence the tag of the
`vectorN` type. This is explained in the reference for `datatype`. Finally,
if you wanted to customize the behavior of the `print` function for all the
`vectorN` types, you would normally have to write something along the lines of

@snippet example/tutorial/tag_dispatching.cpp old_way

Now, with tag dispatching, you can rely on the `vectorN`s all sharing the same
tag and specialize only the `print_impl` struct instead:

@snippet example/tutorial/tag_dispatching.cpp customize

One upside is that all `vectorN`s can now be treated uniformly by the `print`
function, at the cost of some boilerplate when creating the data structure
(to specify the tag of each `vectorN`) and when creating the initial `print`
function (to setup the tag dispatching system with `print_impl`). There are
also other advantages to this technique, like the ability to check for
preconditions in the interface function without having to do it in each
custom implementation, which would be tedious:

@snippet example/tutorial/tag_dispatching.cpp preconditions

@note
Checking preconditions does not make much sense for a `print` function, but
consider for example a function to get the `n`th element of a sequence; you
might want to make sure that the index is not out-of-bounds.

This technique also makes it easier to provide interface functions as function
objects instead of normal overloaded functions, because only the interface
function itself must go through the trouble of defining a function object.
Function objects have several advantages over overloaded functions, like the
ability to be used in higher order algorithms or as variables:

@snippet example/tutorial/tag_dispatching.cpp function_objects


@subsection tutorial-core-creating_concepts Creating new concepts

@todo Write this section.










@section tutorial-header_organization Header organization

------------------------------------------------------------------------------
The library is designed to be modular while keeping the number of headers that
must be included to get basic functionality reasonably low. The structure of
the library was also intentionally kept simple, because we all love simplicity.
What follows is a general overview of the header organization. A list of all
the headers provided by the library is also available in the panel on the left
(under the [Headers](files.html) label) in case you need more details.

- `boost/hana.hpp`\n
  This is the master header of the library. It includes the whole public
  interface of the library except adapters for external libraries, which
  must be included separately.

- `boost/hana/`\n
  This is the main directory of the library containing the definitions of
  everything provided by the library. Each algorithm and container provided
  by the library has its own header. For a container or an algorithm named
  `XXX`, the corresponding header is `boost/hana/XXX.hpp`.

  - `boost/hana/concept/`\n
    This subdirectory contains the definition of Hana's concepts. These
    headers provide a way to check whether an object is a model of the
    corresponding concept, and they sometimes also provide default
    implementations for other related concepts, which are documented
    on a per-concept basis. They also include all the algorithms associated
    to that concept.

  - `boost/hana/core/`\n
    This subdirectory contains the machinery for tag dispatching and other
    related utilities like `make` and `to`.

  - `boost/hana/fwd/`\n
    This subdirectory contains the forward declaration of everything in the
    library. It is essentially a mirror of the `boost/hana/` directory, except
    all the headers contain only forward declarations and documentation. For
    example, to include the `hana::tuple` container, one can use the
    `boost/hana/tuple.hpp` header. However, if one only wants the
    forward declaration of that container, the `boost/hana/fwd/tuple.hpp`
    header can be used instead. Note that forward declarations for headers
    in `boost/hana/ext/` and `boost/hana/functional/` are not provided.

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










@section tutorial-conclusion Conclusion

------------------------------------------------------------------------------
You now have everything you need to start using the library. From this point
forward, mastering the library is only a matter of understanding how to use
the general purpose concepts and containers provided with it, which is best
done by looking at the reference documentation. At some point, you will
probably also want to create your own concepts and data types that fit your
needs better; go ahead, the library was designed to be used that way.

@subsection tutorial-conclusion-warning Fair warning: functional programming ahead

Programming with heterogeneous objects is inherently functional -- since it is
impossible to modify the type of an object, a new object must be introduced
instead, which rules out mutation. Unlike previous metaprogramming libraries
whose design was modeled on the STL, Hana uses a functional style of
programming which is the source for a good portion of its expressiveness.
However, as a result, many concepts presented in the reference will be
unfamiliar to C++ programmers without a knowledge of functional programming.
The reference attempts to make these concepts approachable by using intuition
whenever possible, but bear in mind that the highest rewards are usually the
fruit of some effort.

This finishes the tutorial part of the documentation. I hope you enjoy using
the library, and please consider [contributing][Hana.contributing] to make it
even better!

-- Louis










@section tutorial-reference Using the reference

------------------------------------------------------------------------------
As for most generic libraries, algorithms in Hana are documented by the
concept to which they belong (`Foldable`, `Iterable`, `Searchable`, `Sequence`,
etc...). The different containers are then documented on their own page, and
the concepts that they model are documented there. The concepts modeled by
some container defines what algorithms can be used with such a container.

More specifically, the structure of the reference (available in the menu to
the left) goes as follow:

- @ref group-core\n
  Documentation for the core module, which contains everything needed to
  create concepts, data types and related utilities. This is relevant
  if you need to extend the library, but otherwise you can probably
  ignore this.

- @ref group-concepts\n
  Documentation for all the concepts provided with the library. Each concept:
  - Documents which functions must be implemented absolutely in order to
    model that concept. The set of functions that must be provided is called
    a _minimal complete definition_.
  - Documents semantic constraints that any model of that concept must satisfy.
    These constraints are usually called laws and they are expressed in a
    semi-formal mathematical language. Of course, those laws can't be checked
    automatically but you should still make sure you satisfy them.
  - Documents the concept(s) it refines, if any. Sometimes, a concept is
    powerful enough to provide a model of a concept it refines, or at least
    the implementation for some of its associated functions. When this is the
    case, the concept will document which functions of the refined concept it
    provides, and how it does so. Also, it is sometimes possible that the
    model for a refined concept is unique, in which case it can be provided
    automatically. When this happens, it will be documented but you don't have
    to do anything special to get that model.

- @ref group-datatypes\n
  Documentation for all the data structures provided with the library. Each
  data structure documents the concept(s) it models, and how it does so. It
  also documents the methods tied to it but not to any concept, for example
  `from_just` for `optional`.

- @ref group-functional\n
  General purpose function objects that are generally useful in a purely
  functional setting. These are currently not tied to any concept or container.

- @ref group-ext\n
  Documentation for all the adapters for external libraries. Basically, we
  assign a tag to some objects in external libraries and we document them as
  if they were normal data types provided by Hana.

- @ref group-config\n
  Macros that can be used to tweak the global behavior of the library.

- @ref group-assertions\n
  Macros to perform various types of assertions.

- [<b>Alphabetical index</b>](functions.html)\n
  Alphabetical index of everything provided in the library.

- [<b>Headers</b>](files.html)\n
  A list of all the headers provided by the library.

- @ref group-details\n
  Implementation details; don't go there. Anything not documented at all or
  documented in this group is not guaranteed to be stable.

After you get to know Hana a bit better, it will probably happen that you just
want to find the reference for a precise function, concept or container. If
you know the name of what you're looking for, you can use the _search_ box
located in the upper right corner of any page of the documentation. My
personal experience is that this is by far the quickest way of finding
what you want when you already know its name.


@subsection tutorial-reference-signatures Function signatures

As you will see in the reference, several functions provide signatures
documented in a semi-formal mathematical language. We are in the process
of documenting all functions in this way, but this may take a while. The
notation used is the usual mathematical notation for defining functions.
Specifically, a function `Return f(Arg1, ..., ArgN);` can be defined
equivalently using mathematical notation as

@f[
  \mathtt{f} : \mathtt{Arg}_1 \times \dots \times \mathtt{Arg}_n
                  \to \mathtt{Return}
@f]

However, instead of documenting the actual argument and return types of
functions, those signatures are written in terms of argument and return tags.
This is done because of the heterogeneous setting, where the actual type of
an object is usually pretty meaningless and does not help to reason about
what's being returned or taken by a function. For example, instead of
documenting the `equal` function for `IntegralConstant`s as

@f[
  \mathtt{equal} : \mathtt{integral\_constant<T, n>} \times
                   \mathtt{integral\_constant<T, m>}
                      \to \mathtt{integral\_constant<bool, n == m>}
@f]

which is not really helpful (as it really presents nothing but the
implementation), it is instead documented using the `IntegralConstant`
tag. Note that since `equal` is part of the `Comparable` concept, it is
not _actually_ documented for `IntegralConstant` specifically, but the
idea is there:

@f[
  \mathtt{equal} : \mathtt{IntegralConstant<T>} \times
                   \mathtt{IntegralConstant<T>}
                      \to \mathtt{IntegralConstant<bool>}
@f]

This clearly conveys the intention that comparing two `IntegralConstant`s
gives back another `IntegralConstant` holding a `bool`. In general, this
abstraction of the actual representation of objects makes it possible for
us to reason in a high level manner about functions, even though their
actual return and argument types are heterogeneous and not helpful. Finally,
most functions expect container elements to have some properties. For example,
this is the case of the `sort` algorithm, which obviously requires the
container elements to be `Orderable`. Normally, we would write the signature
for the non-predicated version of `sort` as

@f[
  \mathtt{sort} : \mathtt{S} \to \mathtt{S} \\
    \text{where S is a Sequence}
@f]

However, this fails to express the requirement that the contents of `S` are
`Orderable`. To express this, we use the following notation:

@f[
  \mathtt{sort} : \mathtt{S(T)} \to \mathtt{S(T)} \\
    \text{where S is a Sequence and T is Orderable}
@f]

One way to see this is to pretend that `S`, the sequence tag, is actually
parameterized by the tag of the sequence's elements, `T`. We're also pretending
that the elements all have the same tag `T`, which is not the case in general.
Now, by stating that `T` must be `Orderable`, we're expressing the fact that
the sequence's elements must be `Orderable`. This notation is used in different
flavors to express different kinds of requirements. For example, the
`cartesian_product` algorithm takes a sequence of sequences and returns the
cartesian product of those sequences as a sequence of sequences. Using our
notation, this can be conveyed very easily:

@f[
  \mathtt{cartesian\_product} : \mathtt{S(S(T))} \to \mathtt{S(S(T))} \\
    \text{where S is a Sequence}
@f]










@section tutorial-acknowledgements Acknowledgements

------------------------------------------------------------------------------
I'd like to thank the following persons and organizations for contributing to
Hana in one way or another:

- Zach Laine and Matt Calabrese for the original idea of using function call
  syntax to do type-level computations, as presented in their BoostCon
  [presentation][video.inst_must_go] ([slides 1][slides.inst_must_go1])
  ([slides 2][slides.inst_must_go2]).
- Joel Falcou for mentoring me two consecutive years during my work on Hana
  as part of the [Google Summer of Code][GSoC] program, Niall Douglas for
  being the GSoC admin for Boost and helping me get in the program, and
  finally Google for their awesome GSoC program.
- The [Boost Steering committee][Boost.Steering] for unlocking a grant for me
  to work on Hana in the winter of 2015, as an extension to the previous
  year's GSoC.
- Several [C++Now][] attendees and members of the [Boost mailing list]
  [Boost.Devel] for insightful conversations, comments and questions
  about the project.










@section tutorial-glossary Glossary

------------------------------------------------------------------------------
The reference documentation uses a couple of terms that are specific to this
library. Also, a simplified implementation of functions is sometimes provided
in pseudo-code, the actual implementation sometimes being slightly hard to
understand. This section defines terms used in the reference and in the
pseudo-code used to describe some functions.

@anchor tutorial-glossary-forwarded
#### `forwarded(x)`
Means that the object is forwarded optimally. This means that if `x` is a
parameter, it is `std::forward`ed, and if it is a captured variable, it is
moved from whenever the enclosing lambda is an rvalue.

Also note that when `x` can be moved from, the statement `return forwarded(x);`
in a function with `decltype(auto)` does not mean that an rvalue reference to
`x` will be returned, which would create a dangling reference. Rather, it
means that `x` is returned by value, the value being constructed with the
`std::forward`ed `x`.

@anchor tutorial-glossary-perfect_capture
#### `perfect-capture`
This is used in lambdas to signify that the captured variables are
initialized using perfect forwarding, as if `[x(forwarded(x))...]() { }`
had been used.

@anchor tutorial-glossary-tag_dispatched
#### `tag-dispatched`
This means that the documented function uses [tag dispatching]
(@ref tutorial-core-tag_dispatching), and hence the exact
implementation depends on the model of the concept associated
to the function.

@anchor tutorial-glossary-implementation_defined
#### `implementation-defined`
This expresses the fact that the exact implementation of an entity (usually a
type) should not be relied upon by users. In particular, this means that one
can not assume anything beyond what is written explicitly in the documentation.
Usually, the concepts satisfied by an implementation-defined entity will be
documented, because one could otherwise do nothing with it. Concretely,
assuming too much about an implementation-defined entity will probably
not kill you, but it will very probably break your code when you update
to a newer version of Hana.










@section tutorial-rationales Rationales/FAQ

------------------------------------------------------------------------------
This section documents the rationale for some design choices. It also serves
as a FAQ for some (not so) frequently asked questions. If you think something
should be added to this list, open a GitHub issue and we'll consider either
improving the documentation or adding the question here.


@subsection tutorial-rationales-dependencies Why restrict usage of external dependencies?

There are several reasons for doing so. First, Hana is a very fundamental
library; we are basically reimplementing the core language and the standard
library with support for heterogeneous types. When going through the code,
one quickly realizes that other libraries are rarely needed, and that almost
everything has to be implemented from scratch. Also, since Hana is very
fundamental, there is even more incentive for keeping the dependencies
minimal, because those dependencies will be handed down to the users.
Regarding the minimal reliance on Boost in particular, one big argument
for using it is portability. However, as a cutting edge library, Hana only
targets very recent compilers. Hence, we can afford to depend on modern
constructs and the portability given to us by using Boost would mostly
represent dead weight.


@subsection tutorial-rationales-iterators Why no iterators?

Iterator based designs have their own merits, but they are also known to
reduce the composability of algorithms. Furthermore, the context of
heterogeneous programming brings a lot of points that make iterators much
less interesting. For example, incrementing an iterator would have to return
a new iterator with a different type, because the type of the new object it
is pointing to in the sequence might be different. It also turns out that
implementing most algorithms in terms of iterators leads to a worse
compile-time performance, simply because the execution model of metaprogramming
(using the compiler as an interpreter) is so different from the runtime
execution model of C++ (a processor accessing contiguous memory).


@subsection tutorial-rationales-container_representation Why leave some container's representation implementation-defined?

First, it gives much more wiggle room for the implementation to perform
compile-time and runtime optimizations by using clever representations for
specific containers. For example, a tuple containing homogeneous objects of
type `T` could be implemented as an array of type `T` instead, which is more
efficient at compile-time. Secondly, and most importantly, it turns out that
knowing the type of a _heterogeneous_ container is not as useful as you would
think. Indeed, in the context of heterogeneous programming, the type of the
object returned by a computation is usually part of the computation too. In
other words, there is no way to know the type of the object returned by an
algorithm without actually performing the algorithm. For example, consider
the `find_if` algorithm:

@snippet example/tutorial/rationale.container.cpp hana

If the predicate is satisfied for some element of the tuple, result will be
equal to `just(x)`. Otherwise, `result` will be equal to `nothing`. However,
the `nothing`ness of the result is known at compile-time, which requires
`just(x)` and `nothing` to have different types. Now, say you wanted to
explicitly write the type of the result:

@snippet example/tutorial/rationale.container.cpp hana-explicit

In order to possess the knowledge of what `some_type` is, you would need to
actually perform the algorithm, because `some_type` depends on whether the
predicate is satisfied or not for some element in the container. In other
words, if you were able to write the above, then you would already know what
the result of the algorithm is and you would not need to perform the algorithm
in the first place. In Boost.Fusion, this problem is addressed by having a
separate `result_of` namespace, which contains a metafunction computing the
result type of any algorithm given the types of the arguments passed to it.
For example, the above example could be rewritten with Fusion as:

@snippet example/tutorial/rationale.container.cpp fusion

Notice that we're basically doing the computation twice; once in the `result_of`
namespace and once in the normal `fusion` namespace, which is highly redundant.
Before the days of `auto` and `decltype`, such techniques were necessary to
perform heterogeneous computations. However, since the advent of modern C++,
the need for explicit return types in the context of heterogeneous programming
is largely obsolete, and knowing the actual type of containers is usually not
that useful.


@subsection tutorial-rationales-why_Hana Why Hana?

No, it isn't the name of my girlfriend! I just needed a short and good looking
name that people would easily remember, and Hana came up. It also came to my
attention that Hana means _flower_ in Japanese, and _one_ in Korean. Since
Hana is pretty and it unifies type-level and heterogeneous programming under
a single paradigm, the name appears to be quite well chosen in retrospect :-).


@subsection tutorial-rationales-tuple Why define our own tuple?

Since Hana defines a lot of algorithms on tuples, a possible way to go would
have been to simply use `std::tuple` and provide the algorithms only, instead
of also providing our own tuple. The reason for providing our own tuple is
principally performance. Indeed, all the `std::tuple` implementations tested
so far have a very bad compile-time performance. Also, to get truly amazing
compile-time performance, we need to take advantage of the tuple's internal
representation in some algorithms, which requires defining our own. Finally,
some sugar like `operator[]` could not be provided if we were using a
`std::tuple`, since that operator must be defined as a member function.


@subsection tutorial-rationales-naming How are names chosen?

When deciding upon a name `X`, I try to balance the following things
(in no specific order):

- How idiomatic is `X` in C++?
- How idiomatic is `X` in the rest of the programming world?
- How good of a name `X` _actually is_, regardless of historical reasons
- How do I, as the library author, feel about `X`
- How do users of the library feel about `X`
- Are there technical reasons not to use `X`, like name clashes or names
  reserved by the standard

Of course, good naming is and will always be hard. Names are and will always
be tainted by the author's own bias. Still, I try to choose names in a
reasonable manner.


@subsection tutorial-rationales-parameters How is the parameter order decided?

Unlike naming, which is fairly subjective, the order of the parameters of a
function is usually pretty straightforward to determine. Basically, the rule
of thumb is "the container goes first". It has always been this way in Fusion
and MPL, and this is intuitive for most C++ programmers. Also, in higher-order
algorithms, I try to put the function parameter last, so that multi-line
lambdas look nice:

@code{cpp}
algorithm(container, [](auto x) {
  return ...;
});

// is nicer than

algorithm([](auto x) {
  return ...;
}, container);
@endcode


@subsection tutorial-rationales-tag_dispatching Why tag dispatching?

There are several different techniques we could have used to provide
customization points in the library, and tag-dispatching was chosen. Why?
First, I wanted a two-layer dispatching system because this allows functions
from the first layer (the ones that are called by users) to actually be
function objects, which allows passing them to higher-order algorithms.
Using a dispatching system with two layers also allows adding some
compile-time sanity checks to the first layer, which improves error messages.

Now, tag-dispatching was chosen over other techniques with two layers for a
couple of reasons. First, having to explicitly state how some tag is a model
of a concept gives the responsibility of making sure that the semantic
requirements of the concept are respected to the user. Secondly, when checking
whether a type is a model of some concept, we basically check that some key
functions are implemented. In particular, we check that the functions from the
minimal complete definition of that concept are implemented. For example,
`models<Iterable, T>` checks whether the `is_empty`, `at` and `tail`
functions implemented for `T`. However, the only way to detect this without
tag-dispatching is to basically check whether the following expressions are
valid in a SFINAE-able context:

    implementation_of_at(std::declval<T>(), std::declval<N>())
    implementation_of_is_empty(std::declval<T>())
    implementation_of_tail(std::declval<T>())

Unfortunately, this requires actually doing the algorithms, which might either
trigger a hard compile-time error or hurt compile-time performance. Also, this
requires picking an arbitrary index `N` to call `at` with: what if the `Iterable`
is empty? With tag dispatching, we can just ask whether `at_impl<T>`,
`is_empty_impl<T>` and `tail_impl<T>` are defined, and nothing happens
until we actually call their nested `::%apply` function.


@subsection tutorial-rationales-zip_longest Why not provide `zip_longest`?

It would require either (1) padding the shortest sequences with an arbitrary
object, or (2) padding the shortest sequences with an object provided by the
user when calling `zip_longest`. Since there is no requirement that all the
zipped sequences have elements of similar types, there is no way to provide a
single consistent padding object in all cases. A tuple of padding objects
should be provided, but I find it perhaps too complicated to be worth it for
now. If you need this functionality, open a GitHub issue.










@section tutorial-appendix-constexpr Apendix I: Advanced constexpr

------------------------------------------------------------------------------
In C++, the border between compile-time and runtime is hazy, a fact that is
even more true with the introduction of [generalized constant expressions]
[C++14.gconstexpr] in C++14. However, being able to manipulate heterogeneous
objects is all about understanding that border and then crossing it at one's
will. The goal of this section is to set things straight with `constexpr`; to
understand which problems it can solve and which ones it can't. This section
covers advanced concepts about to constant expressions; only readers with a
good understanding of `constexpr` should attempt to read this.


@subsection tutorial-appendix-constexpr-stripping Constexpr stripping

Let's start with a challenging question. Should the following code compile?

@code{cpp}
template <typename T>
void f(T t) {
  static_assert(t == 1, "");
}

constexpr int one = 1;
f(one);
@endcode

The answer is no, and the error given by Clang goes like

@code{cpp}
error: static_assert expression is not an integral constant expression
  static_assert(t == 1, "");
                ^~~~~~
@endcode

The explanation is that inside of `f`'s body, `t` is not a constant expression,
and hence it can't be used as the operand to a `static_assert`. The reason is
that such a function simply can't be generated by the compiler. To understand
the issue, consider what should happen when we instantiate the `f` template
with a concrete type:

@code{cpp}
// Here, the compiler should generate the code for f<int> and store the
// address of that code into fptr.
void (*fptr)(int) = f<int>;
@endcode

Clearly, the compiler can't generate `f<int>`'s code, which should trigger a
`static_assert` if `t != 1`, because we haven't specified `t` yet. Even worse,
the generated function should work on both constant and non-constant
expressions:

@code{cpp}
void (*fptr)(int) = f<int>; // assume this was possible
int i = ...; // user input
fptr(i);
@endcode

Clearly, `fptr`'s code can't be generated, because it would require being able
to `static_assert` on a runtime value, which does not make sense. Furthermore,
note that it does not matter whether you make the function `constexpr` or not;
making `f` `constexpr` would only state that the _result_ of `f` is a constant
expression whenever its argument is a constant expression, but it still does
not give you the ability to know whether you were called with a constant
expression from `f`'s body. In other words, what we would want is something
like:

@code{cpp}
template <typename T>
void f(constexpr T t) {
  static_assert(t == 1, "");
}

constexpr int one = 1;
f(one);
@endcode

In this hypothetical scenario, the compiler would know that `t` is a constant
expression from the body of `f`, and the `static_assert` could be made to work.
However, `constexpr` parameters do not exist in the current language, and
adding them would bring up very challenging design and implementation issues.
The conclusion of this little experiment is that __argument passing strips
away `constexpr`-ness__. What might be unclear by now are the consequences
of this stripping, which are explained next.


@subsection tutorial-tutorial-appendix-constexpr-preservation Constexpr preservation

The fact that an argument is not a constant expression means that we can't use
it as a non-type template parameter, as an array bound, inside a `static_assert`
or anything else that requires a constant expression. In addition, this means
that the return type of a function can't depend on the _value_ of an argument
which is nothing new if you think about it:

@code{cpp}
    template <int i>
    struct foo { };

    auto f(int i) -> foo<i>; // obviously won't work
@endcode

In fact, the return type of a function may only depend on the types of its
arguments, and `constexpr` can't change this fact. This is of utmost importance
to us, because we're interested in manipulating heterogeneous objects, which
eventually means returning objects with different types depending on the
argument of the function. For example, a function might want to return an
object of type `T` in one case and an object of type `U` in the other;
from our analysis, we now know that these "cases" will have to depend on
information encoded in the _types_ of the arguments, not in their _values_.

To preserve `constexpr`-ness through argument passing, we have to encode the
`constexpr` value into a type, and then pass a not-necessarily-`constexpr`
object of that type to the function. The function, which must be a template,
may then access the `constexpr` value encoded inside that type.

@todo
Improve this explanation and talk about non-integral constant expressions
wrapped into types.


@subsection tutorial-appendix-constexpr-effects Side effects

Let me ask a tricky question. Is the following code valid?

@code{cpp}
template <typename T>
constexpr int f(T& n) { return 1; }

int n = 0;
constexpr int i = f(n);
@endcode

The answer is _yes_, but the reason might not be obvious at first. What
happens here is that we have a non-`constexpr` `int n`, and a `constexpr`
function `f` taking a reference to its argument. The reason why most people
think it shouldn't work is that `n` is not `constexpr`. However, we're not
doing anything with `n` inside of `f`, so there is no actual reason why this
shouldn't work! This is a bit like `throw`ing inside of a `constexpr` function:

@code{cpp}
constexpr int sqrt(int i) {
  if (i < 0) throw "i should be non-negative";

  return ...;
}

constexpr int two = sqrt(4); // ok: did not attempt to throw
constexpr int error = sqrt(-4); // error: can't throw in a constant expression
@endcode

As long as the code path where `throw` appears is not executed, the result of
the invocation can be a constant expression. Similarly, we can do whatever we
want inside of `f`, as long as we don't execute a code path that requires
accessing its argument `n`, which is not a constant expression:

@code{cpp}
template <typename T>
constexpr int f(T& n, bool touch_n) {
  if (touch_n) n + 1;
  return 1;
}

int n = 0;
constexpr int i = f(n, false); // ok
constexpr int j = f(n, true); // error
@endcode

The error given by Clang for the second invocation is

@code{cpp}
error: constexpr variable 'j' must be initialized by a constant expression
constexpr int j = f(n, true); // error
              ^   ~~~~~~~~~~
note: read of non-const variable 'n' is not allowed in a constant expression
  if (touch_n) n + 1;
               ^
@endcode

Let's now step the game up a bit and consider a more subtle example.
Is the following code valid?

@code{cpp}
template <typename T>
constexpr int f(T n) { return 1; }

int n = 0;
constexpr int i = f(n);
@endcode

The only difference with our initial scenario is that `f` now takes its
argument by value instead of by reference. However, this makes a world of
difference. Indeed, we're now asking the compiler to make a copy of `n`
and to pass this copy to `f`. However, `n` is not `constexpr`, so its
value is only known at runtime. How could the compiler make a copy (at
compile-time) of a variable whose value is only known at runtime? Of
course, it can't. Indeed, the error message given by Clang is pretty
explicit about what's happening:

@code{cpp}
error: constexpr variable 'i' must be initialized by a constant expression
constexpr int i = f(n);
              ^   ~~~~
note: read of non-const variable 'n' is not allowed in a constant expression
constexpr int i = f(n);
                    ^
@endcode

@todo
Explain how side-effects may not appear inside constant expressions, even
if the expression they yield are not accessed.

<!-------------------
Let me ask a tricky question. Is the following code valid?

@code{cpp}
  template <typename X>
  auto identity(X x) { return x; }

  static_assert(value(identity(bool_c<true>)), "");
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
------>










@section tutorial-appendix-MPL Apendix II: A minimal MPL

------------------------------------------------------------------------------
This section presents a mini reimplementation of the MPL library. The goal is
to be as backward compatible as possible with the MPL, while still using Hana
under the hood. Only the "Algorithms" part of the MPL is implemented as a case
study, but it should be possible to implement many (but not all) metafunctions
of the MPL.

Scroll down to the `main` function to see the tests. The tests are exactly
the examples in the MPL documentation that were copy/pasted and then
modified as little as possible to work with this reimplementation.

@include example/tutorial/appendix_mpl.cpp










<!-- Links -->
[Boost.Devel]: http://news.gmane.org/gmane.comp.lib.boost.devel
[Boost.Fusion]: http://www.boost.org/doc/libs/release/libs/fusion/doc/html/index.html
[Boost.MPL]: http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html
[Boost.Steering]: https://sites.google.com/a/boost.org/steering/home
[C++14.auto_rt]: http://en.wikipedia.org/wiki/C%2B%2B14#Function_return_type_deduction
[C++14.gconstexpr]: http://en.wikipedia.org/wiki/C%2B%2B11#constexpr_.E2.80.93_Generalized_constant_expressions
[C++14.glambda]: http://en.wikipedia.org/wiki/C%2B%2B14#Generic_lambdas
[C++14.ice]: http://en.cppreference.com/w/cpp/types/integral_constant
[C++14.udl]: http://en.wikipedia.org/wiki/C%2B%2B11#User-defined_literals
[C++14.vtemplate]: http://en.wikipedia.org/wiki/C%2B%2B14#Variable_templates
[C++14]: http://en.wikipedia.org/wiki/C%2B%2B14
[C++17.clite]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3580.pdf
[C++Now]: http://cppnow.org
[constexpr_throw]: http://stackoverflow.com/a/8626450/627587
[CopyConstructible]: http://en.cppreference.com/w/cpp/concept/CopyConstructible
[GOTW]: http://www.gotw.ca/gotw/index.htm
[GSoC]: http://www.google-melange.com/gsoc/homepage/google/gsoc2014
[Hana.contributing]: https://goo.gl/N8DuJW
[Hana.issues]: https://github.com/ldionne/hana/issues
[lie-to-children]: http://en.wikipedia.org/wiki/Lie-to-children
[MPL.arithmetic]: http://www.boost.org/doc/libs/release/libs/mpl/doc/refmanual/arithmetic-operations.html
[MPL.metafunction]: http://www.boost.org/doc/libs/release/libs/mpl/doc/refmanual/metafunction.html
[MPL.mfc]: http://www.boost.org/doc/libs/release/libs/mpl/doc/refmanual/metafunction-class.html
[MPL11]: http://github.com/ldionne/mpl11
[N4487]: https://isocpp.org/files/papers/N4487.pdf
[POD]: http://en.cppreference.com/w/cpp/concept/PODType
[slides.inst_must_go1]: https://github.com/boostcon/2010_presentations/raw/master/mon/instantiations_must_go.pdf
[slides.inst_must_go2]: https://github.com/boostcon/2010_presentations/raw/master/mon/instantiations_must_go_2.pdf
[SO.sfinae]: http://stackoverflow.com/a/257382/627587
[Sprout]: https://github.com/bolero-MURAKAMI/Sprout
[video.inst_must_go]: https://www.youtube.com/watch?v=x7UmrRzKAXU
