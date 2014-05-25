# Boost.Hana
> An experimental C++1y metaprogramming library

This library is an attempt to merge Boost.Fusion and the Boost.MPL into
a single metaprogramming library.

## Disclaimers
This is not an official Boost library and there is no guarantee whatsoever
that it will be proposed.

The library is unstable at the moment; do not use for production.


## Self notes

### Non-constexpr sequences
Sequences whose constructor(s) are not constexpr can't always know their
bounds at compile-time. What we would like is for `Iterable` and such
typeclasses to provide defaults that also work for these runtime sequences.
The difference between runtime sequences and compile-time sequences is that
the `is_empty` method can only return a runtime `bool` for the former, while
it returns a `Bool<...>` for the latter.

#### Attempt 1
This works, but it requires a lot of repetition:

```cpp
template <typename Index, typename Iterable_>
static constexpr auto at_helper(Bool<true>, Index n, Iterable_ iterable)
{ return head(iterable); }

template <typename Index, typename Iterable_>
static constexpr auto at_helper(Bool<false>, Index n, Iterable_ iterable)
{ return at_helper(n == size_t<1>, n - size_t<1>, tail(iterable)); }

template <typename Index, typename Iterable_>
static constexpr auto at_helper(bool cond, Index n, Iterable_ iterable) {
    if (cond) return head(iterable);
    else      return at_helper(n == size_t<1>, n - size_t<1>, tail(iterable));
}

template <typename Index, typename Iterable_>
static constexpr auto at_impl(Index n, Iterable_ iterable)
{ return at_helper(n == size_t<0>, n, iterable); }
```


#### Attempt 2
The obvious improvement is to use an external `if_` that factors the
repetition away. Unfortunately, it does not work.

```cpp
constexpr struct _lazy_if {
    template <typename Then, typename Else, typename ...Args>
    constexpr auto operator()(Bool<true>, Then t, Else, Args ...args) const
    { return t(args...); }

    template <typename Then, typename Else, typename ...Args>
    constexpr auto operator()(Bool<false>, Then, Else e, Args ...args) const
    { return e(args...); }

    template <typename Then, typename Else, typename ...Args>
    constexpr auto operator()(bool cond, Then t, Else e, Args ...args) const {
        if (cond) return t(args...);
        else      return e(args...);
    }
} lazy_if{};


template <typename Index, typename Iterable_>
static constexpr auto at_impl(Index n, Iterable_ iterable) {
    return lazy_if(n == size_t<0>,
        [](auto n, auto it) { return head(it); },
        [](auto n, auto it) { return at_impl(n - size_t<1>, tail(it)); },
        n, iterable
    );
}
```

Note that we pass `Args...` to the `lazy_if` because otherwise we would need
to return two different lambdas from the same function in the `bool` case.

I think the reason for it not working is because we introduce a new
intermediate lambda

```cpp
[](auto n, auto it) { return at_impl(n - size_t<1>, tail(it)); },
```

in the _then_ branch, which causes the compiler to hit the instantiation
limit while trying to recursively instantiate `at_impl` when it instantiates
the lambda.


#### Attempt 2.5
Using a different `if_`, but it still does not work.

```cpp
constexpr struct _tri_if {
    template <typename Then, typename Else, typename Maybe, typename ...Args>
    constexpr auto operator()(Bool<true> cond, Then t, Else, Maybe, Args ...args) const
    { return t(cond, args...); }

    template <typename Then, typename Else, typename Maybe, typename ...Args>
    constexpr auto operator()(Bool<false> cond, Then, Else e, Maybe, Args ...args) const
    { return e(cond, args...); }

    template <typename Then, typename Else, typename Maybe, typename ...Args>
    constexpr auto operator()(bool cond, Then, Else, Maybe m, Args ...args) const
    { return m(cond, args...); }
} tri_if{};

template <typename Index, typename Iterable_>
static constexpr auto at_impl(Index n, Iterable_ iterable) {
    return tri_if(n == size_t<0>,
        [](auto cond, auto n, auto it) { return head(it); },
        [](auto cond, auto n, auto it) { return at_impl(n - size_t<1>, tail(it)); },
        [](bool cond, Index n, Iterable_ it) {
            return cond ? head(it) : at_impl(n - size_t<1>, tail(it));
        },
        n, iterable
    );
}
```


#### Attempt 3
Here, I'm trying to use structs to make everything more explicit. Still does
not work because the problem is when instantiating the inner `apply` function.
See the comment below.

```cpp
template <typename Cond, typename = void>
struct at_helper;

template <typename Dummy>
struct at_helper<Bool<true>, Dummy> {
    template <typename Index, typename Iterable_>
    static constexpr auto apply(Bool<true>, Index n, Iterable_ iterable) {
        return head(iterable);
    }
};

template <typename Dummy>
struct at_helper<Bool<false>, Dummy> {
    template <typename Index, typename Iterable_>
    static constexpr auto apply(Bool<false>, Index n, Iterable_ iterable) {
        return at_helper<decltype(n == size_t<1>)>::apply(
            n == size_t<1>, n - size_t<1>, tail(iterable)
        );
    }
};

template <typename Dummy>
struct at_helper<bool, Dummy> {
    template <typename Index, typename Iterable_>
    static constexpr auto apply(bool cond, Index n, Iterable_ iterable) {
        if (cond)
            return head(iterable);
        else
            // This triggers infinite recursive instantiations.
            // return at_helper<Bool<false>>::apply(false_, n, iterable);

            // This works. decltype(...) is actually bool.
            return at_helper<decltype(n == size_t<1>)>::apply(
                n == size_t<1>, n - size_t<1>, tail(iterable)
            );
    }
};

template <typename Index, typename Iterable_>
static constexpr auto at_impl(Index n, Iterable_ iterable) {
    return at_helper<decltype(n == size_t<0>)>::apply(n == size_t<0>, n, iterable);
}
```

