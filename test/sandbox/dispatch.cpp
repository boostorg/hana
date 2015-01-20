
// 1. There must not exist cycles in the superclass graph.
// 2. You may only use methods that are part of the current MCD and in
//    superclasses to implement functionality. Obviously you can also
//    use stuff that's not in the library at all.
// 3.

template <typename ...>
struct wrong { static constexpr bool value = false; };


#if 0
/////////////////////////////////////////
// Orderable
//
// Superclasses:
// - Comparable
/////////////////////////////////////////
template <typename T, typename U>
struct less_impl {
    static_assert(wrong<less_impl<T, U>>::value,
    "no definition of boost::hana::less for the given data types");
};

template <typename T, typename U>
struct greater_impl {
    template <typename X, typename Y>
    static constexpr auto apply(X x, Y y) {
        return less(y, x);
    }
};

// etc...

struct Orderable {
    template <typename T, typename U>
    struct equal_impl {
        template <typename X, typename Y>
        static constexpr auto apply(X x, Y y) {
            return and_(not_(less(x, y)), not_(less(y, x)));
        }
    };
};


/////////////////////////////////////////
// Functor
/////////////////////////////////////////
template <typename T>
struct fmap_impl {
    template <typename X, typename F>
    static constexpr auto apply(X x, F f) {
        return adjust(x, always(true_), f);
    }
};

template <typename T>
struct adjust_impl {
    template <typename X, typename Pred, typename F>
    static constexpr auto apply(X x, Pred pred, F f) {
        return fmap(x, demux(if_)(pred, f, id));
    }
};


/////////////////////////////////////////
// Applicative
//
// Superclasses:
// - Functor
/////////////////////////////////////////
template <typename A>
struct lift_impl {
    static_assert(wrong<lift_impl<A>>::value,
    "no definition of boost::hana::lift for the given data type");
};

template <typename A>
struct ap_impl {
    static_assert(wrong<ap_impl<A>>::value,
    "no definition of boost::hana::ap for the given data type");
};

struct Applicative {
    template <typename A>
    struct fmap_impl {
        template <typename X, typename F>
        static constexpr auto apply(X x, F f) {
            return ap(lift<A>(f), x);
        }
    };
};


/////////////////////////////////////////
// Monad
//
// Superclasses:
// - Functor, Applicative
/////////////////////////////////////////
template <typename M>
struct flatten_impl {
    static_assert(wrong<flatten_impl<M>>::value,
    "no definition of boost::hana::flatten for the given data type");
};

/////////////////////////////////////////
// List
//
// Superclasses:
// - Comparable, Orderable
// - Functor, Applicative, Monad,
// - Foldable, Iterable
//
// MCD:
// - cons + nil + Foldable ?
/////////////////////////////////////////
template <typename L>
struct cons_impl {
    static_assert(wrong<cons_impl<L>>::value,
    "no definition of boost::hana::cons for the given data type");
};

template <typename L>
struct nil_impl {
    static_assert(wrong<nil_impl<L>>::value,
    "no definition of boost::hana::nil for the given data type");
};

struct List {
    // Functor: Foldable, nil, cons
    template <typename L>
    struct fmap_impl {
        template <typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs xs, F f) {
            return foldr(xs, nil<L>(), compose(cons, f));
        }
    };

    // Applicative: cons, nil, Monad, Functor
    template <typename L>
    struct lift_impl {
        template <typename X>
        static constexpr auto apply(X x)
        { return cons(x, nil<L>()); }
    };

    template <typename L>
    struct ap_impl {
        template <typename Fs, typename Xs>
        static constexpr auto apply(Fs fs, Xs xs) {
            return bind(fs, partial(fmap, xs));
        }
    };

    // Monad: Foldable, nil, concat
    template <typename L>
    struct flatten_impl {
        template <typename Xss>
        static constexpr auto apply(Xss xss) {
            return foldl(xss, nil<L>(), concat);
        }
    };

    // Traversable: Foldable, Functor, Applicative, nil, cons
    template <typename L>
    struct traverse_impl {
        template <typename A, typename Xs, typename F>
        static constexpr decltype(auto) apply(Xs xs, F f) {
            return foldr(xs, lift<A>(nil<L>()), [=](auto x, auto ys) {
                return ap(fmap(f(x), curry<2>(cons)), ys);
            });
        }
    };

    // Comparable: Foldable + zip_with
    template <typename T, typename U>
    struct equal_impl {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs xs, Ys ys) {
            return length(xs) == length(ys) && all_of(zip_with(equal, xs, ys));
        }
    };

    // Orderable:
    template <typename T, typename U>
    struct less_impl {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs xs, Ys ys) {
            return lexicographical order;
        }
    };
};


