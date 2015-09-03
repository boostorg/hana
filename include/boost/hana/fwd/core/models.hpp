/*!
@file
Forward declares `boost::hana::models`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_CORE_MODELS_HPP
#define BOOST_HANA_FWD_CORE_MODELS_HPP

#include <boost/hana/core/when.hpp>


namespace boost { namespace hana {
    //! @ingroup group-core
    //! Returns whether the tag of an object is a model of the given concept.
    //!
    //! Given a C++ type `T` and a concept `Concept`, `models<Concept, T>`
    //! is a compile-time Logical representing whether `T`'s tag is a model
    //! of `Concept`. Usually, concepts provide methods, some of which
    //! __must__ be implemented in order to model the concept, and the others
    //! are given a default definition in terms of the mandatory ones. More
    //! specifically, `models` returns whether a minimal complete definition
    //! of a concept is provided by the tag. However, concepts also usually
    //! provide semantic requirements (called laws) that must be satisfied by
    //! their models; `models` does not attempt to check whether those laws
    //! are satisfied. In fact, `models` does not even check whether the
    //! provided minimal complete definition is syntactically correct, but
    //! only if it is there.
    //!
    //! A few concepts (like Sequence) do not introduce methods that suffice
    //! to provide a minimal complete definition. They provide semantic
    //! guarantees through laws and refined concepts, and they usually also
    //! provide methods that do useful things with existing methods and
    //! those additional guarantees. In this case, it is usually necessary
    //! to explicitly specialize `models_impl` in the `boost::hana` namespace.
    //! When this is the case, the concept should document it.
    //!
    //! @note
    //! `models` is a tag-dispatched method. More specifically,
    //! `models<Concept, T>` dispatches to `models_impl<Concept, tag_of_t<T>>`,
    //! which should be a metafunction with a nested boolean `::value`
    //! representing whether `tag_of_t<T>` models the concept `Concept`.
    //!
    //!
    //! Example
    //! -------
    //! @include example/core/models.cpp
    //!
    //!
    //! How does it work
    //! ----------------
    //! When a concept writer provides minimal complete definitions in terms
    //! of other methods introduced by the concept, they specialize the
    //! `models_impl` metafunction so that whenever a proper minimal complete
    //! definition is provided by the user, `models` returns true. Here's
    //! an example of how a simple concept could support `models`:
    //!
    //! @include example/core/models.new.cpp
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename Concept, typename optional_T>
    constexpr auto models = see-documentation;
#else
    template <typename Concept, typename DataType, typename = void>
    struct models_impl : models_impl<Concept, DataType, when<true>> { };

    template <typename Concept, typename ...T>
    struct _models;

    template <typename Concept, typename ...T>
    constexpr _models<Concept, T...> models{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_CORE_MODELS_HPP
