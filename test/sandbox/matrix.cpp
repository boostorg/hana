/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/core/datatype.hpp>
#include <boost/hana/detail/assert.hpp>
#include <boost/hana/functional.hpp>
#include <boost/hana/functor/fmap_mcd.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/type.hpp>
using namespace boost::hana;


struct Matrix;

template <typename Storage, typename = operators::enable>
struct matrix_type {
    using hana_datatype = Matrix;

    Storage rows_;
    constexpr auto ncolumns() const
    { return length(head(rows_)); }

    constexpr auto nrows() const
    { return length(rows_); }

    constexpr auto size() const
    { return nrows() * ncolumns(); }

    template <typename I, typename J>
    constexpr auto at(I i, J j) const
    { return boost::hana::at(j, boost::hana::at(i, rows_)); }
};

auto transpose = [](auto m) {
    auto new_storage = unpack(m.rows_, zip);
    return matrix_type<decltype(new_storage)>{new_storage};
};

auto rows = [](auto m) {
    return m.rows_;
};

auto columns = [](auto m) {
    return rows(transpose(m));
};

auto elementwise = [](auto f) {
    return [=](auto ...matrices) {
        auto new_storage = zip_with(partial(zip_with, f), matrices.rows_...);
        return matrix_type<decltype(new_storage)>{new_storage};
    };
};

template <typename S1, typename S2>
constexpr auto operator+(matrix_type<S1> m1, matrix_type<S2> m2)
{ return elementwise(_+_)(m1, m2); }

template <typename S1, typename S2>
constexpr auto operator-(matrix_type<S1> m1, matrix_type<S2> m2)
{ return elementwise(_-_)(m1, m2); }


auto scalar_prod = [](auto v1, auto v2) {
    return sum(zip_with(_*_, v1, v2));
};
auto repeat_n = [](auto n, auto x) {
    return unpack(range(int_<0>, n), on(list, always(x)));
};

template <typename S1, typename S2>
constexpr auto operator*(matrix_type<S1> m1, matrix_type<S2> m2) {
    auto storage = fmap(
        [=](auto row) {
            return zip_with(
                    scalar_prod,
                    repeat_n(m2.ncolumns(), row),
                    columns(m2));
        }
    , rows(m1));
    return matrix_type<decltype(storage)>{storage};
}

auto row = [](auto ...entries) {
    return list(entries...);
};

auto matrix = [](auto ...rows) {
    auto storage = list(rows...);
    auto all_same_length = all(tail(storage), [=](auto row) {
        return length(row) == length(head(storage));
    });
    static_assert(all_same_length, "");

    return matrix_type<decltype(storage)>{storage};
};

auto vector = on(matrix, row);


constexpr int exponent(int x, unsigned int n) {
    if (n == 0) return 1;
    else        return x * exponent(x, n - 1);
}

auto remove_at = [](auto n, auto xs) {
    auto with_indices = zip(xs, range(int_<0>, length(xs)));
    auto removed = filter(with_indices, compose(n != _, last));
    return fmap(head, removed);
};

template <typename Matrix>
struct _det; // remove circular dependency between matrix_minor and det

auto matrix_minor = [](auto m, auto i, auto j) {
    auto submatrix_storage = fmap(partial(remove_at, j), remove_at(i, rows(m)));
    matrix_type<decltype(submatrix_storage)> submatrix{submatrix_storage};
    return _det<decltype(submatrix)>{}(submatrix);
};

auto cofactor = [](auto m, auto i, auto j) {
    auto i_plus_j = i + j;
    return int_<exponent(-1, i_plus_j())> * matrix_minor(m, i, j);
};

template <typename Matrix>
struct _det {
    constexpr auto operator()(Matrix m) const {
        return eval_if(m.size() == int_<1>,
            always(m.at(int_<0>, int_<0>)),
            [=](auto _) {
                auto cofactors_1st_row = unpack(_(range)(int_<0>, m.ncolumns()),
                    on(list, partial(cofactor, m, int_<0>))
                );
                return scalar_prod(head(rows(m)), cofactors_1st_row);
            }
        );
    }
};

auto det = [](auto m) {
    return _det<decltype(m)>{}(m);
};

