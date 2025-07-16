#include "laws_basic.hpp"

int main() {
    // Comparable and Hashable
    {
        testComparable<char16_t>();
    }

    // Monoid
    {
        testMonoid<char16_t>();
    }

    // Foldable and Iterable
    {
        testFoldable<char16_t>();
    }

    // Orderable
    {
        testOrderable<char16_t>();
    }

    // Searchable
    {
        testSearchable<char16_t>();
    }
}
