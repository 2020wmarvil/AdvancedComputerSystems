#include <chrono>
#include <cstdlib>
#include <ctime> 
#include <iostream>
#include <limits>
#include <stddef.h>

template<typename T>
struct Matrix
{
    Matrix(size_t size)
        : size(size)
    {
        data = (T*)calloc(size * size, sizeof(T));
    }

    ~Matrix()
    {
        free(data);
    }

    Matrix (const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    void Randomize()
    {
        for (size_t i = 0; i < size * size; i++)
        {
            float randomValue = (float)std::rand() / RAND_MAX; // [0, 1]
            data[i] = randomValue * 32000; // [0, 32000]
        }
    }

    T* data;
    size_t size;
};

template<typename T>
void Mult(const Matrix<T>& mat1, const Matrix<T>& mat2, Matrix<T>& result)
{
    // TODO: multiplication
    size_t size = mat1.size;
    result.size = size;

    for (size_t r = 0; r < size; r++)
    {
        for (size_t c = 0; c < size; c++)
        {
            T sum = 0;

            for (size_t i = 0; i < size; i++)
            {
                sum += mat1.data[r * size + i] * mat2.data[i * size + c];
            }

            result.data[r * size + c] = sum;
        }
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat)
{
    os.precision(16);

    for (size_t r = 0; r < mat.size; r++)
    {
        for (size_t c = 0; c < mat.size; c++)
        {
            os << mat.data[r * mat.size + c] << ", ";
        } os << "\n";
    }

    return os;
}

enum class MatrixType { FLOAT32, FLOAT64, INT32, INT64 };

int main(int argc, const char** argv)
{
    if (argc < 3)
    {
        std::cerr << "Wrong arguments. Usage:\n";
        std::cerr << argv[0] << " [SQUARE_MATRIX_SIZE] [NUM_ITERATIONS]\n";
        return 1;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr))); 

    size_t matrixSize = atoi(argv[1]);
    size_t numIterations = atoi(argv[2]);

    // TODO: parse type
    MatrixType matrixType = MatrixType::FLOAT32;

    std::cout << "Performing " << numIterations << " " << matrixSize << "x" << matrixSize << " matrix calculations.\n";

    using namespace std::chrono;

    time_point<system_clock> start, end;
    for (size_t iter = 0; iter < numIterations; iter++)
    {
        if (matrixType == MatrixType::FLOAT32)
        {
            Matrix<double> mat1(matrixSize);
            Matrix<double> mat2(matrixSize);
            Matrix<double> result(matrixSize);

            mat1.Randomize();
            mat2.Randomize();

            start = system_clock::now();
            Mult(mat1, mat2, result);
            end = system_clock::now();

            duration<double> elapsed_seconds = end - start;
            std::cout << elapsed_seconds.count() << "s\n";
        }
    }

    return 0;
}