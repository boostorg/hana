/*!
@file
Defines `unit test`.

@copyright Louis Dionne 2013-2017
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */
template <typename C>
int basic_strcmp(C const* l, C const* r) {
  for (;*l != static_cast<C>('\0'); l++, r++) {
    if (*l != *r) {
      return *l < *r ? -1 : 1;
    }
  }
  return *r == static_cast<C>('\0') ? 0 : -1;
}
