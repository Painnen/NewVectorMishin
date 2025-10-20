#include <gtest.h>
#include "SparceMatrix.h"
#include"TriangleMatrix.h"
#include"BandMatrix.h"


TEST(TriangleMatrixTest, AdditionSameSize) {
    TriangleMatrix<int> mat1(3);
    TriangleMatrix<int> mat2(3);

    mat1.Set(1, 0, 0);
    mat1.Set(2, 1, 0);
    mat1.Set(3, 1, 1);
    mat1.Set(4, 2, 0);
    mat1.Set(5, 2, 1);
    mat1.Set(6, 2, 2);

    mat2.Set(2, 0, 0);
    mat2.Set(3, 1, 0);
    mat2.Set(4, 1, 1);
    mat2.Set(5, 2, 0);
    mat2.Set(6, 2, 1);
    mat2.Set(7, 2, 2);

    TriangleMatrix<int> result = mat1 + mat2;

    EXPECT_EQ(result.Get(0, 0), 3);   // 1 + 2
    EXPECT_EQ(result.Get(1, 0), 5);   // 2 + 3
    EXPECT_EQ(result.Get(1, 1), 7);   // 3 + 4
    EXPECT_EQ(result.Get(2, 0), 9);   // 4 + 5
    EXPECT_EQ(result.Get(2, 1), 11);  // 5 + 6
    EXPECT_EQ(result.Get(2, 2), 13);  // 6 + 7
}

TEST(TriangleMatrixTest, AdditionDifferentSizesThrows)
{
    TriangleMatrix<int> mat1(3);
    TriangleMatrix<int> mat2(4);

    EXPECT_THROW(mat1 + mat2, std::invalid_argument);
}


TEST(TriangleMatrixTest, SubtractionSameSize)
{
    TriangleMatrix<int> mat1(2);
    TriangleMatrix<int> mat2(2);

    mat1.Set(5, 0, 0);
    mat1.Set(3, 1, 0);
    mat1.Set(7, 1, 1);

    mat2.Set(2, 0, 0);
    mat2.Set(1, 1, 0);
    mat2.Set(4, 1, 1);

    TriangleMatrix<int> result = mat1 - mat2;

    EXPECT_EQ(result.Get(0, 0), 3);   // 5 - 2
    EXPECT_EQ(result.Get(1, 0), 2);   // 3 - 1
    EXPECT_EQ(result.Get(1, 1), 3);   // 7 - 4
}


TEST(TriangleMatrixTest, ScalarMultiplication)
{
    TriangleMatrix<int> mat(2);

    mat.Set(2, 0, 0);
    mat.Set(3, 1, 0);
    mat.Set(4, 1, 1);

    int scalar = 3;
    TriangleMatrix<int> result = mat * scalar;

    EXPECT_EQ(result.Get(0, 0), 6);
    EXPECT_EQ(result.Get(1, 0), 9);
    EXPECT_EQ(result.Get(1, 1), 12);
}

TEST(TriangleMatrixTest, ScalarMultiplicationZero)
{
    TriangleMatrix<int> mat(2);

    mat.Set(1, 0, 0);
    mat.Set(2, 1, 1);

    int scalar = 0;
    TriangleMatrix<int> result = mat * scalar;

    EXPECT_EQ(result.Get(0, 0), 0);
    EXPECT_EQ(result.Get(1, 1), 0);
}


TEST(TriangleMatrixTest, ScalarDivision)
{
    TriangleMatrix<double> mat(2);

    mat.Set(6.0, 0, 0);
    mat.Set(9.0, 1, 0);
    mat.Set(12.0, 1, 1);

    double scalar = 3.0;
    TriangleMatrix<double> result = mat / scalar;

    EXPECT_DOUBLE_EQ(result.Get(0, 0), 2.0);   // 6.0 / 3.0
    EXPECT_DOUBLE_EQ(result.Get(1, 0), 3.0);   // 9.0 / 3.0
    EXPECT_DOUBLE_EQ(result.Get(1, 1), 4.0);   // 12.0 / 3.0
}

TEST(TriangleMatrixTest, ScalarDivisionByZeroThrows)
{
    TriangleMatrix<int> mat(2);
    int scalar = 0;

    EXPECT_THROW(mat / scalar, std::invalid_argument);
}



TEST(TriangleMatrixTest, SelfAssignment)
{
    TriangleMatrix<int> mat(2);
    mat.Set(1, 0, 0);
    mat.Set(2, 1, 1);

    mat = mat;

    EXPECT_EQ(mat.Get(0, 0), 1);
    EXPECT_EQ(mat.Get(1, 1), 2);
}