/////////////////////////////////////////
// Iterable
//
// Superclasses:
// - Foldable
/////////////////////////////////////////
template <typename It>
struct head_impl {
    static_assert(wrong<head_impl<It>>::value,
    "no definition of boost::hana::head for the given data type");
};

template <typename It>
struct tail_impl {
    static_assert(wrong<tail_impl<It>>::value,
    "no definition of boost::hana::tail for the given data type");
};

template <typename It>
struct is_empty_impl {
    static_assert(wrong<is_empty_impl<It>>::value,
    "no definition of boost::hana::is_empty for the given data type");
};

struct Iterable {
    // Foldable
    template <typename It>
    struct foldl_impl {
        template <typename Xs, typename S, typename F>
        static constexpr auto apply(Xs xs, S s, F f) {
            return ...;
        }
    };

    // Searchable
    template <typename It>
    struct find_impl {

    };

    template <typename It>
    struct any_impl {

    };
};
#endif

#include <iostream>
#include <type_traits>


template <typename F>
struct meta_not : std::integral_constant<bool, !F::value> { };

template <bool ...b>
struct any_of_c
    : meta_not<std::is_same<
        any_of_c<b...>,
        any_of_c<(b, false)...>
    >>
{ };

template <typename T, typename ...U>
struct is_one_of
    : any_of_c<std::is_same<T, U>::value...>
{ };

template <typename ...>
using void_t = void;

template <typename ...>
struct valid : std::true_type { };


template <typename F> struct return_of;
template <typename R, typename ...A> struct return_of<R(A...)> { using type = R; };

// Cross-type dispatching algorithm
// --------------------------------
// if (T == U) {
//     stuff<Comparable(T)>::template equal_impl<T, U>
// }
// else if (T is Comparable && U is Comparable &&
//          T and U have a common type C &&
//          C is Comparable)
// {
//     convert to the common type and try again there
// }
// else {
//     the method is not implemented
// }
template <typename Concept, typename = void>
struct via_common : return_of<Concept>::type { };

template <typename Concept, typename = void>
struct has_cross_type { static constexpr bool value = false; };

template <typename Concept, typename T, typename U>
struct has_cross_type<Concept(T, U), std::enable_if_t<
    !std::is_same<T, U>::value &&
    valid<std::common_type_t<T, U>>{}
>> {
    using C = std::common_type_t<T, U>;
    static constexpr bool value = true;
        // is<Concept, T> && is<Concept, U> && is<Concept, C>;
};


template <typename T, typename Concept, typename = void>
struct has_operators : std::false_type { };

template <typename Concept, typename = void>
struct via_operators : via_common<Concept> { };

template <typename Concept, typename = void>
struct via_subclass : via_operators<Concept> { };

/////////////////////////////////////////
// Comparable
/////////////////////////////////////////
struct Comparable {
    template <typename T, typename U>
    struct equal_impl {
        template <typename X, typename Y>
        static void apply(X x, Y y) {
            std::cerr << "default equal_impl\n";
        }
    };

    template <typename T, typename U>
    struct not_equal_impl {
        template <typename X, typename Y>
        static void apply(X x, Y y) {
            std::cerr << "default not_equal_impl\n";
        }
    };
};

template <typename T, typename U>
struct equal_impl : via_subclass<Comparable(T)>::template equal_impl<T, U> {
    // static_assert(T is the same as U);
};

template <typename T, typename U>
struct not_equal_impl : via_subclass<Comparable(T)>::template not_equal_impl<T, U> {
    // static_assert(T is the same as U);
};


template <typename _>
struct via_common<Comparable(_)> : Comparable {
    template <typename T, typename U, bool = has_cross_type<Comparable(T, U)>::value>
    struct equal_impl : Comparable::equal_impl<T, U> { };

