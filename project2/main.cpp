#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime> 
#include <immintrin.h>
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
        //free(data);
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

// Reference: https://codereview.stackexchange.com/questions/177616/avx-simd-in-matrix-multiplication
template<typename T>
void MultSIMD(const Matrix<T>& mat1, const Matrix<T>& mat2, Matrix<T>& result)
{
    size_t N = mat1.size;
    result.size = N;

    size_t jb = std::min(512u, (uint32_t)N);
    size_t kb = std::min(24u, (uint32_t)N);
    
    for (size_t jj = 0; jj < N; jj += jb)
    {
        for (size_t kk = 0; kk < N; kk += kb)
        {
            for (size_t i = 0; i < N; i += 1) {
                for (size_t j = jj; j < jj + jb; j += 16) {
                    __m256i sumA_1, sumB_1;
                    if (kk == 0) {
                        sumA_1 = sumB_1 = _mm256_setzero_si256();
                    }
                    else {
                        sumA_1 = _mm256_load_si256((__m256i*)&result.data[i*N+j]);
                        sumB_1 = _mm256_load_si256((__m256i*)&result.data[i*N+j + 8]);
                    }

                    size_t limit = std::min((uint32_t)N, (uint32_t)(kk + kb));
                    for (size_t k = kk; k < limit; k++) {
                        auto bc_mat1_1 = _mm256_set1_epi32(mat1.data[i*N+k]);
                        auto vecA_mat2 = _mm256_loadu_si256((__m256i*)&mat2.data[k*N+j]);
                        auto vecB_mat2 = _mm256_loadu_si256((__m256i*)&mat2.data[k*N+j + 8]);
                        sumA_1 = _mm256_add_epi32(sumA_1, _mm256_mullo_epi32(bc_mat1_1, vecA_mat2));
                        sumB_1 = _mm256_add_epi32(sumB_1, _mm256_mullo_epi32(bc_mat1_1, vecB_mat2));
                    }

                    _mm256_storeu_si256((__m256i*)&result.data[i*N+j], sumA_1);
                    _mm256_storeu_si256((__m256i*)&result.data[i*N+j + 8], sumB_1);
                }
            }
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

int main(int argc, const char** argv)
{
    if (argc < 4)
    {
        std::cerr << "Wrong arguments. Usage:\n";
        std::cerr << argv[0] << " [SQUARE_MATRIX_SIZE] [NUM_ITERATIONS] [USE_SIMD]\n";
        return 1;
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr))); 

    size_t matrixSize = atoi(argv[1]);
    size_t numIterations = atoi(argv[2]);
    size_t bUseSIMD = atoi(argv[3]);

    std::cout << "Performing " << numIterations << " " << matrixSize << "x" << matrixSize << " matrix calculations.\n";

    using namespace std::chrono;

    time_point<system_clock> start, end;
    for (size_t iter = 0; iter < numIterations; iter++)
    {
        // Float32
        {
            Matrix<float> mat1(matrixSize);
            Matrix<float> mat2(matrixSize);
            Matrix<float> result(matrixSize);

            mat1.Randomize();
            mat2.Randomize();

            start = system_clock::now();
            if (bUseSIMD) MultSIMD(mat1, mat2, result);
            else Mult(mat1, mat2, result);
            end = system_clock::now();

            duration<double> elapsed_seconds = end - start;
            std::cout << "Float32: " << elapsed_seconds.count() << "s\n";
        }

        // Float64
        {
            Matrix<double> mat1(matrixSize);
            Matrix<double> mat2(matrixSize);
            Matrix<double> result(matrixSize);

            mat1.Randomize();
            mat2.Randomize();

            start = system_clock::now();
            if (bUseSIMD) MultSIMD(mat1, mat2, result);
            else Mult(mat1, mat2, result);
            end = system_clock::now();

            duration<double> elapsed_seconds = end - start;
            std::cout << "Float64: " << elapsed_seconds.count() << "s\n";
        }

        // Int16
        {
            Matrix<int16_t> mat1(matrixSize);
            Matrix<int16_t> mat2(matrixSize);
            Matrix<int16_t> result(matrixSize);

            mat1.Randomize();
            mat2.Randomize();

            start = system_clock::now();
            if (bUseSIMD) MultSIMD(mat1, mat2, result);
            else Mult(mat1, mat2, result);
            end = system_clock::now();

            duration<double> elapsed_seconds = end - start;
            std::cout << "Int16: " << elapsed_seconds.count() << "s\n";
        }

        // Int32
        {
            Matrix<int32_t> mat1(matrixSize);
            Matrix<int32_t> mat2(matrixSize);
            Matrix<int32_t> result(matrixSize);

            mat1.Randomize();
            mat2.Randomize();

            start = system_clock::now();
            if (bUseSIMD) MultSIMD(mat1, mat2, result);
            else Mult(mat1, mat2, result);
            end = system_clock::now();

            duration<double> elapsed_seconds = end - start;
            std::cout << "Int32: " << elapsed_seconds.count() << "s\n";
        }

        // Int64
        {
            Matrix<int64_t> mat1(matrixSize);
            Matrix<int64_t> mat2(matrixSize);
            Matrix<int64_t> result(matrixSize);

            mat1.Randomize();
            mat2.Randomize();

            start = system_clock::now();
            if (bUseSIMD) MultSIMD(mat1, mat2, result);
            else Mult(mat1, mat2, result);
            end = system_clock::now();

            duration<double> elapsed_seconds = end - start;
            std::cout << "Int64: " << elapsed_seconds.count() << "s\n";
        }
    }

    return 0;
}