# Boost.Hana
> An experimental C++1y metaprogramming library

This library is an attempt to merge Boost.Fusion and the Boost.MPL into
a single metaprogramming library.

## Disclaimers
This is not an official Boost library and there is no guarantee whatsoever
that it will be proposed.

The library is unstable at the moment; do not use for production.


## Todo
- Do we want `char_<1> + char<2> == char_<3>` or `char_<1> + char_<2> == int_<3>`?
- How to avoid problems with ADL? `type<invalid> == type<invalid>` will instantiate
  `invalid` because of ADL for `operator==`.
- Does it make sense to have something like an `argwise(f)` wrapper for functions
  which would provide some typeclasses like container-like Functor. In particular,
  `fmap(g, argwise(f))(xs...)` would be equivalent to `f(g(xs)...)`. This is also
  equivalent to `argmap` in the MPL11. Perhaps other MPL11 operations like `bind`
  and `on` have a category theoretical meaning?