    template <typename T, typename U>
    struct equal_impl<T, U, true> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            std::cerr << "cross-type dispatching of equal_impl: ";
            using C = std::common_type_t<T, U>;
            return ::equal_impl<C, C>::apply((C)x, (C)y);
        }
    };


    template <typename T, typename U, bool = has_cross_type<Comparable(T, U)>::value>
    struct not_equal_impl : Comparable::not_equal_impl<T, U> { };

    template <typename T, typename U>
    struct not_equal_impl<T, U, true> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            std::cerr << "cross-type dispatching of not_equal_impl: ";
            using C = std::common_type_t<T, U>;
            return ::not_equal_impl<C, C>::apply((C)x, (C)y);
        }
    };
};

template <typename _>
struct via_operators<Comparable(_)> : via_common<Comparable(_)> {
    template <typename T, typename U, typename = void>
    struct equal_impl
        : via_common<Comparable(_)>::template equal_impl<T, U>
    { };

    template <typename T, typename U>
    struct equal_impl<T, U, std::enable_if_t<!has_operators<T, Comparable>::value,
        decltype((void)(std::declval<T>() == std::declval<U>()))>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            std::cerr << "equal_impl via operator==: ";
            (void)(x == y);
        }
    };


    template <typename T, typename U, typename = void>
    struct not_equal_impl
        : via_common<Comparable(_)>::template not_equal_impl<T, U>
    { };

    template <typename T, typename U>
    struct not_equal_impl<T, U, std::enable_if_t<!has_operators<T, Comparable>::value,
        decltype((void)(std::declval<T>() != std::declval<U>()))>> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            std::cerr << "not_equal_impl via operator!=: ";
            (void)(x != y);
        }
    };
};


/////////////////////////////////////////
// Enumerable
/////////////////////////////////////////
struct Enumerable {
    template <typename E>
    struct succ_impl {
        static_assert(wrong<succ_impl<E>>::value,
        "no definition of boost::hana::succ for the given data type");
    };

    template <typename E>
    struct pred_impl {
        static_assert(wrong<pred_impl<E>>::value,
        "no definition of boost::hana::pred for the given data type");
    };
};

template <typename E>
struct succ_impl : via_subclass<Enumerable(E)>::template succ_impl<E> {

};

template <typename E>
struct pred_impl : via_subclass<Enumerable(E)>::template pred_impl<E> {

};

/////////////////////////////////////////
// Constant
/////////////////////////////////////////
struct Constant {
    template <typename C>
    struct value_impl {
        static_assert(wrong<value_impl<C>>::value,
        "no definition of boost::hana::value for the given data type");
    };
};

template <typename C>
struct value_impl : via_subclass<Constant(C)>::template value_impl<C> {

};

/////////////////////////////////////////
// Monoid
/////////////////////////////////////////
struct Monoid {
    template <typename T, typename U>
    struct plus_impl {
        static_assert(wrong<plus_impl<T, U>>::value,
        "no definition of boost::hana::plus for the given data type");
    };

    template <typename M>
    struct zero_impl {
        static_assert(wrong<zero_impl<M>>::value,
        "no definition of boost::hana::zero for the given data type");
    };
};

template <typename T, typename U>
struct plus_impl : via_subclass<Monoid(T)>::template plus_impl<T, U> {
    // static_assert(T is the same as U);
};

template <typename M>
struct zero_impl : via_subclass<Monoid(M)>::template zero_impl<M> {

};

/////////////////////////////////////////
// IntegralConstant
//
// Superclasses:
// - Constant
// - Enumerable
// - Comparable, Orderable
// - Monoid, Group, Ring, IntegralDomain
/////////////////////////////////////////
struct IntegralConstant
    : Constant
    , Enumerable
    , Comparable
    , Monoid
{
    template <typename C>
    struct integral_constant_impl {
        static_assert(wrong<integral_constant_impl<C>>::value,
        "no definition of boost::hana::integral_constant for the given data type");
    };

    // Enumerable
    template <typename C>
    struct succ_impl {
        template <typename X>
        static void apply(X x) {
            std::cerr << "succ_impl via IntegralConstant\n";
        }
    };

    template <typename C>
    struct pred_impl {
        template <typename X>
        static void apply(X x) {
            std::cerr << "pred_impl via IntegralConstant\n";
        }
    };

    // Comparable
    template <typename T, typename U>
    struct equal_impl {
        template <typename X, typename Y>
        static void apply(X x, Y y) {
            std::cerr << "equal_impl via IntegralConstant\n";
        }
    };

    // Monoid
    template <typename T, typename U>
    struct plus_impl {
        template <typename X, typename Y>
        static void apply(X x, Y y) {
            std::cerr << "plus_impl via IntegralConstant\n";
        }
    };

    template <typename C>
    struct zero_impl {
        static void apply() {
            std::cerr << "zero_impl via IntegralConstant\n";
        }
    };
};

