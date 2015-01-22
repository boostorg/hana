
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
#include <boost/hana/functional/placeholder.hpp>
using boost::hana::_;


//////////////////////////////////////////////////////////////////////////////
// base
//////////////////////////////////////////////////////////////////////////////
struct not_implemented { };

template <typename Concept, typename Next = not_implemented, typename = void>
struct default_ : Next { };

template <typename Method, typename Concept, typename Next = not_implemented>
struct provided : Next { };

template <typename ...>
using void_t = void;

template <typename ...>
struct valid : std::true_type { };

template <typename To>
auto to = [](auto x) { return static_cast<To>(x); };



//////////////////////////////////////////////////////////////////////////////
// operators.hpp
//////////////////////////////////////////////////////////////////////////////
template <typename Method, typename = void>
struct has_operator : std::false_type { };

template <typename Method, typename Op, typename Next = not_implemented, typename = void>
struct operator_ : Next { };

template <template <typename ...> class Method, typename T, typename U, typename Op, typename Next>
struct operator_<Method<T, U>, Op, Next, std::enable_if_t<
    !has_operator<Method<T, U>>::value,
    decltype((void)(Op{}(std::declval<T>(), std::declval<U>())))
>> {
    template <typename X, typename Y>
    static constexpr decltype(auto) apply(X&& x, Y&& y)
    { return Op{}(std::forward<X>(x), std::forward<Y>(y)); }
};

//////////////////////////////////////////////////////////////////////////////
// cross_type.hpp
//////////////////////////////////////////////////////////////////////////////
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

template <typename Concept, typename Method, typename Next = not_implemented,
    bool = has_cross_type<Concept>::value>
struct cross_type : Next { };

template <typename Concept, typename T, typename U, typename Method, typename Next>
struct cross_type<Concept(T, U), Method, Next, true> {
    template <typename X, typename Y>
    static constexpr decltype(auto) apply(X&& x, Y&& y) {
        using Common = std::common_type_t<X, Y>;
        std::cerr << "cross-type dispatching: ";
        return Method{}(to<Common>(std::forward<X>(x)),
                        to<Common>(std::forward<Y>(y)));
    }
};


/////////////////////////////////////////
// Comparable
/////////////////////////////////////////
struct Comparable { };

template <typename T, typename U> struct equal_impl;
constexpr struct {
    template <typename X, typename Y>
    constexpr decltype(auto) operator()(X x, Y y) const
    { return equal_impl<decltype(x), decltype(y)>::apply(x, y); }
} equal{};
template <typename T, typename U>
struct equal_impl
    : provided<equal_impl<T, U>, Comparable,
        operator_<equal_impl<T, U>, decltype(_==_),
            cross_type<Comparable(T, U), decltype(equal),
                default_<equal_impl<T, U>>
            >
        >
    >
{ };

template <typename T, typename U> struct not_equal_impl;
constexpr struct {
    template <typename X, typename Y>
    constexpr decltype(auto) operator()(X x, Y y) const
    { return not_equal_impl<decltype(x), decltype(y)>::apply(x, y); }
} not_equal{};
template <typename T, typename U>
struct not_equal_impl
    : provided<not_equal_impl<T, U>, Comparable,
        operator_<not_equal_impl<T, U>, decltype(_!=_),
            cross_type<Comparable(T, U), decltype(not_equal),
                default_<not_equal_impl<T, U>>
            >
        >
    >
{ };

template <typename T, typename U, typename Next>
struct default_<equal_impl<T, U>, Next> {
    template <typename X, typename Y>
    static void apply(X x, Y y)
    { std::cerr << "default equal_impl\n"; }
};

template <typename T, typename U, typename Next>
struct default_<not_equal_impl<T, U>, Next> {
    template <typename X, typename Y>
    static void apply(X x, Y y)
    { std::cerr << "default not_equal_impl\n"; }
};


/////////////////////////////////////////
// Enumerable
/////////////////////////////////////////
struct Enumerable { };

template <typename E> struct succ_impl;
constexpr struct {
    template <typename X>
    constexpr decltype(auto) operator()(X x) const
    { return succ_impl<X>::apply(x); }
} succ{};
template <typename E>
struct succ_impl
    : provided<succ_impl<E>, Enumerable,
        default_<succ_impl<E>>
    >
{ };

/////////////////////////////////////////
// Constant
/////////////////////////////////////////
struct Constant { };

template <typename C> struct value_impl;
constexpr struct {
    template <typename C>
    constexpr decltype(auto) operator()(C x) const
    { return value_impl<C>::apply(x); }
} value{};
template <typename C>
struct value_impl
    : provided<value_impl<C>, Constant,
        default_<value_impl<C>>
    >
{ };

/////////////////////////////////////////
// Monoid
/////////////////////////////////////////
struct Monoid { };

