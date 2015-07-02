/*!
@file
Forward declares `boost::hana::Sequence`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CONCEPT_SEQUENCE_HPP
#define BOOST_HANA_FWD_CONCEPT_SEQUENCE_HPP

#include <boost/hana/fwd/cartesian_product.hpp>
#include <boost/hana/fwd/drop_back.hpp>
#include <boost/hana/fwd/drop_back_exactly.hpp>
#include <boost/hana/fwd/group.hpp>
#include <boost/hana/fwd/insert.hpp>
#include <boost/hana/fwd/insert.hpp>
#include <boost/hana/fwd/insert_range.hpp>
#include <boost/hana/fwd/intersperse.hpp>
#include <boost/hana/fwd/partition.hpp>
#include <boost/hana/fwd/permutations.hpp>
#include <boost/hana/fwd/remove_at.hpp>
#include <boost/hana/fwd/reverse.hpp>
#include <boost/hana/fwd/scan_left.hpp>
#include <boost/hana/fwd/scan_right.hpp>
#include <boost/hana/fwd/slice.hpp>
#include <boost/hana/fwd/sort.hpp>
#include <boost/hana/fwd/span.hpp>
#include <boost/hana/fwd/subsequence.hpp>
#include <boost/hana/fwd/take.hpp>
#include <boost/hana/fwd/take_exactly.hpp>
#include <boost/hana/fwd/take_until.hpp>
#include <boost/hana/fwd/take_while.hpp>
#include <boost/hana/fwd/unfold_left.hpp>
#include <boost/hana/fwd/unfold_right.hpp>
#include <boost/hana/fwd/unique.hpp>
#include <boost/hana/fwd/zip.hpp>
#include <boost/hana/fwd/zip_shortest.hpp>
#include <boost/hana/fwd/zip_shortest_with.hpp>
#include <boost/hana/fwd/zip_with.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Sequence` concept represents generic index-based sequences.
    //!
    //! Compared to other abstract concepts, the Sequence concept is very
    //! specific. It represents generic index-based sequences. The reason
    //! why such a specific concept is provided is because there are a lot
    //! of models that behave exactly the same while being implemented in
    //! wildly different ways. It is useful to regroup all those data types
    //! under the same umbrella for the purpose of generic programming.
    //!
    //! In fact, models of this concept are not only _similar_. They are
    //! actually _isomorphic_, in a sense that we define below, which is
    //! a fancy way of rigorously saying that they behave exactly the same
    //! to an external observer.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! `Iterable`, `empty`, `prepend`, `models`
    //!
    //! The `Sequence` concept does not provide basic methods that could be
    //! used as a minimal complete definition; instead, it borrows methods from
    //! other concepts and add laws to them. For this reason, it is necessary
    //! to specialize the `models` metafunction in the `boost::hana` namespace
    //! in addition to defining the above methods. Explicitly specializing the
    //! `models` metafunction can be seen like a seal saying "this data type
    //! satisfies the additional laws of a `Sequence`", since those can't be
    //! checked by Hana automatically.
    //!
    //!
    //! Laws
    //! ----
    //! For any Sequence data type `S`, the `to<Tuple>` conversion from `S`
    //! (as a Foldable, see below) must be a natural isomorphism. Furthermore,
    //! it must be the unique (up to implementation) bijective natural
    //! MonadPlus and Iterable isomorphism between `Tuple` and `S`.
    //! Intuitively, this means that all Sequences act exactly like `Tuple`s,
    //! but their implementation may differ. This is ensured by stating that
    //! conversion to and from a `Tuple` preserves both information quantity
    //! and organization.
    //!
    //! First, information quantity is preserved by requiring `to<Tuple>`
    //! to be bijective. Hence, the `S` and `Tuple` data types contain the
    //! same amount of objects, and information quantity is preserved. Note
    //! that we explicitly require the isomorphism to be bijective because
    //! [not all isomorphisms are bijective][1].
    //!
    //! Then, information organization is preserved by requiring `to<Tuple>`
    //! to be the unique natural MonadPlus isomorphism between `Tuple` and
    //! `S`. Since everything in Sequence is implemented in terms of MonadPlus,
    //! Iterable and other refined concepts, this effectively gives us laws
    //! that must be respected for the methods of Sequence. The result is that
    //! for any Sequence `xs` of data type `S` and any n-ary function `f`
    //! (suppose without loss of generality that `f` takes its Sequence
    //! argument in the first parameter),
    //! @code
    //!     to<Tuple>(f(xs, -, ..., -)) == f(to<Tuple>(xs), -, ..., -)
    //! @endcode
    //!
    //! If `f` does not return a Sequence, then simply change the above for
    //! @code
    //!     f(xs, -, ..., -) == f(to<Tuple>(xs), -, ..., -)
    //! @endcode
    //!
    //! Here, the notation `f(xs, -, ..., -)` denotes the partial application
    //! of the function `f` to its `xs` argument, with all the other arguments
    //! left unbound. Hence, these comparisons are comparisons between
    //! functions, and they express the fact that any Sequence is just
    //! as good as a `Tuple` for an external observer (the function `f`).
    //!
    //!
    //! Refined concepts
    //! ----------------
    //! 1. `Comparable` (definition provided automatically)\n
    //! Two `Sequence`s are equal if and only if they contain the same number
    //! of elements and their elements at any given index are equal.
    //! @snippet example/sequence.cpp Comparable
    //!
    //! 2. `Orderable` (definition provided automatically)\n
    //! `Sequence`s are ordered using the traditional lexicographical ordering.
    //! @snippet example/sequence.cpp Orderable
    //!
    //! 3. `Functor` (definition provided automatically)\n
    //! `Sequence`s implement `transform` as the mapping of a function over
    //! each element of the sequence. This is somewhat equivalent to what
    //! `std::transform` does to ranges of iterators. Also note that mapping
    //! a function over an empty sequence returns an empty sequence and never
    //! applies the function, as would be expected.
    //! @snippet example/sequence.cpp Functor
    //!
    //! 4. `Applicative` (definition provided automatically)\n
    //! First, `lift`ing a value into a `Sequence` is the same as creating a
    //! singleton sequence containing that value. Second, applying a sequence
    //! of functions to a sequence of values will apply each function to
    //! all the values in the sequence, and then return a list of all the
    //! results. In other words,
    //! @code
    //!     ap([f1, ..., fN], [x1, ..., xM]) == [
    //!         f1(x1), ..., f1(xM),
    //!         ...
    //!         fN(x1), ..., fN(xM)
    //!     ]
    //! @endcode
    //! Example:
    //! @snippet example/sequence.cpp Applicative
    //!
    //! 5. `Monad` (definition provided automatically)\n
    //! First, `flaten`ning a `Sequence` takes a sequence of sequences and
    //! concatenates them to get a larger sequence. In other words,
    //! @code
    //!     flatten([[a1, ..., aN], ..., [z1, ..., zM]]) == [
    //!         a1, ..., aN, ..., z1, ..., zM
    //!     ]
    //! @endcode
    //! This acts like a `std::tuple_cat` function, except it receives a
    //! sequence of sequences instead of a variadic pack of sequences to
    //! flatten.\n
    //! __Example__:
    //! @snippet example/sequence.cpp Monad.ints
    //!
    //! Also note that the model of `Monad` for `Sequence`s can be seen as
    //! modeling nondeterminism. A nondeterministic computation can be
    //! modeled as a function which returns a sequence of possible results.
    //! In this line of thought, `chain`ing a sequence of values into such
    //! a function will return a sequence of all the possible output values,
    //! i.e. a sequence of all the values applied to all the functions in
    //! the sequences.\n
    //! __Example__:
    //! @snippet example/sequence.cpp Monad.types
    //!
    //! 6. `MonadPlus` (minimal complete definition modified)\n
    //! `Sequence`s are models of the `MonadPlus` concept by considering the
    //! empty sequence as the unit of `concat`, and sequence concatenation
    //! as the combining operation. Note that the minimal complete definition
    //! of `Sequence` includes the `prepend` and the `empty` methods, which
    //! are part of the `MonadPlus` concept. When those methods are provided,
    //! the `concat` method is automatically defined and hence it is not
    //! strictly required to implement it in order to model `MonadPlus`.
    //! @snippet example/sequence.cpp MonadPlus
    //!
    //! 7. `Foldable` (definition provided automatically)\n
    //! The model of `Foldable` for `Sequence`s is uniquely determined by the
    //! model of `Iterable`. The automatically provided model is the one
    //! provided by the `Iterable` concept.
    //! @snippet example/sequence.cpp Foldable
    //!
    //! 8. `Iterable`\n
    //! The model of `Iterable` for `Sequence`s corresponds to iteration over
    //! each element of the sequence, in order.
    //! @snippet example/sequence.cpp Iterable
    //!
    //! 9. `Searchable` (definition provided automatically)\n
    //! Searching through a `Sequence` is equivalent to just searching through
    //! a list of the values it contains. The keys and the values on which
    //! the search is performed are both the elements of the sequence.
    //! @snippet example/sequence.cpp Searchable
    //!
    //!
    //! Concrete models
    //! ---------------
    //! `Tuple`
    //!
    //!
    //! Free `make` method
    //! ------------------
    //! For any `Sequence` `S`, the `make<S>` method is defined
    //! automatically as
    //! @code
    //!     make<S>(x1, ..., xn) == fold_right(make<Tuple>(x1, ..., xn), empty<S>(), flip(prepend))
    //!                          == [x1, ..., xn] // of data type S
    //! @endcode
    //!
    //! While this definition is correct, it can be compile-time inefficient.
    //! Hence, implementers of new sequences are encouraged to override this
    //! default definition.
    //!
    //!
    //! [1]: http://en.wikipedia.org/wiki/Isomorphism#Isomorphism_vs._bijective_morphism
    struct Sequence {
        template <typename T, typename U> struct equal_impl;
        template <typename T, typename U> struct less_impl;

        template <typename S> struct lift_impl;
        template <typename S> struct ap_impl;
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CONCEPT_SEQUENCE_HPP
