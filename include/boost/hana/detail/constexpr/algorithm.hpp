/*!
@file
Defines several `constexpr` algorithms.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_DETAIL_CONSTEXPR_ALGORITHM_HPP
#define BOOST_HANA_DETAIL_CONSTEXPR_ALGORITHM_HPP

#include <boost/hana/detail/std/move.hpp>
#include <boost/hana/functional/placeholder.hpp>


namespace boost { namespace hana { namespace detail { namespace constexpr_ {
    template <typename T>
    constexpr void swap(T& x, T& y) {
        auto tmp = x;
        x = y;
        y = detail::std::move(tmp);
    }

    template <typename BidirIter>
    constexpr void reverse(BidirIter first, BidirIter last) {
        while (first != last) {
            if (first == --last)
                break;
            constexpr_::swap(*first, *last);
            ++first;
        }
    }

    template <typename BidirIter, typename BinaryPred>
    constexpr bool next_permutation(BidirIter first, BidirIter last,
                                    BinaryPred pred)
    {
        BidirIter i = last;
        if (first == last || first == --i)
            return false;
        while (true) {
            BidirIter ip1 = i;
            if (pred(*--i, *ip1)) {
                BidirIter j = last;
                while (!pred(*i, *--j))
                    ;
                constexpr_::swap(*i, *j);
                constexpr_::reverse(ip1, last);
                return true;
            }
            if (i == first) {
                constexpr_::reverse(first, last);
                return false;
            }
        }
    }

    template <typename BidirIter>
    constexpr bool next_permutation(BidirIter first, BidirIter last)
    { return next_permutation(first, last, _ < _); }


    template <typename InputIter1, typename InputIter2, typename BinaryPred>
    constexpr bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                                           InputIter2 first2, InputIter2 last2,
                                           BinaryPred pred)
    {
        for (; first2 != last2; ++first1, ++first2) {
            if (first1 == last1 || pred(*first1, *first2))
                return true;
            else if (pred(*first2, *first1))
                return false;
        }
        return false;
    }

    template <typename InputIter1, typename InputIter2>
    constexpr bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
                                           InputIter2 first2, InputIter2 last2)
    { return lexicographical_compare(first1, last1, first2, last2, _ < _); }


    template <typename InputIter1, typename InputIter2, typename BinaryPred>
    constexpr bool equal(InputIter1 first1, InputIter1 last1,
                         InputIter2 first2, InputIter2 last2,
                         BinaryPred pred)
    {
        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
            if (!pred(*first1, *first2))
                return false;
        return first1 == last1 && first2 == last2;
    }

    template <typename InputIter1, typename InputIter2>
    constexpr bool equal(InputIter1 first1, InputIter1 last1,
                         InputIter2 first2, InputIter2 last2)
    { return equal(first1, last1, first2, last2, _ == _); }


    template <typename BidirIter, typename BinaryPred>
    constexpr void sort(BidirIter first, BidirIter last, BinaryPred pred) {
        if (first == last) return;

        BidirIter i = first;
        for (++i; i != last; ++i) {
            BidirIter j = i;
            auto t = *j;
            for (BidirIter k = i; k != first && pred(t,  *--k); --j)
                *j = *k;
            *j = t;
        }
    }

    template <typename BidirIter>
    constexpr void sort(BidirIter first, BidirIter last)
    { sort(first, last, _ < _); }


    template <typename InputIter, typename T>
    constexpr InputIter find(InputIter first, InputIter last, T const& value) {
        for (; first != last; ++first)
            if (*first == value)
                return first;
        return last;
    }

    template <typename InputIter, typename UnaryPred>
    constexpr InputIter find_if(InputIter first, InputIter last, UnaryPred /*pred*/) {
        for (; first != last; ++first)
            if (pred(*first))
                return first;
        return last;
    }

    template <typename ForwardIter, typename T>
    constexpr void iota(ForwardIter first, ForwardIter last, T value) {
        while (first != last) {
            *first++ = value;
            ++value;
        }
    }
}}}} // end namespace boost::hana::detail::constexpr_

#endif // !BOOST_HANA_DETAIL_CONSTEXPR_ALGORITHM_HPP