#### Attempt 4
It works, but it's not a solution. I basically use the preprocessor to
generate some of the duplicate code.

```cpp
#define HACK(f)                                                             \
template <TPARAMS>                                                          \
static constexpr auto f(Bool<true>, PARAMS)                                 \
{ return TRUE_BRANCH; }                                                     \
                                                                            \
template <TPARAMS>                                                          \
static constexpr auto f(Bool<false>, PARAMS)                                \
{ return FALSE_BRANCH; }                                                    \
                                                                            \
template <TPARAMS>                                                          \
static constexpr auto f(bool cond, PARAMS) {                                \
    if (cond) return TRUE_BRANCH;                                           \
    else      return FALSE_BRANCH;                                          \
}                                                                           \
/**/

#define TPARAMS typename Index, typename Iterable_
#define PARAMS Index n, Iterable_ iterable
#define TRUE_BRANCH head(iterable)
#define FALSE_BRANCH at_helper(n == size_t<1>, n - size_t<1>, tail(iterable))

HACK(at_helper);

template <typename Index, typename Iterable_>
static constexpr auto at_impl(Index n, Iterable_ iterable)
{ return at_helper(n == size_t<0>, n, iterable); }
```

#### Attempt 5
Trying to factor out the branches into lambdas. This recursively instantiates
stuff too.

```cpp
BOOST_HANA_CONSTEXPR_LAMBDA auto true_branch = [](auto at_impl, auto n, auto iterable)
{ return head(iterable); };

BOOST_HANA_CONSTEXPR_LAMBDA auto false_branch = [](auto at_impl, auto n, auto iterable)
{ return at_impl(n - size_t<1>, tail(iterable)); };

template <typename F, typename Index, typename Iterable_>
static constexpr auto at_helper(F at_impl, Bool<true>, Index n, Iterable_ iterable)
{ return true_branch(at_impl, n, iterable); }

template <typename F, typename Index, typename Iterable_>
static constexpr auto at_helper(F at_impl, Bool<false>, Index n, Iterable_ iterable)
{ return false_branch(at_impl, n, iterable); }

template <typename F, typename Index, typename Iterable_>
static constexpr auto at_helper(F at_impl, bool cond, Index n, Iterable_ iterable) {
    if (cond) return true_branch(at_impl, n, iterable);
    else      return false_branch(at_impl, n, iterable);
}

BOOST_HANA_CONSTEXPR_LAMBDA auto at_impl = fix(
    [](auto at_impl, auto n, auto iterable)
    { return at_helper(at_impl, n == size_t<0>, n, iterable); }
);
```

#### Attempt 6
Make everything explicit by using structs.

```cpp
template <typename Condition, typename Index, typename Iterable_>
struct at_helper;

template <typename AtHelper>
struct false_branch {
    template <typename Index, typename It>
    static constexpr auto apply(Index n, It it) {
        return AtHelper::apply(n == size_t<1>, n - size_t<1>, tail(it));
    }
};

template <typename Index, typename Iterable_>
struct at_helper<Bool<true>, Index, Iterable_> {
    static constexpr auto apply(Bool<true>, Index n, Iterable_ it)
    { return head(it); }
};

template <typename Index, typename Iterable_>
struct at_helper<Bool<false>, Index, Iterable_> {
    static constexpr auto apply(Bool<false>, Index n, Iterable_ it) {
        // This works
        return false_branch<at_helper<decltype(n == size_t<1>), decltype(n - size_t<1>), decltype(tail(it))>>::apply(n, it);

        // This works too.
        // return at_helper<decltype(n == size_t<1>), decltype(n - size_t<1>), decltype(tail(it))>::
        //        apply(n == size_t<1>, n - size_t<1>, tail(it));
    }
};

template <typename Index, typename Iterable_>
struct at_helper<bool, Index, Iterable_> {
    static constexpr auto apply(bool cond, Index n, Iterable_ it) {
        if (cond) return head(it);
                  // Surprisingly, using the false_branch<...> just like above
                  // does not work here. WTF!
        else      return at_helper<decltype(n == size_t<1>), decltype(n - size_t<1>), decltype(tail(it))>::
                         apply(n == size_t<1>, n - size_t<1>, tail(it));
    }
};

template <typename Index, typename Iterable_>
static constexpr auto at_impl(Index n, Iterable_ iterable) {
    return at_helper<decltype(n == size_t<0>), decltype(n), decltype(iterable)>::
           apply(n == size_t<0>, n, iterable);
}
```



## Todo
- Do we want `char_<1> + char<2> == char_<3>` or `char_<1> + char_<2> == int_<3>`?
- How to avoid problems with ADL? `type<invalid> == type<invalid>` will instantiate
  `invalid` because of ADL for `operator==`.
- Does it make sense to have something like an `argwise(f)` wrapper for functions
  which would provide some typeclasses like container-like Functor. In particular,
  `fmap(g, argwise(f))(xs...)` would be equivalent to `f(g(xs)...)`. This is also
  equivalent to `argmap` in the MPL11. Perhaps other MPL11 operations like `bind`
  and `on` have a category theoretical meaning?