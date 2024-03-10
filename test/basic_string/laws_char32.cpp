#include "laws_basic.hpp"

int main() {
    // Comparable and Hashable
    {
        testComparable<char32_t>();
    }

    // Monoid
    {
        testMonoid<char32_t>();
    }

    // Foldable and Iterable
    {
        testFoldable<char32_t>();
    }

    // Orderable
    {
        testOrderable<char32_t>();
    }

    // Searchable
    {
        testSearchable<char32_t>();
    }
}
