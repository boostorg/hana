
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

template <typename F> struct return_of;
template <typename R, typename ...A> struct return_of<R(A...)> { using type = R; };

template <typename Concept, typename = void>
struct via_operators : return_of<Concept>::type { };

template <typename Concept, typename = void>
struct via_subclass : via_operators<Concept> { };

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

#if 0
template <typename Concept, typename = void>
struct has_cross_type { static constexpr bool value = false; };

template <typename Concept, typename T, typename U>
struct has_cross_type<Concept(T, U), void_t<common_t<T, U>>> {
    // assert is not same T, U
    using C = common_t<T, U>;
    static constexpr bool value = is<Concept, T> && is<Concept, U> && is<Concept, C>;
};

template <typename T, typename U, typename F>
struct cross_type_dispatch
    : if_<is_same<T, U>,
        typename F::template normal<T, U>,
        if_<has_cross_type<T, U>,
            typename F::template via_common<T, U>,
            the method is not implemented
        >
    >
{ };

struct equal_impl_cross_type {
    template <typename T, typename U>
    struct normal : stuff<Comparable(T)>::template equal_impl<T, U> { };

    template <typename T, typename U>
    struct via_common {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y) {
            return equal(to<C>(detail::std::forward<X>(x)),
                         to<C>(detail::std::forward<Y>(y)));
        }
    };

    template <typename Stuff, typename X, typename Y>
    using otherwise = typename Stuff::template equal_impl<X, Y>;
};

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
template <typename T, typename U>
struct equal_impl
    : cross_type_dispatch<T, U, equal_impl_cross_type>
{ };
#endif

template <typename _>
struct via_operators<Comparable(_)> : Comparable {
    template <typename T, typename U, typename = void>
    struct equal_impl : Comparable::equal_impl<T, U> { };
    template <typename T, typename U>
    struct equal_impl<T, U, decltype((void)(std::declval<T>() == std::declval<U>()))> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return std::forward<X>(x) == std::forward<Y>(y); }
    };

    template <typename T, typename U, typename = void>
    struct not_equal_impl : Comparable::not_equal_impl<T, U> { };
    template <typename T, typename U>
    struct not_equal_impl<T, U, decltype((void)(std::declval<T>() != std::declval<U>()))> {
        template <typename X, typename Y>
        static constexpr decltype(auto) apply(X&& x, Y&& y)
        { return std::forward<X>(x) != std::forward<Y>(y); }
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
    std::string name;

    friend void operator==(Employee, Employee) {
        std::cerr << "operator==(Employee, Employee)\n";
    }
};





// Priorities:
// 1. Explicit/partial specialization
// 2. Implementation provided by subclass
// 3. Implementation provided by operators
// 4. Implementation via common type if it applies (todo)
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
}
