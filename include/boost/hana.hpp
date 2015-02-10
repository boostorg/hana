/*!
@file
Includes all the library components except the adapters for external
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
- Document how to write common Boost.Fusion and Boost.MPL idioms with
  Boost.Hana.
- Setup a BJam build system.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_HPP
#define BOOST_HANA_HPP

// Type classes
#include <boost/hana/applicative.hpp>
#include <boost/hana/comparable.hpp>
#include <boost/hana/constant.hpp>
#include <boost/hana/enumerable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/functor.hpp>
#include <boost/hana/group.hpp>
#include <boost/hana/integral_domain.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/monad.hpp>
#include <boost/hana/monoid.hpp>
#include <boost/hana/orderable.hpp>
#include <boost/hana/product.hpp>
#include <boost/hana/record.hpp>
#include <boost/hana/ring.hpp>
#include <boost/hana/searchable.hpp>
#include <boost/hana/traversable.hpp>

// Data types
#include <boost/hana/bool.hpp>
#include <boost/hana/either.hpp>
#include <boost/hana/integer_list.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/lazy.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/maybe.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/set.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/type_list.hpp>

// Misc
#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/core.hpp>
#include <boost/hana/functional.hpp>

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
for what is now Boost.Hana. We are now in late July and the plan is to
request an official review to get the library in Boost by the end of GSoC;
we'll see how that works out.

Let the fun begin.


@section tutorial-introduction Introduction

------------------------------------------------------------------------------
Boost.Hana is a library of combinators tailored towards the manipulation
of heterogeneous collections (think `std::tuple`). It provides high level
operations to manipulate those collections in compile-time efficient ways
and with a great level of expressiveness. One of its main purpose is to
unify type-level (think [Boost.MPL][]) and heterogeneous value-level
programming (think [Boost.Fusion][]) under a single consistent interface,
allowing the compile-time and the runtime worlds to interact in new, useful
ways. This unification also has the nice side effect of increasing code reuse
and easing the learning curve for metaprogramming.

The core of Hana is a powerful compile-time dispatching system allowing the
library to be extended in a ad-hoc manner. This extension system is used
within Hana to provide all the functionality of the library in a modular
way. It makes Hana very easy to extend, and in fact the library provides
interoperation with Boost.Fusion, Boost.MPL and some components of the
standard library out of the box.

@subsection tutorial-introduction-warning A word of warning

Programming with heterogeneous objects is inherently functional -- since it is
impossible to modify the type of an object, a new object must be introduced
instead, which rules out mutation. Unlike previous metaprogramming libraries
like Boost.MPL and Boost.Fusion, I have decided to embrace this and as a
result, Hana uses an almost purely functional style of programming. However,
this style of programming influences not only the internal implementation; it
also leaks into the interface. Hence, if you continue beyond this point, be
prepared to see quite a bit of functional programming along the way.

In particular, unlike previous metaprogramming libraries, the design of Hana
is not based on that of the STL. Instead, it is inspired by several standard
and non standard modules written for the Haskell programming language. Through
experience, I have found this to be much more expressive, flexible and easy to
use while not sacrificing any performance given the
compile-time/runtime-but-heterogeneous setting. However, as a result, many
concepts used in Hana will be unfamiliar to C++ programmers without a
knowledge of FP, to whom I say: be assured that the reward is totally worth
it.


@section tutorial-quick_start Quick start

------------------------------------------------------------------------------
This section assumes the reader is already familiar with basic metaprogramming
and the [C++14 standard][Wikipedia.C++14]. First, let's include the library:

@snippet example/tutorial/quickstart.cpp includes

Unless specified otherwise, the documentation assumes that the above lines
are present before examples and code snippets. Also note that finer grained
headers are provided and will be explained in the [Header organization]
(@ref tutorial-header_organization) section, but for now that'll do.


@subsection tutorial-quick_start-value_computations Value computations
Let's create an heterogeneous list, which is essentially the same as a
`std::tuple`:

@snippet example/tutorial/quickstart.cpp xs

There are several operations that can be performed on lists; here's a couple
so you get the feeling:

@snippet example/tutorial/quickstart.cpp value_operations

> __Note__\n
> `BOOST_HANA_CONSTANT_CHECK` is a compile-time assertion; the details are
> explained later in the tutorial.

You have probably observed how the `auto` keyword is used when defining `xs` 
and how the actual type of `list(...)` is not written anywhere. This will be
the case a lot in Hana. In particular, `list` is a [generic lambda]
[Wikipedia.generic_lambda] taking a variable number of arguments and
returning an object of an unspecified type, but which is nonetheless a
valid sequence for Hana. It is defined as:

@code
    auto list = [](auto ...xs) {
        return unspecified; // magic going on here
    };
@endcode

So the actual type of `list(...)` is completely unspecified; this will also
be the case for most objects defined by Hana. It does not mean that you can't
name that type, but simply that you can't do anything useful with it.

@snippet example/tutorial/quickstart.cpp useless

However, you will never need the actual type of an object to do something
useful with it, so don't let it bother you.

Another interesting observation in the previous example is that `is_empty`
returns a value that's known at compile-time even though the list it was
called on is not a constant expression (it's not declared with `constexpr` and
it contains an object of the non-literal type `std::string` anyways). Indeed,
the size of the sequence is known at compile-time regardless of its contents,
so it only makes sense that Hana does not throw away this information. If that
seems surprising to you, think about `std::tuple`:

@snippet example/tutorial/quickstart.cpp std_tuple_parallel

Since the size of the tuple is encoded in it's type, it's always available
at compile-time regardless of whether the tuple is `constexpr` or not. How
this works in Hana will be explained in detail later, but the trick is that
`is_empty` returns something like a `std::integral_constant`, which can
always be converted to an integral value at compile-time.


@subsection tutorial-quick_start-type_computations Type computations

One of the initial goals of the library was to unify type level programming
with value level programming. So in principle, it should be possible to
manipulate types and sequences of types, and then to perform type computations
on those just as one would do with the Boost.MPL. Indeed, it's possible.
First, we create a list containing types:

@snippet example/tutorial/quickstart.cpp ts

Let me clarify what's going on. First, the list does not _actually_ contain
types; it only contains objects that represent types for Hana. So those
`type<...>` guys are actually objects! In fact, they are [variable templates]
[Wikipedia.variable_template] defined roughly as:

@code
    template <typename T>
    constexpr unspecified type{};
@endcode

Like for `list(...)`, the actual C++ type of `type<...>` is left unspecified,
but you won't need it anyways. Since `type<T>` is an object (the fact that
it's declared with `constexpr` is irrelevant), it makes perfect sense to
create an heterogeneous sequence of these guys. Let's see what kind of
operations we can do with our list:

@snippet example/tutorial/quickstart.cpp type_operations

Ok, so we can store types in heterogeneous sequences and then manipulate those
sequences as-if they contained regular heterogeneous objects (which is in fact
the case). That's neat, but types are still not very useful when they are in
the form of a weird object with an unspecified type that's only understood
by Hana. In other words, we'd like to be able to take the `T` out of a
`type<T>` object, and as a real C++ type that is. That's possible; it turns
out that while `type<T>` has an unspecified type, it is still guaranteed to
have a nested alias to `T` named `type`:

@snippet example/tutorial/quickstart.cpp type_out

Hence, in Boost.MPL parlance, `decltype(type<T>)` is a nullary metafunction
returning `T`. Now that we can retrieve the actual C++ type out of those
objects, we can do pretty much anything we want. For example, let's print
the name of each type in our list, a bit like we did for our list of values
above:

@snippet example/tutorial/quickstart.cpp type_foreach

Are we able to apply type transformations? Sure:

@snippet example/tutorial/quickstart.cpp type_transformation_def

`add_pointer` just takes an object of the form `type<T>` and returns an
object of the form `type<T*>`. Let's make sure it works as expected:

@snippet example/tutorial/quickstart.cpp type_transformation_check

Since this pattern of defining functions that are equivalent to some
metafunction is recurrent (and quite generic), Hana provides utilities
to make this automatically. One of these, `metafunction`, takes an arbitrary
metafunction (in the Boost.MPL sense of it) and promotes it to a regular
function working on `type<...>`s:

@snippet example/tutorial/quickstart.cpp metafunction

Specifically, `metafunction` is a variable template defined roughly as

@code
    template <template <typename ...> class f>
    constexpr auto metafunction = [](auto t) {
        using T = typename decltype(t)::type;
        return type<typename f<T>::type>;
    };
@endcode

except that the real implementation in Hana works for an arbitrary number of
`t` arguments, which is omitted here for simplicity. Hence, `metafunction<f>`
is a function taking a `type<T>` and returning a `type<R>`, where `R` is the
result of applying the `f` metafunction to `T`. This way, we can take
arbitrary metafunctions working on normal C++ types, apply them to our
`type<...>` objects, and then go back to normal C++ types with
`decltype(...)::%type` if we want. At this point, a nice insight is that the
Boost.MPL is in fact equivalent to the subset of Hana that works with
`type<...>`s!

This is it for the quick start. Of course, there is much more available like
associative sequences, sets, ranges and even an heterogeneous `std::optional`
called `Maybe`, but you can read on if you want to know more.


@section tutorial-basic_concepts Basic concepts

------------------------------------------------------------------------------
This section introduces the basic concepts of the library in a friendly way.
In particular, it does not present everything available in the library and it
should not be seen as a replacement for the reference documentation, but
merely as a complement and introduction to it. The terms [type class]
(@ref tutorial-extending-typeclasses) and [data type]
(@ref tutorial-extending-datatypes) will be used in this section without
having been defined precisely. For now, it suffices that you think of the
former as abstract interfaces specifying a set of operations called _methods_,
and of the latter as actual implementations of those interfaces, with the
particularity that a data type may "implement" several such interfaces.


@subsection tutorial-basic_concepts-heterogeneity Heterogeneity

The purpose of Hana is to manipulate heterogeneous objects. Before we dive any
deeper in the library, let's ask a fundamental question: does it even make
sense to manipulate heterogeneous objects?

For the sake of the explanation, let me make the following claim: a function
template that compiles with an argument of every possible type must have a
trivial implementation, in the sense that it must do nothing with its argument
except perhaps return it. Hence, for a function template to do something
interesting, it must fail to compile for some set of arguments. While I won't
try to prove that claim formally -- it might be false in some corner cases --,
think about it for a moment. Let's say I want to apply a function to each
element of an heterogeneous sequence:

@code
    for_each(list(x, y, z), f)
@endcode

The first observation is that `f` must have a templated call operator because
`x`, `y` and `z` have different types. The second observation is that without
knowing anything specific about the types of `x`, `y` and `z`, it is
impossible for `f` to do anything meaningful. For example, could it print its
argument? Of course not, since it does not know whether `std::cout << x` is
well-formed! In order to do something meaningful, the function has to put
constraints on its arguments, it has to define a domain. In other words, it
can't be _fully_ polymorphic, at least not conceptually.

So while we're manipulating types that are technically heterogeneous, they
still conceptually need something in common, or it wouldn't be possible to do
anything meaningful with them. We'll still say that we're manipulating
heterogeneous objects, but always keep in mind that the objects we manipulate
must have something in common, and hence be homogeneous in _some regard_.


@subsection tutorial-basic_concepts-constant Constants, or setting constexpr straight

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

To represent this fact, Hana defines the concept of a `Constant`, which is an
object from which a constant expression may always be obtained, regardless of
the `constexpr`-ness of the object. `Constant`s provide a way to obtain that
constant expression through the use of the `value` function. Specifically, for
any `Constant` `c`, the following program must be valid:

@code
    template <typename X>
    void f(X x) {
        constexpr auto y = value(x);
    }

    int main() {
        f(c);
    }
@endcode

This law that must be respected by `Constant`s expresses the minimal
requirement that we're able to retrieve a constant expression from an object,
even if that object isn't a constant expression. There is no restriction on
what the type of the constant expression might be, but it should be
documented. Hana provides a model of this concept called an `Integral`; it
encodes a compile-time value of an integral type, and you can think of it
as a `std::integral_constant`. To create an `Integral`, we use the `integral`
variable template:

@snippet example/tutorial/basic_concepts/constant/integral.cpp integral_create

`integral` is defined roughly as

@code
    template <typename T, T v>
    constexpr unspecified integral{};
@endcode

where the type of `integral<...>`, while being unspecified, actually looks a
lot like a `std::integral_constant`. Indeed, an `Integral` is guaranteed to
have the same members as an equivalent `std::integral_constant`, so they can
be used as such

@snippet example/tutorial/basic_concepts/constant/integral.cpp integral_api

Shorthands to create `Integral`s of the most common integral types are also
provided, so the following is equivalent to the previous example:

@snippet example/tutorial/basic_concepts/constant/integral.cpp integral_shorthands

Additionally, `Integral`s can be created with the help of [user-defined literals]
[Wikipedia.CXX14_udl]:

@snippet example/tutorial/basic_concepts/constant/integral.cpp literals

However, this syntax only allows creating `Integral`s with an underlying value
of type `long long`. Another feature of `Integral`s is that they overload
common operators so that `integral<...>`s can be used with an intuitive
syntax:

@snippet example/tutorial/basic_concepts/constant/integral.cpp integral_operators

It is important to realize that these operators return `Integral`s, not normal
values of an integral type. Specifically, for an operator `@`,

@code
    integral<X, x> @ integral<Y, y> == integral<decltype(x @ y), x @ y>
@endcode

This is very important because it allows all the information that's known at
compile-time (i.e. `Constant`s) to be conserved as long as it's only used with
other values known at compile-time. If we did not encode the result of these
operations in the type of the returned object, we would lose the ability to
e.g. make the return type of a function dependent on that object. It is also
interesting to observe that whenever an `Integral` is used with a normal
runtime value, the result will be a runtime value (because of the implicit
conversion). Hence, we get the following table

left operand | right operand | result
:----------: | :-----------: | :----:
`Constant`   | `Constant`    | `Constant`
`Constant`   | runtime       | runtime
runtime      | `Constant`    | runtime
runtime      | runtime       | runtime


@subsection tutorial-basic_concepts-side_effects Constants and side effects

Let me ask a tricky question. Is the following code valid?

@code
    template <typename X>
    auto identity(X x) { return x; }

    static_assert(value(identity(bool_<true>)), "");
@endcode

The answer is "no", but the reason might not be obvious at first. Even more
puzzling is that the following code is perfectly valid:

@snippet example/tutorial/basic_concepts/constant/side_effects.cpp pure

To understand why the compiler can't possibly evaluate the first assertion
at compile-time, notice that `identity` was not marked `constexpr` and
consider the following alternative (but valid) definition for `identity`:

@snippet example/tutorial/basic_concepts/constant/side_effects.cpp impure_identity

The signature of the function did not change; the function could even have
been defined in a separate source file. However, it is now obvious that the
compiler can't evaluate that expression at compile-time. On the other hand,
when we write

@snippet example/tutorial/basic_concepts/constant/side_effects.cpp impure

we're telling the compiler to perform those potential side effects during the
dynamic initialization phase! Then, we use `value` to return the compile-time
value associated to its argument. Also note that `value` takes a `const&` to
its argument; if it tried taking it by value, we would be reading from a
non-`constexpr` variable to do the copying, and that could hide side-effects.

Normally, this would not be a big issue because the expressions we want to
call `value` with should be constant expressions anyway, so there would be
no need to introduce a dummy variable. However, Hana uses lambdas pretty
heavily, and unfortunately lambdas can't appear in constant expressions in
the current language. Hence, some expressions like

@code
    length(list(1, 2, 3))
@endcode

are not constant expressions because they use lambdas internally. In other
words, while that expression is a `Constant`, it is not `constexpr`.

[constexpr_throw]: http://stackoverflow.com/a/8626450/627587
[GOTW]: http://www.gotw.ca/gotw/index.htm
[Wikipedia.CXX14_udl]: http://en.wikipedia.org/wiki/C%2B%2B11#User-defined_literals
[Wikipedia.generalized_constexpr]: http://en.wikipedia.org/wiki/C%2B%2B11#constexpr_.E2.80.93_Generalized_constant_expressions


@subsection tutorial-basic_concepts-assertions A zoo of assertions

The goal of this section is to explain different types of assertions that you
will encounter in the documentation. While these assertions are not meant to
be used in user code, you will encounter them in the documentation and in the
examples, so it is important to understand their purpose.

@todo
Write this, but how to avoid duplicating the reference documentation?


@subsection tutorial-basic_concepts-comparing Comparing objects

@todo Use Type and List (both compile-time and runtime).


@subsection tutorial-basic_concepts-mapping Transforming collections

@todo Use Maybe and List.


@subsection tutorial-basic_concepts-folding Folding collections

@todo Use Range, List


@subsection tutorial-basic_concepts-searching Searching in collections

@todo Use Map, Set, List


@subsection tutorial-basic_concepts-iteration Iterating through collections

@todo Use List


@subsection tutorial-basic_concepts-list The List type class

@todo Use List


@subsection tutorial-basic_concepts-adapting Adapting user-defined structs

@todo Use a hand-crafted example


@section tutorial-external_libraries Integration with external libraries

------------------------------------------------------------------------------

@subsection tutorial-external_libraries-stl The standard library

@todo


@subsection tutorial-external_libraries-fusion Boost.Fusion

@todo


@subsection tutorial-external_libraries-mpl Boost.MPL

@todo


@section tutorial-performance_considerations Performance considerations

------------------------------------------------------------------------------
@todo


@subsection tutorial-performance_considerations-compile_time Compile-time performance

@todo


@subsection tutorial-performance_considerations-run_time Runtime performance

@todo


@section tutorial-extending Extending the library

------------------------------------------------------------------------------
As advertised, the core of Hana is a powerful compile-time dispatching system
making it painless to extend the library in a ad-hoc manner. The following
sections explain this system and how you can use it to extend the library
without hassle. The dispatching system is built around two core concepts;
_type classes_ and _data types_.


@subsection tutorial-extending-tag_dispatching Basic tag dispatching

Tag dispatching is a generic programming technique for picking the right
implementation of a function depending on the type of the arguments passed to
the function. While this technique is not used as-is in the library, it is
explained here because it forms the conceptual basis for Hana's compile-time
dispatching system.

The usual mechanism for overriding a function's behavior is overloading.
Unfortunately, this mechanism is not always convenient when dealing with
families of related types having different C++ types, or with objects of
unspecified types as is often the case in Hana. For example, consider trying
to overload a function for all Boost.Fusion vectors:

@code
    template <typename ...T>
    void function(boost::fusion::vector<T...> v) {
        // whatever
    }
@endcode

If you know Boost.Fusion, then you probably know that it won't work. This is
because Boost.Fusion vectors are not necessarily specializations of the
`boost::fusion::vector` template. Fusion vectors also exist in numbered forms,
which are all of different types:

@code
    boost::fusion::vector1<T>
    boost::fusion::vector2<T, U>
    boost::fusion::vector3<T, U, V>
    ...
@endcode

This is an implementation detail required by the lack of variadic templates in
C++03 that leaks into the interface. This is unfortunate, but we need a way to
work around it. And it gets worse; I said earlier that `list`, `type` and
other Boost.Hana components did not specify their type at all -- specifying
those types would be too restrictive for the implementation --, so it would be
outright impossible to overload functions for those. To work around this, we
use an infrastructure with three distinct components:

1. A metafunction associating a single tag to every type in a family of
   related types. Hana calls those tags _data types_, and the metafunction
   is called `datatype`. The next section explains these in depth.

2. A function belonging to the public interface of the library, for which
   we'd like to be able to provide a customized implementation.

3. An implementation for the function, parameterized with the data type(s)
   of the argument(s) passed to the function.

When the public interface function is called, it will use the metafunction
on its argument(s) (or a subset thereof) to obtain their data type(s) and
redirect to the implementation associated to those data type(s). For example,
a basic setup for tag dispatching of a function that prints its argument to a
stream would look like:

@snippet tutorial/extending/tag_dispatching.cpp main


@subsection tutorial-extending-datatypes Data types

> __Note__\n
> Since I'm going to use the word "type" a lot, I'll sometimes use the term
> "C++ type" to make it clear that I'm really speaking about the stuff
> returned by `decltype(...)` as opposed to "data types", which are the
> subject of this section. So whenever you see "C++ type", think `decltype`.

Concretely, data types are implemented as empty structs or classes:

@code
    struct MyDatatype;
@endcode

This is very similar to the tags present in Boost.MPL and Boost.Fusion, except
Hana data types are not limited to represent sequences only. To carry the
association between a C++ type and its data type, we use the `datatype`
metafunction, which takes a normal C++ type and returns the data type
associated to it. Since several different C++ types can share a common data
type, we can now associate, for example, all the `boost::fusion::vectorN`s to
a single type representing them all, say `BoostFusionVector`.

There are several ways to specify the data type of a C++ type. If it's a
user-defined type, one can define a nested `hana::dataype` alias inside of it:

@code
    struct MyUserDefinedType {
        struct hana { using datatype = MyDatatype; };
    };
@endcode

Sometimes, however, the C++ type can't be modified (if it's in a foreign
library) or simply can't have nested types (if it's not a struct or class).
In those cases, using a nested alias is impossible and so ad-hoc customization
is also supported by specializing `datatype`:

@code
    struct i_cant_modify_this;

    namespace boost { namespace hana {
        template <>
        struct datatype<i_cant_modify_this> {
            using type = MyDatatype;
        };
    }}
@endcode

`datatype` can also be specialized for all C++ types satisfying some boolean
condition using `when`. `when` accepts a single compile-time boolean and
enables the specialization of `datatype` if and only if that boolean is
`true`. This is similar to the well known C++ idiom of using a dummy template
parameter with `std::enable_if` and relying on SFINAE. For
`boost::fusion::vector`, we would then do

@code
    struct BoostFusionVector;

    namespace boost { namespace hana {
        template <typename T>
        struct datatype<T, when<is_a_boost_fusion_vector<T>::value>> {
            using type = BoostFusionVector;
        };
    }}
@endcode

where `is_a_boost_fusion_vector` is some metafunction returning whether a type
is a Fusion vector.

One last thing about `datatype`; when it is not specialized and when the given
C++ type does not have a nested `hana::dataype` alias, `datatype` returns the
type itself after stripping it from all cv-qualifiers and references. This is
_super_ useful, mainly for two reasons. First, it allows Hana to adopt a
reasonable default behavior for some operations involving types that have no
clue about data types and all that stuff. For example, Hana allows comparing
any two types for which a valid `operator==` is defined out of the box.
Second, it also means that you can ignore data types completely if you don't
need their functionality; just use the normal C++ type of your objects and
everything will "just work".

By the way, `datatype_t<T>` is an alias to `datatype<T>::%type` saving you
from writing `typename datatype<T>::%type` all over the place in dependent
contexts; you can use whichever you prefer.


@subsection tutorial-extending-typeclasses Type classes

Type classes arise from the observation that some groups of operations are
often semantically related. For example, if one were to define the concept of
an object that can be printed to a stream, it would also make sense to provide
a way to get a string representation of that object. Furthermore, given a way
to print an object to a stream, one can obtain a string representation easily
by using a `std::stringstream`, and the converse is also true. Since defining
any one of those operations also provides an implementation for the other, we
would like to be able to only define one to avoid code duplication, while
retaining the ability to override both for efficiency.

Type classes make it possible to do all of this. Concretely, type classes are
just bundles of related operations, called _methods_ or _type class methods_.
Instead of doing per-function dispatching as in the previous section, we
simply dispatch to the implementation of a function inside a type class. For
example, a basic setup for type class dispatching would look like:

@snippet tutorial/typeclasses/typeclass_dispatching.cpp main

Grouping operations into type classes has several advantages. First, it allows
advanced customization of the default behavior of methods -- this will be
explained below. Second, it ties the concepts used in the library -- which
otherwise only live in the documentation -- to actual C++ constructs.

> __Note__\n
> It is interesting to observe that type class dispatching is strictly more
> general than basic tag dispatching, the latter being equivalent to type
> class dispatching where all type classes have a single method.

Let's now explain the above snippet in detail. A type class is just a C++
structure or class calling the `BOOST_HANA_TYPECLASS` macro in its definition
at public scope:

@snippet example/tutorial/typeclasses/printable_0mcd_arithmetic.cpp Printable

The `BOOST_HANA_TYPECLASS` macro creates a nested template named `instance`,
so that name is reserved inside a type class to avoid clashes. Other arbitrary
members can be put in the type class, but it is probably a good idea to keep
anything unrelated out for the sake of separating concerns:

@code
    struct Printable {
        BOOST_HANA_TYPECLASS(Printable);

        // This is valid, but it's probably not a good idea unless those are
        // strongly related to the type class.
        int foo;
        void bar() const { };
        struct baz { };
    };
@endcode

As you will see later with minimal complete definitions, it is often useful to
put other _related_ members in a type class; don't hesitate to do it when it
makes sense. The nested `instance` member of `Printable` is used when
dispatching to the right implementation of a method. It should contain the
method implementations for a specific data type. From now on, I will refer to
the set of all data types for which `Printable::instance` is specialized as
the _instances of `Printable`_, and I will refer to the act of specializing
`Printable::instance` as the act of _instantiating `Printable`_.

To make a type an instance of `Printable`, we must implement the `print`
and `to_string` methods:

@snippet example/tutorial/typeclasses/printable_0mcd_arithmetic.cpp int_instance

Note that we could have chosen a name different from `*_impl` inside the type
class, but this naming convention has the advantage of being clear and
avoiding name clashes inside the instance. In particular, we would not want to
use `Printable::instance<int>::%print`, because using `print` inside the
instance would then refer to the implementation instead of `::%print`, which
is unexpected. This naming convention is used for all the type classes in
Boost.Hana. Now that we have made `int` an instance of `Printable`, we can
write:

@snippet example/tutorial/typeclasses/printable_0mcd_arithmetic.cpp print_int

So far so good, but you probably don't want to write an instance for each
arithmetic type, right?. Fortunately, I didn't want to either so it is
possible to instantiate a type class for all types satisfying a predicate
using `when`, just like for the `datatype` metafunction:

@snippet example/tutorial/typeclasses/printable_0mcd_arithmetic.cpp arithmetic_instance

As expected, we can now write

@snippet example/tutorial/typeclasses/printable_0mcd_arithmetic.cpp print_double

Ok, we managed to cut down the number of instances quite a bit, but we still
can't print a `std::string` without writing an explicit instance for
`std::string`, which is a bummer. Again, laziness won me over and so it is
possible to instantiate a type class for all types making some expression
well-formed (think SFINAE):

@snippet example/tutorial/typeclasses/printable_0mcd_streamable.cpp streamable_instance

`when_valid` is always equivalent to `when<true>`, but it takes an arbitrary
number of types and relies on the fact that SFINAE will kick in and remove the
specialization if any of the types is not well-formed. Note that `when_valid`
can be used when specializing arbitrary metafunctions supporting the `when`
specialization protocol. As expected, we can now write

@snippet example/tutorial/typeclasses/printable_0mcd_streamable.cpp print_string

Note that instances provided without `when` (i.e. an explicit or partial
specialization in the case of a parametric data type) have the priority over
instances provided with it. This is to allow types to instantiate a type class
even if an instance for the same type class is provided through a predicate.
This design choice was made assuming that a specialization (even partial)
is usually meant to be more specific than a catch-all instance enabled by
a predicate.

All is good so far, but notice how we have to write an implementation for
`to_string` every time, and how the implementation is always the same. It
should be possible to remove this code duplication, and it is indeed. To do
so, we introduce the notion of a _minimial complete definition_, often
abbreviated _MCD_. The minimal complete definition of a type class is the
minimal set of methods that must be implemented to instantiate it. In the case
of `Printable`, we can always implement `to_string` in terms of `print`, so
`print` is a MCD of `Printable`. To avoid code duplication, the default
implementation of `to_string` is provided as a member of the type class and is
inheritted by instances that would like to get that default implementation:

@snippet example/tutorial/typeclasses/printable_1mcd.cpp Printable

> __Note__\n
> For simplicity, the term minimal complete definition can refer either to
> a minimal set of required methods or to the member of the type class
> providing the corresponding default implementations.

It is possible for a type class to have several minimal complete definitions.
For example, one could observe that `print` can also be implemented in terms
of `to_string`. If we wanted to do so, we could provide both minimal complete
definitions by putting them into suitably named members of the `Printable`
type class:

@snippet example/tutorial/typeclasses/printable_2mcd.cpp Printable

Either minimal complete definitions could now be used to instantiate
`Printable`. By convention, in Boost.Hana, the minimal complete definition
is always named `mcd` when there is a single one. A nested type named `mcd`
is also provided when there are no default implementations to provide for
consistency and for extensibility, as will be explained next. If there is
more than one possible mcd, each mcd is in a different nested type with a
descriptive name. In all cases, the minimal complete definition(s) are
documented.

It is recommended to always inherit from a minimal complete definition, even
when the default implementations are not actually used:

@snippet example/tutorial/typeclasses/printable_2mcd.cpp string_instance

This allows methods to be added to the type class without breaking the
instance, provided the type class does not change its minimal complete
definition(s). This is the reason why a minimal complete definition is
always provided, even when it is empty.

To show the full power of type classes and introduce the `is_a` utility, let's
define a `Printable` instance for `std::vector`s containing any `Printable`
type.

@snippet example/tutorial/typeclasses/printable_2mcd.cpp vector_instance

`is_a` is a variable template taking a type class and a data type and
returning whether the data type is an instance of the given type class. The
result is returned as a boolean `Integral`, which explains the trailing `()`
to convert the result to a `bool` at compile-time. We can now print nested
containers:

@snippet example/tutorial/typeclasses/printable_2mcd.cpp print_vector

`is_a` can also be used as a function object and with more than one data type;
see the [reference documentation](@ref group-core-is_a) for details. Another
thing that can be done with type classes is to provide a default instance for
all data types. To do so, a nested `default_instance` template must be defined
in the type class:

@snippet example/tutorial/typeclasses/printable_default_instance.cpp Printable

`default_instance` should be just like a normal instance. Note that this
feature should seldom be used because methods with a meaningful behavior
for all data types are rare. This feature is provided for flexibility, but
it should be a hint to reconsider your type class design if you are about
to use it. Anyway, with such a default type class, it is now possible to
print anything, but it might not always be meaningful:

@snippet example/tutorial/typeclasses/printable_default_instance.cpp print_foo

One last thing that can be done with type classes is to explicitly disable
instances. This can be useful if, for example, an instance for `Printable`
is provided for some type but you would prefer to have a compile-time error
if one tries to print an object of that type. To disable the faulty instance,
simply specialize it and inherit from `disable`:

@snippet example/tutorial/typeclasses/printable_default_instance.cpp disable

Now, trying to print an object of data type `UserDefined` will result in a
compile-time error.


@todo
- Document type classes with operators.
- Document how to instantiate _unary_ type classes from inside a data type.


@section tutorial-header_organization Header organization

------------------------------------------------------------------------------
The library is designed to be modular while keeping the number of headers that
must be included to get basic functionality reasonably low. The structure of
the library was also intentionally kept simple, because we all love simplicity.

@todo
Try to merge this with the file list generated by Doxygen.

- `boost/hana.hpp`\n
  This is the master header of the library. It includes the whole public
  interface of the library except adapters for external libraries, which
  must be included separately.

- `boost/hana/`\n
  This is the main directory of the library containing the definitions of
  type classes and data types. A file of the form `boost/hana/[XXX].hpp`
  contains the definition for the type class or data type named `XXX`.
  In the case of a type class, this includes all the minimal complete
  definitions, the provided instances and the operators, if any.
  In the case of a data type, this includes all the type class instances
  provided for this data type as well as any related method that does not
  belong to a type class.

  - `boost/hana/core/`\n
    This subdirectory contains the machinery for type class dispatching and
    other related utilities that do not belong to type classes, like `make`
    and `convert`.

  - `boost/hana/fwd/`\n
    This subdirectory contains the forward declaration of every type class
    and data type in the library. Basically, `boost/hana/fwd/[XXX].hpp`
    is the forward declaration for the type class or data type named `XXX`.

  - `boost/hana/functional/`\n
    This subdirectory contains various function objects that are often useful,
    but that do not necessarily belong to a type class.

  - `boost/hana/ext/`\n
    This directory contains adapters for external libraries. This is the only
    part of the public interface which is not included by the master header,
    because that would make the master header dependent on those external
    libraries. Note that only the strict minimum required to adapt the
    external components is included in these headers (e.g. a forward
    declaration). This means that the definition of the external component
    should still be included when one wants to use it. For example:

    @snippet example/tutorial/organization/include_ext.cpp main

  - `boost/hana/sandbox/`\n
    This directory contains experimental code on which no guarantee whatsoever
    is made. It might not even compile and it will definitely not be stable.

  - `boost/hana/detail/`\n
    This directory contains utilities required internally. Nothing in `detail/`
    is guaranteed to be stable, so you should not use it.

### Example
Let's say I want to include `set`. I only have to include its header and I
can use all the methods it supports right away:

@snippet example/tutorial/organization/include_set.cpp main


@section tutorial-mastering Mastering the library

------------------------------------------------------------------------------
You now have everything you need to start using the library. From here on,
mastering the library is only a matter of understanding and knowing how to
use the general purpose type classes and data types provided with it, which
is best done by looking at the reference documentation. At some point, you
will probably also want to create your own type classes and data types that
fit your needs better; go ahead, the library was intended to be used that way.

The structure of the reference (available in the menu to the left) goes as
follow:
  - @ref group-core\n
    Documentation for the core module, which contains everything needed to
    implement type classes, data types and related utilities. This is relevant
    if you need to extend the library, but otherwise the tutorial pretty much
    covered it all.

  - @ref group-functional\n
    General purpose function objects that are generally useful in a purely
    functional setting. These are not tied to any type class or data type
    (currently).

  - @ref group-typeclasses\n
    Documentation for all the type classes provided with the library. Each
    type class is documented as follows:
      - The methods it provides
      - The minimal complete definition(s) required to instantiate it
      - The laws that must be respected by its instances
      - Any additional type class specific information

  - @ref group-datatypes\n
    Documentation for all the data types provided with the library. Each
    data type is documented as follows:
      - The type classes it instantiates, and how it does so
      - Methods tied to the data type but not to any type class
      - Any data type specific information

  - @ref group-config\n
    Macros that can be used to tweak the global behavior of the library.

  - @ref group-assertions\n
    Macros to perform various types of assertions.

  - @ref group-details\n
    Why did I document implementation details? That's a good question.

I hope you enjoy using the library as much as I enjoyed writing it!

-- Louis


<!-- Links -->
[Boost.Fusion]: http://www.boost.org/doc/libs/release/libs/fusion/doc/html/index.html
[Boost.MPL]: http://www.boost.org/doc/libs/release/libs/mpl/doc/index.html
[C++Now]: http://cppnow.org
[GSoC]: http://www.google-melange.com/gsoc/homepage/google/gsoc2014
[MPL11]: http://github.com/ldionne/mpl11
[Wikipedia.C++14]: http://en.wikipedia.org/wiki/C%2B%2B14
[Wikipedia.generic_lambda]: http://en.wikipedia.org/wiki/C%2B%2B14#Generic_lambdas
[Wikipedia.variable_template]: http://en.wikipedia.org/wiki/C%2B%2B14#Variable_templates

 */

#endif // !BOOST_HANA_HPP
