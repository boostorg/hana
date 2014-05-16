# Boost.Hana
> An experimental C++1y metaprogramming library

## Disclaimers
This is not an official Boost library and there is no guarantee whatsoever
that it will be proposed.

The library is unstable at the moment; do not use for production.


## Todo
- Do we want `char_<1> + char<2> == char_<3>` or `char_<1> + char_<2> == int_<3>`?
- How to avoid problems with ADL? `type<invalid> == type<invalid>` will instantiate
  `invalid` because of ADL for `operator==`.
