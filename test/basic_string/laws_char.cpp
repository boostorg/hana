#include "laws_basic.hpp"

int main() {
    // Comparable and Hashable
    {
        testComparable<char>();
    }

    // Monoid
    {
        testMonoid<char>();
    }

    // Foldable and Iterable
    {
        testFoldable<char>();
    }

    // Orderable
    {
        testOrderable<char>();
    }

    // Searchable
    {
        testSearchable<char>();
    }
}
