
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

template <typename Concept, typename T>
struct stuff : Concept { };

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
struct equal_impl : stuff<Comparable, T>::template equal_impl<T, U> {
    // static_assert(T is the same as U);
};

template <typename T, typename U>
struct not_equal_impl : stuff<Comparable, T>::template not_equal_impl<T, U> {
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
struct succ_impl : stuff<Enumerable, E>::template succ_impl<E> {

};

template <typename E>
struct pred_impl : stuff<Enumerable, E>::template pred_impl<E> {

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
struct value_impl : stuff<Constant, C>::template value_impl<C> {

};

#if 1
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
struct plus_impl : stuff<Monoid, T>::template plus_impl<T, U> {
    // static_assert(T is the same as U);
};

template <typename M>
struct zero_impl : stuff<Monoid, M>::template zero_impl<M> {

};
#endif

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
struct integral_constant_impl : stuff<IntegralConstant, C>::template integral_constant_impl<C> {

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

// Comparable
#if 1
template <>
struct equal_impl<MPLIntegralC, MPLIntegralC>
    : IntegralConstant::equal_impl<MPLIntegralC, MPLIntegralC>
{ };
#else
template <>
struct stuff<Comparable, MPLIntegralC>
    : IntegralConstant
{ };
#endif

// Enumerable
template <>
struct stuff<Enumerable, MPLIntegralC>
    : IntegralConstant
{ };

// Monoid
template <>
struct stuff<Monoid, MPLIntegralC>
    : IntegralConstant
{ };

// etc...

#if 0
/////////////////////////////////////////
// Employee
/////////////////////////////////////////
struct Employee {
    std::string name;

    friend bool operator==(Employee, Employee) {
        // ...
    }
};


// We must be able to call `equal(Employee, Employee)`.
#endif




// Priorities:
// 1. Explicit/partial specialization
// 2. Implementation provided by subclass
// 3. Implementation provided by operators
// 4. Default implementation


int main() {
    succ_impl<MPLIntegralC>::apply(0);
    pred_impl<MPLIntegralC>::apply(0);

    equal_impl<MPLIntegralC, MPLIntegralC>::apply(0, 0);
    not_equal_impl<MPLIntegralC, MPLIntegralC>::apply(0, 0);

    value_impl<MPLIntegralC>::apply(0);
    integral_constant_impl<MPLIntegralC>::apply(0);

    zero_impl<MPLIntegralC>::apply();
    plus_impl<MPLIntegralC, MPLIntegralC>::apply(0, 0);
}
