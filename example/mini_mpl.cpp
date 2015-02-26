/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana.hpp>
#include <boost/hana/ext/boost/mpl.hpp>
#include <boost/hana/ext/std.hpp>

#include <boost/mpl/lambda.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/quote.hpp>

#include <iostream>
#include <type_traits>
namespace hana = boost::hana;
namespace mpl = boost::mpl;


//////////////////////////////////////////////////////////////////////////////
// This is a mini reimplementation of the MPL library.
//
// The goal is to be as backward compatible as possible with the MPL, while
// still using Hana under the hood. Only the "Algorithms" part of the MPL is
// implemented as a case study, but it should be possible to implement many
// (but not all) other metafunctions of the MPL.
//
// Scroll down to the main function to see the tests. The tests are exactly
// the examples in the MPL documentation that were copy/pasted and then
// modified as little as possible to work with this reimplementation.
//////////////////////////////////////////////////////////////////////////////


namespace hpl
{

//////////////////////////////////////////////////////////////////////////////
// Utilities
// Most of this should be abstracted and then lifted into Hana.
//////////////////////////////////////////////////////////////////////////////

// Equivalent to `trait` for metafunction classes.
template <typename F>
struct _boolean_metafunction_class {
    template <typename ...T>
    constexpr decltype(auto) operator()(T ...t) const {
        using L = typename mpl::lambda<F>::type;
        return typename L::template apply<typename T::type...>::type{};
    }
};

template <typename F>
constexpr _boolean_metafunction_class<F> boolean_metafunction_class{};

// unique{_by} should be implemented in Hana. Also note that this can
// certainly be made more efficient.
template <typename Predicate, typename Sequence>
constexpr decltype(auto) unique_by(Predicate predicate, Sequence sequence) {
    return hana::transform(hana::group_by(predicate, sequence), hana::head);
}


//////////////////////////////////////////////////////////////////////////////
// Sequences, compile-time integers & al
//
// Differences with the MPL:
// 1. No more goddamn numbered variants
// 2. These are dependent types, so no pattern matching. This can be modified
//    easily.
// 3. `pair<...>::first` and `pair<...>::second` won't work;
//    use `first<pair<...>>` instead
//////////////////////////////////////////////////////////////////////////////
template <typename ...T>
using vector = decltype(hana::tuple_t<T...>);

template <typename T, T v>
using integral_c = decltype(hana::integral_constant<T, v>);

template <typename T, T ...v>
using vector_c = decltype(hana::tuple_c<T, v...>);

template <typename T, T from, T to>
using range_c = decltype(hana::range_c<T, from, to>);

template <int i>
using int_ = integral_c<int, i>;

template <bool b>
using bool_ = integral_c<bool, b>;

template <typename T, typename U>
struct pair : decltype(hana::pair(hana::type<T>, hana::type<U>)) { };

template <typename P>
struct first : decltype(hana::first(P{})) { };

template <typename P>
struct second : decltype(hana::second(P{})) { };


//////////////////////////////////////////////////////////////////////////////
// Miscellaneous metafunctions
//////////////////////////////////////////////////////////////////////////////
template <typename C1, typename C2>
struct equal_to
    : bool_<C1::value == C2::value>
{ };

template <typename C1, typename C2>
struct less
    : bool_<(C1::value < C2::value)>
{ };

template <typename C1, typename C2>
struct greater
    : bool_<(C1::value > C2::value)>
{ };

template <typename N>
struct next
    : integral_c<typename N::value_type, N::value + 1>
{ };

//////////////////////////////////////////////////////////////////////////////
// Intrinsics
//////////////////////////////////////////////////////////////////////////////
template <typename Sequence, typename T>
struct push_front {
    using type = decltype(hana::prepend(hana::type<T>, Sequence{}));
};

template <typename Sequence>
struct size
    : decltype(hana::length(Sequence{}))
{ };

//////////////////////////////////////////////////////////////////////////////
// Iteration algorithms
//
// Differences with the MPL:
// 1. reverse_fold:
//    Does not take an optional additional ForwardOp argument.
//
// 2. iter_fold, reverse_iter_fold:
//    Not implemented because we don't use iterators
//////////////////////////////////////////////////////////////////////////////
template <typename Sequence, typename State, typename F>
struct fold
    : decltype(hana::foldl(
        Sequence{}, hana::type<State>, hana::metafunction_class<
            typename mpl::lambda<F>::type
        >
    ))
{ };

template <typename Sequence, typename State, typename F>
struct reverse_fold
    : decltype(hana::foldr(
        Sequence{}, hana::type<State>, hana::flip(hana::metafunction_class<
            typename mpl::lambda<F>::type
        >)
    ))
{ };

template <typename Sequence, typename State, typename F>
using accumulate = fold<Sequence, State, F>;

//////////////////////////////////////////////////////////////////////////////
// Query algorithms
//
// Differences with the MPL:
// 1. find_if and find:
//    Not returning an iterator; instead it returns the type itself and
//    bails out if the type was not found. We could do better by providing
//    a MPL-friendly interface to Maybe. We could also provide a function to
//    find the index of a given element in an Iterable. This couldn't be in
//    Searchable, because those have no notion of index.
//
// 2. lower_bound, upper_bound:
//    Not implemented.
//
// 3. {min,max}_element:
//    Not returning an iterator, and also won't work on empty sequences.
//////////////////////////////////////////////////////////////////////////////
template <typename Sequence, typename Pred>
struct find_if
    : decltype(hana::from_just(hana::find(
        Sequence{}, boolean_metafunction_class<Pred>
    )))
{ };

template <typename Sequence, typename T>
struct find
    : decltype(hana::from_just(hana::find(
        Sequence{}, hana::equal.to(hana::type<T>)
    )))
{ };

template <typename Sequence, typename T>
struct contains
    : decltype(hana::elem(Sequence{}, hana::type<T>))
{ };

template <typename Sequence, typename T>
struct count
    : decltype(hana::count(Sequence{}, hana::equal.to(hana::type<T>)))
{ };

template <typename Sequence, typename Pred>
struct count_if
    : decltype(hana::count(Sequence{}, boolean_metafunction_class<Pred>))
{ };

template <typename Sequence, typename Pred = mpl::quote2<less>>
struct min_element
    : decltype(hana::minimum_by(boolean_metafunction_class<Pred>, Sequence{}))
{ };

template <typename Sequence, typename Pred = mpl::quote2<less>>
struct max_element
    : decltype(hana::maximum_by(boolean_metafunction_class<Pred>, Sequence{}))
{ };

template <typename S1, typename S2, typename Pred = mpl::quote2<std::is_same>>
struct equal
    : decltype( // inefficient but whatever
        hana::length(S1{}) == hana::length(S2{}) &&
        hana::all_of(hana::zip.with(boolean_metafunction_class<Pred>,
                hana::to<hana::Tuple>(S1{}),
                hana::to<hana::Tuple>(S2{})))
    )
{ };

//////////////////////////////////////////////////////////////////////////////
// Transformaton algorithms
//
// Differences from the MPL:
// 1. The algorithms do not accept an optional inserter, and they always return
//    a `vector`.
// 2. stable_partition: not implemented
// 3. All the reverse_* algorithms are not implemented.
//////////////////////////////////////////////////////////////////////////////
template <typename Sequence>
struct copy {
    using type = decltype(hana::to<hana::Tuple>(Sequence{}));
};

template <typename Sequence, typename Pred>
struct copy_if {
    using type = decltype(hana::filter(
        hana::to<hana::Tuple>(Sequence{}), boolean_metafunction_class<Pred>
    ));
};

template <typename Sequence, typename Sequence_or_Op, typename = void>
struct transform;

template <typename Sequence, typename Op>
struct transform<Sequence, Op> {
    using type = decltype(hana::transform(
        hana::to<hana::Tuple>(Sequence{}),
        hana::metafunction_class<typename mpl::lambda<Op>::type>
    ));
};

template <typename S1, typename S2, typename Op>
struct transform {
    using type = decltype(hana::zip.with(
        hana::metafunction_class<typename mpl::lambda<Op>::type>,
        hana::to<hana::Tuple>(S1{}),
        hana::to<hana::Tuple>(S2{})
    ));
};

template <typename Sequence, typename OldType, typename NewType>
struct replace {
    using type = decltype(hana::replace(
        hana::to<hana::Tuple>(Sequence{}),
        hana::equal.to(hana::type<OldType>),
        hana::type<NewType>
    ));
};

template <typename Sequence, typename Pred, typename NewType>
struct replace_if {
    using type = decltype(hana::replace(
        hana::to<hana::Tuple>(Sequence{}),
        boolean_metafunction_class<Pred>,
        hana::type<NewType>
    ));
};

template <typename Sequence, typename T>
struct remove {
    using type = decltype(hana::filter(
        hana::to<hana::Tuple>(Sequence{}),
        hana::not_equal.to(hana::type<T>)
    ));
};

template <typename Sequence, typename Pred>
struct remove_if {
    using type = decltype(hana::filter(
        hana::to<hana::Tuple>(Sequence{}),
        hana::compose(hana::not_, boolean_metafunction_class<Pred>)
    ));
};

template <typename Sequence, typename Pred>
struct unique {
    using type = decltype(unique_by(
        boolean_metafunction_class<Pred>, hana::to<hana::Tuple>(Sequence{})
    ));
};

template <typename Sequence, typename Pred>
struct partition {
    using hana_pair = decltype(hana::partition(
        hana::to<hana::Tuple>(Sequence{}),
        boolean_metafunction_class<Pred>
    ));
    using type = pair<
        decltype(hana::first(hana_pair{})),
        decltype(hana::second(hana_pair{}))
    >;
};

template <typename Sequence, typename Pred = mpl::quote2<less>>
struct sort {
    using type = decltype(hana::sort_by(
        boolean_metafunction_class<Pred>, hana::to<hana::Tuple>(Sequence{})
    ));
};

template <typename Sequence>
struct reverse {
    using type = decltype(hana::reverse(hana::to<hana::Tuple>(Sequence{})));
};


//////////////////////////////////////////////////////////////////////////////
// Runtime algorithms
//////////////////////////////////////////////////////////////////////////////
template <typename Sequence, typename F>
void for_each(F f) {
    hana::for_each(Sequence{}, [&f](auto t) {
        f(typename decltype(t)::type{});
    });
}

template <typename Sequence, typename TransformOp, typename F>
void for_each(F f) {
    for_each<typename transform<Sequence, TransformOp>::type>(f);
}

} // end namespace hpl


