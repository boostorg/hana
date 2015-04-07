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
    return hana::transform(hana::group(sequence, predicate), hana::head);
}

//////////////////////////////////////////////////////////////////////////////
// integral_c
//////////////////////////////////////////////////////////////////////////////
template <typename T, T v>
using integral_c = std::integral_constant<T, v>;

template <int i>
using int_ = integral_c<int, i>;

template <long i>
using long_ = integral_c<long, i>;

template <bool b>
using bool_ = integral_c<bool, b>;

using true_ = bool_<true>;
using false_ = bool_<false>;


//////////////////////////////////////////////////////////////////////////////
// Sequences, compile-time integers & al
//
// Differences with the MPL:
// 1. No more annoying numbered variants
// 2. `pair<...>::first` and `pair<...>::second` won't work;
//    use `first<pair<...>>` instead
//////////////////////////////////////////////////////////////////////////////
template <typename ...T>
struct vector : decltype(hana::tuple_t<T...>) {
    using hana = vector;
    using datatype = boost::hana::Tuple;
};

template <typename ...T>
struct set : decltype(hana::set(hana::type<T>...)) {
    using hana = set;
    using datatype = boost::hana::Set;
};

template <typename T, T ...v>
struct vector_c : decltype(hana::tuple_c<T, v...>) {
    using hana = vector_c;
    using datatype = boost::hana::Tuple;
};

template <typename T, T from, T to>
struct range_c : decltype(hana::range_c<T, from, to>) {
    using hana = range_c;
    using datatype = boost::hana::Range;
};


template <typename T, typename U>
struct pair : decltype(hana::make_pair(hana::type<T>, hana::type<U>)) { };

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
//
// Differences with the MPL:
// 1. `at` does not work for associative sequences; use `find` instead.
// 2. `begin`, `end`, `clear`, `erase`, `erase_key`, `insert`, `insert_range`,
//    `is_sequence`, `key_type`, `order`, `sequence_tag`, `value_type`: not implemented
//////////////////////////////////////////////////////////////////////////////
template <typename Sequence, typename N>
struct at
    : decltype(hana::at(N{}, Sequence{}))
{ };

template <typename Sequence, long n>
using at_c = at<Sequence, long_<n>>;

template <typename Sequence>
struct back
    : decltype(+hana::last(Sequence{}))
{ };

template <typename Sequence>
struct empty
    : decltype(hana::is_empty(Sequence{}))
{ };

template <typename Sequence>
struct front
    : decltype(+hana::head(Sequence{}))
{ };

template <typename Sequence>
struct pop_back {
    using type = decltype(hana::init(hana::to<hana::Tuple>(Sequence{})));
};

template <typename Sequence>
struct pop_front {
    using type = decltype(hana::tail(Sequence{}));
};

template <typename Sequence, typename T>
struct push_back {
    using type = decltype(hana::append(Sequence{}, hana::type<T>));
};

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
    : decltype(hana::fold.left(
        Sequence{}, hana::type<State>, hana::metafunction_class<
            typename mpl::lambda<F>::type
        >
    ))
{ };

template <typename Sequence, typename State, typename F>
struct reverse_fold
    : decltype(hana::fold.right(
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
//    Instead of returning an iterator, they either have a nested `::type`
//    alias to the answer, or they have no nested `::type` at all, which
//    makes them SFINAE-friendly.
//
// 2. lower_bound, upper_bound:
//    Not implemented.
//
// 3. {min,max}_element:
//    Not returning an iterator, and also won't work on empty sequences.
//////////////////////////////////////////////////////////////////////////////
template <typename Sequence, typename Pred>
struct find_if
    : decltype(hana::find_if(
        Sequence{}, boolean_metafunction_class<Pred>
    ))
{ };

template <typename Sequence, typename T>
struct find
    : decltype(hana::find(Sequence{}, hana::type<T>))
{ };

template <typename Sequence, typename T>
struct contains
    : decltype(hana::elem(Sequence{}, hana::type<T>))
{ };

template <typename Sequence, typename T>
struct count
    : decltype(hana::count(Sequence{}, hana::type<T>))
{ };

template <typename Sequence, typename Pred>
struct count_if
    : decltype(hana::count_if(Sequence{}, boolean_metafunction_class<Pred>))
{ };

template <typename Sequence, typename Pred = mpl::quote2<less>>
struct min_element
    : decltype(hana::minimum(Sequence{}, boolean_metafunction_class<Pred>))
{ };

template <typename Sequence, typename Pred = mpl::quote2<less>>
struct max_element
    : decltype(hana::maximum(Sequence{}, boolean_metafunction_class<Pred>))
{ };

template <typename S1, typename S2, typename Pred = mpl::quote2<std::is_same>>
struct equal
    : decltype( // inefficient but whatever
        hana::length(S1{}) == hana::length(S2{}) &&
        hana::all(hana::zip.with(boolean_metafunction_class<Pred>,
                hana::to<hana::Tuple>(S1{}),
                hana::to<hana::Tuple>(S2{})))
    )
{ };

//////////////////////////////////////////////////////////////////////////////
// Transformaton algorithms
//
// Differences from the MPL:
// 1. The algorithms do not accept an optional inserter, and they always
//    return a `vector`.
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
        hana::type<OldType>,
        hana::type<NewType>
    ));
};

