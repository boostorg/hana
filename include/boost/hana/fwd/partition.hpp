/*!
@file
Forward declares `boost::hana::partition`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_PARTITION_HPP
#define BOOST_HANA_FWD_PARTITION_HPP

#include <boost/hana/core/when.hpp>
#include <boost/hana/detail/by_fwd.hpp>



namespace boost { namespace hana {
    //! Partition a sequence based on a `predicate`.
    //! @relates Sequence
    //!
    //! Specifically, returns an unspecified `Product` whose first element is
    //! a sequence of the elements satisfying the predicate, and whose second
    //! element is a sequence of the elements that do not satisfy the predicate.
    //!
    //!
    //! Signature
    //! ---------
    //! Given a Sequence `S(T)`, a `Logical` `Bool` and a predicate
    //! \f$ T \to Bool \f$, `partition` has the following signature:
    //! \f[
    //!     \mathtt{partition} : S(T) \times (T \to Bool) \to S(T) \times S(T)
    //! \f]
    //!
    //! @param xs
    //! The sequence to be partitioned.
    //!
    //! @param predicate
    //! A function called as `predicate(x)` for each element `x` in the
    //! sequence and returning a `Constant Logical`. If the result of
    //! `predicate` is true, then `x` is added to the sequence in the first
    //! component of the resulting `Product`. Otherwise, `x` is added to the
    //! sequence in the second component.
    //!
    //! ### Example
    //! @snippet example/sequence.cpp partition
    //!
    //!
    //! Syntactic sugar (`partition.by`)
    //! --------------------------------
    //! `partition` can be called in an alternate way, which provides a nice
    //! syntax in some cases where the predicate is short:
    //! @code
    //!     partition.by(predicate, xs) == partition(xs, predicate)
    //!     partition.by(predicate) == partition(-, predicate)
    //! @endcode
    //!
    //! where `partition(-, predicate)` denotes the partial application of
    //! `partition` to `predicate`.
    //!
    //! ### Example
    //! @snippet example/sequence.cpp partition.by
    //!
    //!
    //! Benchmarks
    //! ----------
    //! <div class="benchmark-chart"
    //!      style="min-width: 310px; height: 400px; margin: 0 auto"
    //!      data-dataset="benchmark.partition.compile.json">
    //! </div>
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto partition = [](auto&& xs, auto&& predicate) {
        return tag-dispatched;
    };
#else
    template <typename S, typename = void>
    struct partition_impl : partition_impl<S, when<true>> { };

    struct partition_t : detail::by<partition_t> {
        template <typename Xs, typename Pred>
        constexpr auto operator()(Xs&& xs, Pred&& pred) const;
    };

    constexpr partition_t partition{};
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_PARTITION_HPP