TEST(TriangleMatrixTest, EqualityOperator)
{
    TriangleMatrix<int> mat1(2);
    TriangleMatrix<int> mat2(2);

    mat1.Set(1, 0, 0);
    mat1.Set(2, 1, 0);
    mat1.Set(3, 1, 1);

    mat2.Set(1, 0, 0);
    mat2.Set(2, 1, 0);
    mat2.Set(3, 1, 1);

    EXPECT_TRUE(mat1 == mat2);
}

TEST(TriangleMatrixTest, InequalityOperatorDifferentValues)
{
    TriangleMatrix<int> mat1(2);
    TriangleMatrix<int> mat2(2);

    mat1.Set(1, 0, 0);
    mat2.Set(2, 0, 0);

    EXPECT_TRUE(mat1 != mat2);
}

TEST(TriangleMatrixTest, InequalityOperatorDifferentSizes)
{
    TriangleMatrix<int> mat1(2);
    TriangleMatrix<int> mat2(3);

    EXPECT_TRUE(mat1 != mat2);
}

// Тесты для методов доступа
TEST(TriangleMatrixTest, GetSetOperations)
{
    TriangleMatrix<int> mat(3);

    mat.Set(5, 1, 0);
    mat.Set(10, 2, 1);

    EXPECT_EQ(mat.Get(1, 0), 5);
    EXPECT_EQ(mat.Get(2, 1), 10);
    EXPECT_EQ(mat.Get(0, 1), 0);
}

TEST(TriangleMatrixTest, SetAboveDiagonalThrows)
{
    TriangleMatrix<int> mat(2);

    EXPECT_THROW(mat.Set(1, 0, 1), std::invalid_argument);
}

TEST(TriangleMatrixTest, GetOutOfRangeThrows)
{
    TriangleMatrix<int> mat(2);

    EXPECT_THROW(mat.Get(2, 0), std::out_of_range);
}


TEST(SparceMatrixTest, Addition)
{
    SparceMatrix<int> mat1(3);
    SparceMatrix<int> mat2(3);

    mat1.Set(1, 0, 0);
    mat1.Set(2, 1, 1);
    mat1.Set(3, 2, 2);

    mat2.Set(4, 0, 0);
    mat2.Set(5, 1, 1);
    mat2.Set(6, 2, 2);


    SparceMatrix<int> result = mat1 + mat2;


    EXPECT_EQ(result.Get(0, 0), 5);
    EXPECT_EQ(result.Get(1, 1), 7);
    EXPECT_EQ(result.Get(2, 2), 9);
    EXPECT_EQ(result.Get(0, 1), 0);
}

TEST(SparceMatrixTest, AdditionDifferentElements)
{
    SparceMatrix<int> mat1(3);
    SparceMatrix<int> mat2(3);

    mat1.Set(1, 0, 0);
    mat1.Set(2, 1, 0);
    mat1.Set(3, 2, 1);

    mat2.Set(4, 0, 0);
    mat2.Set(5, 1, 2);
    mat2.Set(6, 2, 2);

    SparceMatrix<int> result = mat1 + mat2;

    EXPECT_EQ(result.Get(0, 0), 5);  // 1 + 4 = 5, а не 6!
    EXPECT_EQ(result.Get(1, 0), 2);
    EXPECT_EQ(result.Get(1, 2), 5);
    EXPECT_EQ(result.Get(2, 1), 3);
    EXPECT_EQ(result.Get(2, 2), 6);
}

TEST(SparceMatrixTest, AdditionDifferentDimensions)
{
    SparceMatrix<int> mat1(3);
    SparceMatrix<int> mat2(4);

    EXPECT_THROW(mat1 + mat2, std::invalid_argument);
}

TEST(SparceMatrixTest, ScalarMultiplication)
{
    SparceMatrix<int> mat(3);

    mat.Set(1, 0, 0);
    mat.Set(2, 1, 1);
    mat.Set(3, 2, 2);
    mat.Set(4, 0, 1);

    SparceMatrix<int> result = mat * 3;

    EXPECT_EQ(result.Get(0, 0), 3);   // 1 * 3
    EXPECT_EQ(result.Get(1, 1), 6);   // 2 * 3
    EXPECT_EQ(result.Get(2, 2), 9);   // 3 * 3
    EXPECT_EQ(result.Get(0, 1), 12);  // 4 * 3
}

