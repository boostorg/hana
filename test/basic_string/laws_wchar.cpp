#include "laws_basic.hpp"

int main() {
    // Comparable and Hashable
    {
        testComparable<wchar_t>();
    }

    // Monoid
    {
        testMonoid<wchar_t>();
    }

    // Foldable and Iterable
    {
        testFoldable<wchar_t>();
    }

    // Orderable
    {
        testOrderable<wchar_t>();
    }

    // Searchable
    {
        testSearchable<wchar_t>();
    }
}