template <typename Sequence, typename Pred, typename NewType>
struct replace_if {
    using type = decltype(hana::replace_if(
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
    using type = decltype(hana::sort(
        hana::to<hana::Tuple>(Sequence{}), boolean_metafunction_class<Pred>
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

// at
{
    using range = range_c<long,10,50>;
    static_assert(at<range, int_<0>>::value == 10, "");
    static_assert(at<range, int_<10>>::value == 20, "");
    static_assert(at<range, int_<40>>::value == 50, "");
}

// at_c
{
    using range = range_c<long, 10, 50>;
    static_assert(at_c<range, 0>::value == 10, "");
    static_assert(at_c<range, 10>::value == 20, "");
    static_assert(at_c<range, 40>::value == 50, "");
}

// back
{
    using range1 = range_c<int,0,1>;
    using range2 = range_c<int,0,10>;
    using range3 = range_c<int,-10,0>;
    using types = vector<int, char, float>;
    static_assert(back<range1>::value == 0, "");
    static_assert(back<range2>::value == 9, "");
    static_assert(back<range3>::value == -1, "");
    static_assert(std::is_same<back<types>::type, float>{}, "");
}

// empty
{
    using empty_range = range_c<int,0,0>;
    using types = vector<long,float,double>;
    static_assert(empty<empty_range>{}, "");
    static_assert(!empty<types>{}, "");
}

// front
{
    using types1 = vector<long>;
    using types2 = vector<int,long>;
    using types3 = vector<char,int,long>;
    static_assert(std::is_same<front<types1>::type, long>{}, "");
    static_assert(std::is_same<front<types2>::type, int>{}, "");
    static_assert(std::is_same<front<types3>::type, char>{}, "");
}

// pop_back
{
    using types1 = vector<long>;
    using types2 = vector<long,int>;
    using types3 = vector<long,int,char>;


    using result1 = pop_back<types1>::type;
    using result2 = pop_back<types2>::type;
    using result3 = pop_back<types3>::type;

    static_assert(size<result1>::value == 0, "");
    static_assert(size<result2>::value == 1, "");
    static_assert(size<result3>::value == 2, "");

    static_assert(std::is_same< back<result2>::type, long>{}, "");
    static_assert(std::is_same< back<result3>::type, int>{}, "");
}

// pop_front
{
    using types1 = vector<long>;
    using types2 = vector<int,long>;
    using types3 = vector<char,int,long>;

    using result1 = pop_front<types1>::type;
    using result2 = pop_front<types2>::type;
    using result3 = pop_front<types3>::type;

    static_assert(size<result1>::value == 0, "");
    static_assert(size<result2>::value == 1, "");
    static_assert(size<result3>::value == 2, "");

    static_assert(std::is_same<front<result2>::type, long>{}, "");
    static_assert(std::is_same<front<result3>::type, int>{}, "");
}

// push_back
{
    using bools = vector_c<bool,false,false,false,true,true,true,false,false>;
    using message = push_back<bools, false_>::type;
    static_assert(back<message>::type::value == false, "");
    static_assert(count_if<message, equal_to<mpl::_1, false_>>{} == 6u, "");
}

// push_front
{
    using v = vector_c<int,1,2,3,5,8,13,21>;
    static_assert(size<v>{} == 7u, "");

    using fibonacci = push_front<v, int_<1>>::type;
    static_assert(size<fibonacci>{} == 8u, "");

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

    static_assert(size<empty_list>{} == 0u, "");
    static_assert(size<numbers>{} == 6u, "");
    static_assert(size<more_numbers>{} == 100u, "");
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
    static_assert(count<types, short>{} == 2u, "");
}

// count_if
{
    using types = vector<int,char,long,short,char,long,double,long>;
    static_assert(count_if<types, std::is_floating_point<mpl::_>>{} == 1u, "");
    static_assert(count_if<types, std::is_same<mpl::_, char>>{} == 2u, "");
    static_assert(count_if<types, std::is_same<mpl::_, void>>{} == 0u, "");
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
    static_assert(size<result>{} == 10u, "");
    static_assert(equal<result, numbers, mpl::quote2<equal_to>>{}, "");
}

// copy_if
{
    using result = copy_if<range_c<int, 0, 10>, less<mpl::_1, int_<5>>>::type;
    static_assert(size<result>{} == 5u, "");
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