TEST(SparceMatrixTest, ScalarMultiplicationZero)
{
    SparceMatrix<int> mat(2);

    mat.Set(5, 0, 0);
    mat.Set(10, 1, 1);

    SparceMatrix<int> result = mat * 0;


    EXPECT_EQ(result.Get(0, 0), 0);
    EXPECT_EQ(result.Get(1, 1), 0);
    EXPECT_EQ(result.Get(0, 1), 0);
}

TEST(SparceMatrixTest, MultiplicationSquareMatrices)
{
    SparceMatrix<int> A(2);
    SparceMatrix<int> B(2);

    A.Set(1, 0, 0);
    A.Set(2, 0, 1);
    A.Set(3, 1, 0);
    A.Set(4, 1, 1);

    B.Set(5, 0, 0);
    B.Set(6, 0, 1);
    B.Set(7, 1, 0);
    B.Set(8, 1, 1);

    SparceMatrix<int> C = A * B;


    EXPECT_EQ(C.Get(0, 0), 19);
    EXPECT_EQ(C.Get(0, 1), 22);
    EXPECT_EQ(C.Get(1, 0), 43);
    EXPECT_EQ(C.Get(1, 1), 50);
}

TEST(SparceMatrixTest, MultiplicationDifferentDimensionsThrows)
{
    SparceMatrix<int> A(2);
    SparceMatrix<int> B(3);

    A.Set(1, 0, 0);
    B.Set(2, 0, 0);

    EXPECT_THROW(A * B, std::invalid_argument);
}



TEST(SparceMatrixTest, SubtractionDifferentDimensionsThrows)
{
    SparceMatrix<int> mat1(2);
    SparceMatrix<int> mat2(3);

    EXPECT_THROW(mat1 - mat2, std::invalid_argument);
}

TEST(SparceMatrixTest, DivisionByScalar)
{
    SparceMatrix<int> mat(3);

    mat.Set(10, 0, 0);
    mat.Set(20, 1, 1);
    mat.Set(30, 2, 2);
    mat.Set(40, 0, 1);

    SparceMatrix<int> result = mat / 2;

    EXPECT_EQ(result.Get(0, 0), 5);
    EXPECT_EQ(result.Get(1, 1), 10);
    EXPECT_EQ(result.Get(2, 2), 15);
    EXPECT_EQ(result.Get(0, 1), 20);
}

TEST(SparceMatrixTest, DivisionByZeroThrows)
{
    SparceMatrix<int> mat(2);
    mat.Set(5, 0, 0);

    EXPECT_THROW(mat / 0, std::invalid_argument);
}

TEST(SparceMatrixTest, EqualityOperator)
{
    SparceMatrix<int> mat1(2);
    SparceMatrix<int> mat2(2);

    mat1.Set(1, 0, 0);
    mat1.Set(2, 1, 1);

    mat2.Set(1, 0, 0);
    mat2.Set(2, 1, 1);

    EXPECT_TRUE(mat1 == mat2);
}

TEST(SparceMatrixTest, InequalityOperatorDifferentValues)
{
    SparceMatrix<int> mat1(2);
    SparceMatrix<int> mat2(2);

    mat1.Set(1, 0, 0);
    mat2.Set(2, 0, 0);

    EXPECT_TRUE(mat1 != mat2);
}

TEST(SparceMatrixTest, InequalityOperatorDifferentSizes)
{
    SparceMatrix<int> mat1(2);
    SparceMatrix<int> mat2(3);

    EXPECT_TRUE(mat1 != mat2);
}

TEST(SparceMatrixTest, IteratorTraversal)
{
    SparceMatrix<int> mat(3);
    mat.Set(1, 0, 0);
    mat.Set(2, 1, 1);
    mat.Set(3, 2, 2);

    std::vector<int> values;
    for (auto it = mat.begin(); it != mat.end(); ++it) {
        values.push_back(*it);
    }

    EXPECT_EQ(values.size(), 3);
    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values[1], 2);
    EXPECT_EQ(values[2], 3);
}

TEST(SparceMatrixTest, IteratorEmptyMatrix)
{
    SparceMatrix<int> mat(2);

    auto begin = mat.begin();
    auto end = mat.end();

    EXPECT_EQ(begin, end);
}

TEST(SparceMatrixTest, ApplyLambda)
{
    SparceMatrix<int> mat(2);
    mat.Set(1, 0, 0);
    mat.Set(2, 1, 1);

    mat.ApplyLambda([](int& value) { value *= 2; });

    EXPECT_EQ(mat.Get(0, 0), 2);
    EXPECT_EQ(mat.Get(1, 1), 4);
}