template <typename N>
struct is_odd
    : hpl::bool_<(N::value % 2)>
{ };


int main() {
using namespace hpl;

//////////////////////////////////////////////////////////////////////////////
// Misc
//////////////////////////////////////////////////////////////////////////////

// pair
{
    static_assert(std::is_same<first<pair<int, float>>::type, int>{}, "");
    static_assert(std::is_same<second<pair<int, float>>::type, float>{}, "");
}

//////////////////////////////////////////////////////////////////////////////
// Intrinsics
//////////////////////////////////////////////////////////////////////////////

// push_front
{
    using v = vector_c<int,1,2,3,5,8,13,21>;
    static_assert(size<v>{} == 7, "");

    using fibonacci = push_front<v, int_<1>>::type;
    static_assert(size<fibonacci>{} == 8, "");

    static_assert(equal<
        fibonacci,
        vector_c<int,1,1,2,3,5,8,13,21>,
        equal_to<mpl::_, mpl::_>
    >{}, "");
}

// size
{
    using empty_list = vector<>;
    using numbers = vector_c<int,0,1,2,3,4,5>;
    using more_numbers = range_c<int,0,100>;

    static_assert(size<empty_list>{} == 0, "");
    static_assert(size<numbers>{} == 6, "");
    static_assert(size<more_numbers>{} == 100, "");
}


//////////////////////////////////////////////////////////////////////////////
// Iteration algorithms
//////////////////////////////////////////////////////////////////////////////

// fold
{
    using types = vector<long,float,short,double,float,long,long double>;
    using number_of_floats = fold<types, int_<0>,
        mpl::if_<std::is_floating_point<mpl::_2>,
            next<mpl::_1>,
            mpl::_1
        >
    >::type;
    static_assert(number_of_floats{} == 4, "");
}

// reverse_fold
{
    using numbers = vector_c<int,5,-1,0,-7,-2,0,-5,4>;
    using negatives = vector_c<int,-1,-7,-2,-5>;
    using result = reverse_fold<numbers, vector_c<int>,
        mpl::if_<less<mpl::_2, int_<0>>,
            push_front<mpl::_1, mpl::_2>,
            mpl::_1
        >
    >::type;
    static_assert(equal<negatives, result>{}, "");
}

//////////////////////////////////////////////////////////////////////////////
// Query algorithms
//////////////////////////////////////////////////////////////////////////////

// find_if
{
    using types = vector<char,int,unsigned,long,unsigned long>;
    using found = find_if<types, std::is_same<mpl::_1, unsigned>>::type;
    static_assert(std::is_same<found, unsigned>{}, "");
}

// find
{
    using types = vector<char,int,unsigned,long,unsigned long>;
    static_assert(std::is_same<find<types, unsigned>::type, unsigned>{}, "");
}

// contains
{
    using types = vector<char,int,unsigned,long,unsigned long>;
    static_assert(!contains<types, bool>{}, "");
}

// count
{
    using types = vector<int,char,long,short,char,short,double,long>;
    static_assert(count<types, short>{} == 2, "");
}

// count_if
{
    using types = vector<int,char,long,short,char,long,double,long>;
    static_assert(count_if<types, std::is_floating_point<mpl::_>>{} == 1, "");
    static_assert(count_if<types, std::is_same<mpl::_, char>>{} == 2, "");
    static_assert(count_if<types, std::is_same<mpl::_, void>>{} == 0, "");
}

// min_element (MPL's example is completely broken)
{
}

// max_element (MPL's example is completely broken)
{
}

// equal
{
    using s1 = vector<char,int,unsigned,long,unsigned long>;
    using s2 = vector<char,int,unsigned,long>;
    static_assert(!equal<s1,s2>{}, "");
}


//////////////////////////////////////////////////////////////////////////////
// Transformaton algorithms
//////////////////////////////////////////////////////////////////////////////
// copy
{
    using numbers = vector_c<int,10, 11, 12, 13, 14, 15, 16, 17, 18, 19>;
    using result = copy<range_c<int, 10, 20>>::type;
    static_assert(size<result>{} == 10, "");
    static_assert(equal<result, numbers, mpl::quote2<equal_to>>{}, "");
}

// copy_if
{
    using result = copy_if<range_c<int, 0, 10>, less<mpl::_1, int_<5>>>::type;
    static_assert(size<result>{} == 5, "");
    static_assert(equal<result, range_c<int, 0, 5>>{}, "");
}

// transform
{
    using types = vector<char,short,int,long,float,double>;
    using pointers = vector<char*,short*,int*,long*,float*,double*>;
    using result = transform<types,std::add_pointer<mpl::_1>>::type;
    static_assert(equal<result, pointers>{}, "");
}

// replace
{
    using types = vector<int,float,char,float,float,double>;
    using expected = vector<int,double,char,double,double,double>;
    using result = replace< types,float,double >::type;
    static_assert(equal<result, expected>{}, "");
}

// replace_if
{
    using numbers = vector_c<int,1,4,5,2,7,5,3,5>;
    using expected = vector_c<int,1,4,0,2,0,0,3,0>;
    using result = replace_if<numbers, greater<mpl::_, int_<4>>, int_<0>>::type;
    static_assert(equal<result, expected, mpl::quote2<equal_to>>{}, "");
}

// remove
{
    using types = vector<int,float,char,float,float,double>;
    using result = remove<types, float>::type;
    static_assert(equal<result, vector<int, char, double>>{}, "");
}

// remove_if
{
    using numbers = vector_c<int,1,4,5,2,7,5,3,5>;
    using result = remove_if<numbers, greater<mpl::_, int_<4> > >::type;
    static_assert(equal<result, vector_c<int,1,4,2,3>, mpl::quote2<equal_to>>{}, "");
}

// unique
{
    using types = vector<int,float,float,char,int,int,int,double>;
    using expected = vector<int,float,char,int,double>;
    using result = unique<types, std::is_same<mpl::_1, mpl::_2>>::type;
    static_assert(equal<result, expected>{}, "");
}

// partition
{
    using r = partition<range_c<int,0,10>, is_odd<mpl::_1>>::type;
    static_assert(equal<first<r>::type, vector_c<int,1,3,5,7,9>>{}, "");
    static_assert(equal<second<r>::type, vector_c<int,0,2,4,6,8>>{}, "");
}

// sort
{
    using numbers = vector_c<int,3,4,0,-5,8,-1,7>;
    using expected = vector_c<int,-5,-1,0,3,4,7,8>;
    using result = sort<numbers>::type;
    static_assert(equal<result, expected, equal_to<mpl::_, mpl::_>>{}, "");
}

// reverse
{
    using numbers = vector_c<int,9,8,7,6,5,4,3,2,1,0>;
    using result = reverse<numbers>::type;
    static_assert(equal<result, range_c<int,0,10>>{}, "");
}

//////////////////////////////////////////////////////////////////////////////
// Runtime algorithms
//////////////////////////////////////////////////////////////////////////////

// for_each
{
    auto value_printer = [](auto x) {
        std::cout << x << '\n';
    };

    for_each<range_c<int, 0, 10> >(value_printer);
}

}