template <typename T, typename U> struct plus_impl;
constexpr struct {
    template <typename X, typename Y>
    constexpr decltype(auto) operator()(X x, Y y) const
    { return plus_impl<decltype(x), decltype(y)>::apply(x, y); }
} plus{};
template <typename T, typename U>
struct plus_impl
    : provided<plus_impl<T, U>, Monoid,
        operator_<plus_impl<T, U>, decltype(_+_),
            cross_type<Monoid(T, U), decltype(plus),
                default_<plus_impl<T, U>>
            >
        >
    >
{ };

template <typename M> struct zero_impl;
template <typename M>
struct _zero {
    constexpr decltype(auto) operator()() const
    { return zero_impl<M>::apply(); }
};
template <typename M>
constexpr _zero<M> zero{};
template <typename M>
struct zero_impl
    : provided<zero_impl<M>, Monoid,
        default_<zero_impl<M>>
    >
{ };

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
{ };

template <typename Method, typename Concept>
struct for_free;

// Comparable
template <typename T, typename U>
struct for_free<equal_impl<T, U>, IntegralConstant> {
    template <typename X, typename Y>
    static void apply(X x, Y y) {
        std::cerr << "equal_impl via IntegralConstant\n";
    }
};

// Enumerable
template <typename C>
struct for_free<succ_impl<C>, IntegralConstant> {
    template <typename X>
    static void apply(X x) {
        std::cerr << "succ_impl via IntegralConstant\n";
    }
};

// Monoid
template <typename T, typename U>
struct for_free<plus_impl<T, U>, IntegralConstant> {
    template <typename X, typename Y>
    static void apply(X x, Y y) {
        std::cerr << "plus_impl via IntegralConstant\n";
    }
};

template <typename C>
struct for_free<zero_impl<C>, IntegralConstant> {
    static void apply() {
        std::cerr << "zero_impl via IntegralConstant\n";
    }
};


template <typename C> struct integral_constant_impl;
template <typename C>
struct _integral_constant {
    template <typename T, T v>
    constexpr decltype(auto) operator()() const
    { return integral_constant_impl<C>::template apply<T, v>(); }
};
template <typename C>
constexpr _integral_constant<C> integral_constant{};
template <typename C>
struct integral_constant_impl
    : provided<integral_constant_impl<C>, IntegralConstant,
        default_<integral_constant_impl<C>>
    >
{ };

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
    template <typename T, T v>
    static void apply() {
        std::cerr << "integral_constant_impl<MPLIntegralC>\n";
    }
};

// Comparable
template <typename Next>
struct provided<equal_impl<MPLIntegralC, MPLIntegralC>, Comparable, Next>
    : for_free<equal_impl<MPLIntegralC, MPLIntegralC>, IntegralConstant>
{ };

// Enumerable
template <typename Next>
struct provided<succ_impl<MPLIntegralC>, Enumerable, Next>
    : for_free<succ_impl<MPLIntegralC>, IntegralConstant>
{ };

// Monoid
template <typename Next>
struct provided<plus_impl<MPLIntegralC, MPLIntegralC>, Monoid, Next>
    : for_free<plus_impl<MPLIntegralC, MPLIntegralC>, IntegralConstant>
{ };

template <typename Next>
struct provided<zero_impl<MPLIntegralC>, Monoid, Next>
    : for_free<zero_impl<MPLIntegralC>, IntegralConstant>
{ };


// etc...

/////////////////////////////////////////
// Employee
/////////////////////////////////////////
struct Employee {
    template <typename E, typename Y, typename = std::enable_if_t<std::is_same<E, Employee>::value>>
    friend void operator==(E, Y) {
        std::cerr << "operator==(" << typeid(E).name() << ", " << typeid(Y).name() << ")\n";
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
struct has_operator<equal_impl<MyType, MyType>> : std::true_type { };
template <>
struct has_operator<not_equal_impl<MyType, MyType>> : std::true_type { };

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
// 2. Implementation provided by subclass (this one makes no sense for binary methods with T != U)
// 3. Implementation provided by operators
// 4. Implementation via common type if it applies
// 5. Default implementation

// When you create a dispatching plugin:
// 1. Todo


// When you create a type class:
// 0. Each method should by default call `via_subclass<Typeclass(...)>::method_impl<...>`
// 1. Inherit from superclasses
// 2. Place the default method implementations in the body of the type class
// 3. Place any provided superclass methods in the body of the type class
// 4. Place any operator-based method implementation in the `via_operators<Typeclass(...)>`
//      - Do not forget to inherit via_common in your via_operators
//      - ...
// 5. Place any method using common types to dispatch in the `via_common<Typeclass(...)>`
//      - Do not forget to inherit from `Typeclass` in your via_common
//      - ...


// NOTE: In a lazy context, Foldable and Searchable collapse!!

int main() {
    succ_impl<MPLIntegralC>::apply(0);

    equal_impl<MPLIntegralC, MPLIntegralC>::apply(0, 0);
    not_equal_impl<MPLIntegralC, MPLIntegralC>::apply(0, 0);

    value_impl<MPLIntegralC>::apply(0);
    integral_constant_impl<MPLIntegralC>::apply<int, 0>();

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



    (void)value; (void)succ;
}