TEST(SparceMatrixTest, ApplyLambdaWithZeroElements)
{
    SparceMatrix<int> mat(2);


    EXPECT_NO_THROW({
        mat.ApplyLambda([](int& value) { value *= 2; });
        });
}

TEST(SparceMatrixTest, FirstNorm)
{
    SparceMatrix<int> mat(3);
    mat.Set(1, 0, 0);
    mat.Set(-2, 0, 1);
    mat.Set(3, 1, 0);
    mat.Set(-4, 1, 1);
    mat.Set(5, 2, 2);

    // Первая норма = максимальная сумма по столбцам
    // Col0: |1| + |3| = 4
    // Col1: |-2| + |-4| = 6
    // Col2: |5| = 5
    EXPECT_EQ(mat.FirstNorm(), 6);
}

TEST(SparceMatrixTest, SecondNorm)
{
    SparceMatrix<int> mat(2);
    mat.Set(3, 0, 0);
    mat.Set(4, 1, 1);

    // Евклидова норма = sqrt(3² + 4²) = 5
    EXPECT_DOUBLE_EQ(mat.SecondNorm(), 5.0);
}

TEST(SparceMatrixTest, InfinityNorm)
{
    SparceMatrix<int> mat(3);
    mat.Set(1, 0, 0);
    mat.Set(-2, 0, 1);
    mat.Set(3, 0, 2);
    mat.Set(-4, 1, 0);
    mat.Set(5, 1, 1);
    mat.Set(-6, 1, 2);

    // Бесконечная норма = максимальная сумма по строкам
    // Row0: |1| + |-2| + |3| = 6
    // Row1: |-4| + |5| + |-6| = 15
    EXPECT_EQ(mat.InfinityNorm(), 15);
}

TEST(SparceMatrixTest, HelderNorm)
{
    SparceMatrix<int> mat(2);
    mat.Set(3, 0, 0);
    mat.Set(4, 1, 1);

    // Норма при p=2 должна совпадать со второй нормой
    EXPECT_DOUBLE_EQ(mat.HelderNorm(2.0), 5.0);

    // Норма при p=1 = |3| + |4| = 7
    EXPECT_DOUBLE_EQ(mat.HelderNorm(1.0), 7.0);
}

TEST(SparceMatrixTest, HelderNormInvalidP)
{
    SparceMatrix<int> mat(2);

    EXPECT_THROW(mat.HelderNorm(0.5), std::invalid_argument);
}

TEST(SparceMatrixTest, MoveConstructor)
{
    SparceMatrix<int> original(2);
    original.Set(5, 0, 0);

    SparceMatrix<int> moved(std::move(original));

    EXPECT_EQ(moved.Get(0, 0), 5);
    EXPECT_EQ(original.GetDimension(), 0); // original должен быть пустым
}

TEST(SparceMatrixTest, AssignmentOperator)
{
    SparceMatrix<int> mat1(2);
    SparceMatrix<int> mat2(3);

    mat1.Set(5, 0, 0);
    mat2.Set(10, 1, 1);

    mat2 = mat1;

    EXPECT_TRUE(mat1 == mat2);
    EXPECT_EQ(mat2.GetDimension(), 2);
}

TEST(SparceMatrixTest, EmptyMatrixOperations)
{
    SparceMatrix<int> mat1(0);
    SparceMatrix<int> mat2(0);

    // Операции с пустыми матрицами не должны вызывать ошибок
    EXPECT_NO_THROW(mat1 + mat2);
    EXPECT_NO_THROW(mat1 - mat2);
    EXPECT_NO_THROW(mat1 * mat2);

    EXPECT_EQ(mat1.FirstNorm(), 0);
    EXPECT_EQ(mat1.SecondNorm(), 0.0);
}

TEST(SparceMatrixTest, SetZeroRemovesElement)
{
    SparceMatrix<int> mat(2);
    mat.Set(5, 0, 0);

    EXPECT_EQ(mat.GetSize(), 1);

    mat.Set(0, 0, 0); // установка нуля должна удалить элемент

    EXPECT_EQ(mat.GetSize(), 0);
    EXPECT_EQ(mat.Get(0, 0), 0);
}

TEST(SparceMatrixTest, OutOfBoundsSetThrows)
{
    SparceMatrix<int> mat(2);

    EXPECT_THROW(mat.Set(5, 5, 5), std::out_of_range);
}