namespace boost { namespace hana {
    template <>
    struct Functor::instance<Matrix> : Functor::fmap_mcd {
        template <typename F, typename M>
        static constexpr auto fmap_impl(F f, M mat) {
            auto new_storage = fmap(partial(fmap, f), mat.rows_);
            return matrix_type<decltype(new_storage)>{new_storage};
        }
    };

    template <>
    struct Comparable::instance<Matrix, Matrix> : Comparable::equal_mcd {
        template <typename M1, typename M2>
        static constexpr auto equal_impl(M1 m1, M2 m2) {
            return m1.nrows() == m2.nrows() &&
                   m1.ncolumns() == m2.ncolumns() &&
                   all_of(zip_with(_==_, m1.rows_, m2.rows_));
        }
    };
}}


void test_sizes() {
    auto m = matrix(
        row(1, '2', 3),
        row('4', char_<'5'>, 6)
    );
    BOOST_HANA_CONSTEXPR_ASSERT(m.size() == 6);
    BOOST_HANA_CONSTEXPR_ASSERT(m.ncolumns() == 3);
    BOOST_HANA_CONSTEXPR_ASSERT(m.nrows() == 2);
}

void test_at() {
    auto m = matrix(
        row(1, '2', 3),
        row('4', char_<'5'>, 6),
        row(int_<7>, '8', 9.3)
    );
    BOOST_HANA_CONSTEXPR_ASSERT(m.at(int_<0>, int_<0>) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(m.at(int_<0>, int_<1>) == '2');
    BOOST_HANA_CONSTEXPR_ASSERT(m.at(int_<0>, int_<2>) == 3);

    BOOST_HANA_CONSTEXPR_ASSERT(m.at(int_<1>, int_<0>) == '4');
    BOOST_HANA_CONSTANT_ASSERT(m.at(int_<1>, int_<1>) == char_<'5'>);
    BOOST_HANA_CONSTEXPR_ASSERT(m.at(int_<1>, int_<2>) == 6);

    BOOST_HANA_CONSTANT_ASSERT(m.at(int_<2>, int_<0>) == int_<7>);
    BOOST_HANA_CONSTEXPR_ASSERT(m.at(int_<2>, int_<1>) == '8');
    BOOST_HANA_CONSTEXPR_ASSERT(m.at(int_<2>, int_<2>) == 9.3);
}

void test_comparable() {
    BOOST_HANA_CONSTEXPR_ASSERT(matrix(row(1, 2)) == matrix(row(1, 2)));
    BOOST_HANA_CONSTEXPR_ASSERT(matrix(row(1, 2)) != matrix(row(1, 5)));

    BOOST_HANA_CONSTEXPR_ASSERT(matrix(row(1, 2), row(3, 4)) == matrix(row(1, 2), row(3, 4)));
    BOOST_HANA_CONSTEXPR_ASSERT(matrix(row(1, 2), row(3, 4)) != matrix(row(1, 2), row(0, 4)));
    BOOST_HANA_CONSTEXPR_ASSERT(matrix(row(1, 2), row(3, 4)) != matrix(row(0, 2), row(3, 4)));

    BOOST_HANA_CONSTANT_ASSERT(matrix(row(1), row(2)) != matrix(row(3, 4), row(5, 6)));
    BOOST_HANA_CONSTANT_ASSERT(matrix(row(1), row(2)) != matrix(row(3, 4)));
}

void test_functor() {
    auto m = matrix(
        row(1, int_<2>, 3),
        row(int_<4>, 5, 6),
        row(7, 8, int_<9>)
    );
    BOOST_HANA_CONSTEXPR_ASSERT(fmap(_ + int_<1>, m) ==
        matrix(
            row(2, int_<3>, 4),
            row(int_<5>, 6, 7),
            row(8, 9, int_<10>)
        )
    );
}

void test_operators() {
    auto m = matrix(row(1, 2), row(3, 4));
    BOOST_HANA_CONSTEXPR_ASSERT(m + m == matrix(row(2, 4), row(6, 8)));
    BOOST_HANA_CONSTEXPR_ASSERT(m - m == matrix(row(0, 0), row(0, 0)));
}

void test_matrix_multiplication() {
    auto A = matrix(
        row(1, 2, 3),
        row(4, 5, 6)
    );

    auto B = matrix(
        row(1, 2),
        row(3, 4),
        row(5, 6)
    );

    BOOST_HANA_CONSTEXPR_ASSERT(A * B == matrix(
        row(1*1 + 2*3 + 5*3, 1*2 + 2*4 + 3*6),
        row(4*1 + 3*5 + 5*6, 4*2 + 5*4 + 6*6)
    ));
}

void test_vector() {
    auto v = vector(1, '2', int_<3>, 4.2f);
    BOOST_HANA_CONSTEXPR_ASSERT(v.size() == 4);
    BOOST_HANA_CONSTEXPR_ASSERT(v.nrows() == 4);
    BOOST_HANA_CONSTEXPR_ASSERT(v.ncolumns() == 1);
}

void test_transpose() {
    auto m = matrix(
        row(1, 2.2, '3'),
        row(4, '5', 6)
    );
    BOOST_HANA_CONSTEXPR_ASSERT(transpose(m) ==
        matrix(
            row(1, 4),
            row(2.2, '5'),
            row('3', 6)
        )
    );
}

void test_repeat_n() {
    struct T;
    BOOST_HANA_CONSTANT_ASSERT(repeat_n(int_<0>, type<T>) == list());
    BOOST_HANA_CONSTANT_ASSERT(repeat_n(int_<1>, type<T>) == list(type<T>));
    BOOST_HANA_CONSTANT_ASSERT(repeat_n(int_<2>, type<T>) == list(type<T>, type<T>));
    BOOST_HANA_CONSTANT_ASSERT(repeat_n(int_<3>, type<T>) == list(type<T>, type<T>, type<T>));
    BOOST_HANA_CONSTANT_ASSERT(repeat_n(int_<4>, type<T>) == list(type<T>, type<T>, type<T>, type<T>));
}

void test_determinant() {
    BOOST_HANA_CONSTEXPR_ASSERT(det(matrix(row(1))) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(det(matrix(row(2))) == 2);

    BOOST_HANA_CONSTEXPR_ASSERT(det(matrix(row(1, 2), row(3, 4))) == -2);

    BOOST_HANA_CONSTEXPR_ASSERT(
        det(matrix(
            row(1, 5, 6),
            row(3, 2, 4),
            row(7, 8, 9)
        ))
        == 51
    );

    BOOST_HANA_CONSTEXPR_ASSERT(
        det(matrix(
            row(1, 5, 6, -3),
            row(3, 2, 4, -5),
            row(7, 8, 9, -1),
            row(8, 2, 1, 10)
        )) == 214
    );

    BOOST_HANA_CONSTEXPR_ASSERT(
        det(matrix(
            row(1,  5,  6, -3, 92),
            row(3,  2,  4, -5, 13),
            row(7,  8,  9, -1, 0),
            row(8,  2,  1, 10, 41),
            row(3, 12, 92, -7, -4)
        )) == -3115014
    );
}

void test_remove_at() {
    BOOST_HANA_CONSTANT_ASSERT(remove_at(int_<0>, list(1)) == list());
    BOOST_HANA_CONSTEXPR_ASSERT(remove_at(int_<0>, list(1, '2')) == list('2'));
    BOOST_HANA_CONSTEXPR_ASSERT(remove_at(int_<0>, list(1, '2', 3.3)) == list('2', 3.3));

    BOOST_HANA_CONSTEXPR_ASSERT(remove_at(int_<1>, list(1, '2')) == list(1));
    BOOST_HANA_CONSTEXPR_ASSERT(remove_at(int_<1>, list(1, '2', 3.3)) == list(1, 3.3));

    BOOST_HANA_CONSTEXPR_ASSERT(remove_at(int_<2>, list(1, '2', 3.3)) == list(1, '2'));
}

void test_exponent() {
    BOOST_HANA_CONSTEXPR_ASSERT(exponent(3, 0) == 1);
    BOOST_HANA_CONSTEXPR_ASSERT(exponent(3, 1) == 3);
    BOOST_HANA_CONSTEXPR_ASSERT(exponent(3, 2) == 3 * 3);
    BOOST_HANA_CONSTEXPR_ASSERT(exponent(3, 3) == 3 * 3 * 3);
    BOOST_HANA_CONSTEXPR_ASSERT(exponent(3, 4) == 3 * 3 * 3 * 3);
}

int main() {
    test_repeat_n();
    test_remove_at();
    test_exponent();

    test_sizes();
    test_at();
    test_comparable();
    test_functor();
    test_operators();
    test_vector();
    test_transpose();
    test_matrix_multiplication();
    test_determinant();
}