template <typename C>
struct integral_constant_impl : via_subclass<IntegralConstant(C)>::template integral_constant_impl<C> {

};


/////////////////////////////////////////
// MPLIntegralC
/////////////////////////////////////////
struct MPLIntegralC;

// Constant
template <>
struct value_impl<MPLIntegralC> {
    template <typename X>
    static void apply(X x) {
        std::cerr << "value_impl<MPLIntegralC>\n";
    }
};

// IntegralConstant
template <>
struct integral_constant_impl<MPLIntegralC> {
    template <typename X>
    static void apply(X x) {
        std::cerr << "integral_constant_impl<MPLIntegralC>\n";
    }
};

// Comparable, Enumerable, Monoid
template <typename Concept>
struct via_subclass<Concept(MPLIntegralC), std::enable_if_t<
    std::is_base_of<Concept, IntegralConstant>::value
>>
    : IntegralConstant
{ };

// etc...

/////////////////////////////////////////
// Employee
/////////////////////////////////////////
struct Employee {
    template <typename E, typename = std::enable_if_t<std::is_same<E, Employee>::value>>
    friend void operator==(E, E) {
        std::cerr << "operator==(Employee, Employee)\n";
    }
};


/////////////////////////////////////////
// Manager
/////////////////////////////////////////
struct Manager : Employee { };


/////////////////////////////////////////
// MyType
//
// Usually, Hana types have some operators that forward back to the methods.
// However, the methods are at least partly implemented. Normally, we would
// have a problem because the via_operators would call != before it can get
// to the default implementation (the desired path), which would cause
// infinite recursion because != for MyType calls not_equal. The workaround
// is to provide a has_operators trait that is specialized when the != is
// an alias to not_equal, and we only check the operators in `via_operators`
// if that is not the case.
/////////////////////////////////////////
struct MyType;

template <>
struct equal_impl<MyType, MyType> {
    template <typename X, typename Y>
    static auto apply(X x, Y y) {
        std::cerr << "equal_impl<MyType, MyType>\n";
    }
};

template <>
struct has_operators<MyType, Comparable> : std::true_type { };

struct MyType {
    friend void operator==(MyType a, MyType b) {
        std::cerr << "operator==(MyType, MyType): ";
        equal_impl<MyType, MyType>::apply(a, b);
    }
    friend void operator!=(MyType a, MyType b) {
        std::cerr << "operator!=(MyType, MyType): ";
        not_equal_impl<MyType, MyType>::apply(a, b);
    }
};





// Priorities:
// 1. Explicit/partial specialization
// 2. Implementation provided by subclass
// 3. Implementation provided by operators
// 4. Implementation via common type if it applies
// 5. Default implementation


int main() {
    succ_impl<MPLIntegralC>::apply(0);
    pred_impl<MPLIntegralC>::apply(0);

    equal_impl<MPLIntegralC, MPLIntegralC>::apply(0, 0);
    not_equal_impl<MPLIntegralC, MPLIntegralC>::apply(0, 0);

    value_impl<MPLIntegralC>::apply(0);
    integral_constant_impl<MPLIntegralC>::apply(0);

    zero_impl<MPLIntegralC>::apply();
    plus_impl<MPLIntegralC, MPLIntegralC>::apply(0, 0);

    equal_impl<Employee, Employee>::apply(Employee{}, Employee{});
    not_equal_impl<Employee, Employee>::apply(Employee{}, Employee{});

    equal_impl<Employee, Manager>::apply(Employee{}, Manager{});
    not_equal_impl<Employee, Manager>::apply(Employee{}, Manager{});

    equal_impl<MyType, MyType>::apply(MyType{}, MyType{});
    not_equal_impl<MyType, MyType>::apply(MyType{}, MyType{});
    (void)(MyType{} == MyType{});
    (void)(MyType{} != MyType{});
}