TEST(SparceMatrixTest, GetFromEmptyMatrix)
{
    SparceMatrix<int> mat(2);

    EXPECT_EQ(mat.Get(0, 0), 0);
    EXPECT_EQ(mat.Get(1, 1), 0);
}

TEST(BandMatrixTest, ConstructorAndBasicProperties)
{
    BandMatrix<int> mat(3, 2);

    EXPECT_EQ(mat.GetDimension(), 3);

}

TEST(BandMatrixTest, AdditionSameBandWidth)
{
    BandMatrix<int> mat1(3, 2);
    BandMatrix<int> mat2(3, 2);

    //Заполнение матриц (нужно добавить метод Set в BandMatrix)
    mat1.Set(1, 0, 0); mat1.Set(2, 1, 1);
    mat2.Set(3, 0, 0); mat2.Set(4, 1, 1);

    BandMatrix<int> result = mat1 + mat2;

    EXPECT_EQ(result.Get(0, 0), 4);
    EXPECT_EQ(result.Get(1, 1), 6);
}


TEST(BandMatrixTest, AdditionDifferentDimensionsThrows)
{
    BandMatrix<int> mat1(3, 2);
    BandMatrix<int> mat2(4, 2);

    EXPECT_THROW(mat1 + mat2, std::invalid_argument);
}


TEST(BandMatrixTest, ScalarMultiplication)
{
    BandMatrix<int> mat(2, 2);
    mat.Set(2, 0, 0); mat.Set(4, 1, 1);

    int scalar = 3;
    BandMatrix<int> result = mat * scalar;

    EXPECT_EQ(result.Get(0, 0), 6);
    EXPECT_EQ(result.Get(1, 1), 12);
}


TEST(BandMatrixTest, DivisionByZeroThrows)
{
    BandMatrix<int> mat(2, 2);
    int scalar = 0;

    EXPECT_THROW(mat / scalar, std::invalid_argument);
}

TEST(BandMatrixTest, EqualityOperator)
{
    BandMatrix<int> mat1(2, 2);
    BandMatrix<int> mat2(2, 2);


    mat1.Set(1, 0, 0); mat1.Set(2, 1, 1);
    mat2.Set(1, 0, 0); mat2.Set(2, 1, 1);

    EXPECT_TRUE(mat1 == mat2);
}

TEST(BandMatrixTest, InequalityOperatorDifferentValues)
{
    BandMatrix<int> mat1(2, 2);
    BandMatrix<int> mat2(2, 2);


    mat1.Set(1, 0, 0);
    mat2.Set(2, 0, 0);

    EXPECT_TRUE(mat1 != mat2);
}

TEST(BandMatrixTest, InequalityOperatorDifferentDimensions)
{
    BandMatrix<int> mat1(2, 2);
    BandMatrix<int> mat2(3, 2);

    EXPECT_TRUE(mat1 != mat2);
}

TEST(BandMatrixTest, InequalityOperatorDifferentBandWidth)
{
    BandMatrix<int> mat1(3, 2);
    BandMatrix<int> mat2(3, 3);

    EXPECT_TRUE(mat1 != mat2);
}

TEST(BandMatrixTest, CopyConstructor)
{
    BandMatrix<int> original(3, 2);
    original.Set(5, 0, 0);
    original.Set(10, 1, 1);

    BandMatrix<int> copy(original);

    EXPECT_TRUE(original == copy);
}

TEST(BandMatrixTest, MoveConstructor)
{
    BandMatrix<int> original(3, 2);
    original.Set(5, 0, 0);

    BandMatrix<int> moved(std::move(original));

    EXPECT_EQ(moved.Get(0, 0), 5);
    // Проверяем, что original в валидном состоянии
    EXPECT_EQ(original.GetDimension(), 0);
}

TEST(BandMatrixTest, AssignmentOperator)
{
    BandMatrix<int> mat1(2, 2);
    BandMatrix<int> mat2(3, 3);

    mat1.Set(5, 0, 0);

    mat2 = mat1;

    EXPECT_TRUE(mat1 == mat2);
    EXPECT_EQ(mat2.GetDimension(), 2);
}

TEST(BandMatrixTest, ApplyLambda)
{
    BandMatrix<int> mat(2, 2);
    mat.Set(1, 0, 0); mat.Set(2, 1, 1);

    mat.ApplyLambda([](int& value) { value *= 2; });

    EXPECT_EQ(mat.Get(0, 0), 2);
    EXPECT_EQ(mat.Get(1, 1), 4);
}



