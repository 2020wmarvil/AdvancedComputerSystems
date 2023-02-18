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
        size_t sizeToAlloc = size*size;

        int remainder = sizeToAlloc % 32;
        if (remainder != 0) sizeToAlloc = sizeToAlloc + 32 - remainder;
        
        data = (T*)calloc(sizeToAlloc, sizeof(T));
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

void MultSIMD_Short(const Matrix<int16_t>& mat1, const Matrix<int16_t>& mat2, Matrix<int16_t>& result)
{
    size_t N = mat1.size;
    result.size = N;

    __m256i vec_multi = _mm256_setzero_si256();
    __m256i vec_mat1 = _mm256_setzero_si256();
    __m256i vec_mat2 = _mm256_setzero_si256();
    
    for (int i = 0; i < N; i += 16) {
        for (int j = 0; j < N; ++j) {
            vec_mat2 = _mm256_set_epi16(
                mat2.data[(i+0)*N+j], 
                mat2.data[(i+1)*N+j],
                mat2.data[(i+2)*N+j],
                mat2.data[(i+3)*N+j],
                mat2.data[(i+4)*N+j],
                mat2.data[(i+5)*N+j],
                mat2.data[(i+6)*N+j],
                mat2.data[(i+7)*N+j],
                mat2.data[(i+8)*N+j], 
                mat2.data[(i+9)*N+j],
                mat2.data[(i+10)*N+j],
                mat2.data[(i+11)*N+j],
                mat2.data[(i+12)*N+j],
                mat2.data[(i+13)*N+j],
                mat2.data[(i+14)*N+j],
                mat2.data[(i+15)*N+j]
            );
    
            for (int k = 0; k < N; ++k) {
                vec_mat1 = _mm256_loadu_si256((__m256i*)&mat1.data[k*N+i]);
                vec_multi = _mm256_add_epi16(vec_multi ,_mm256_mullo_epi16(vec_mat1, vec_mat2));
    
                result.data[k*N+j] += 
                      _mm256_extract_epi16(vec_multi, 0) 
                    + _mm256_extract_epi16(vec_multi, 1) 
                    + _mm256_extract_epi16(vec_multi, 2) 
                    + _mm256_extract_epi16(vec_multi, 3) 
                    + _mm256_extract_epi16(vec_multi, 4) 
                    + _mm256_extract_epi16(vec_multi, 5) 
                    + _mm256_extract_epi16(vec_multi, 6) 
                    + _mm256_extract_epi16(vec_multi, 7)
                    + _mm256_extract_epi16(vec_multi, 8) 
                    + _mm256_extract_epi16(vec_multi, 9) 
                    + _mm256_extract_epi16(vec_multi, 10) 
                    + _mm256_extract_epi16(vec_multi, 11) 
                    + _mm256_extract_epi16(vec_multi, 12) 
                    + _mm256_extract_epi16(vec_multi, 13) 
                    + _mm256_extract_epi16(vec_multi, 14) 
                    + _mm256_extract_epi16(vec_multi, 15);
    
                vec_multi = _mm256_setzero_si256();
            }
        }
    }
}

void MultSIMD_Float(const Matrix<float>& mat1, const Matrix<float>& mat2, Matrix<float>& result)
{
    size_t N = mat1.size;
    result.size = N;

    size_t jb = std::min(512u, (uint32_t)N);
    size_t kb = std::min(24u, (uint32_t)N);
    
    for (size_t jj = 0; jj < N; jj += jb) {
        for (size_t kk = 0; kk < N; kk += kb) {
            for (size_t i = 0; i < N; i += 1) {
                for (size_t j = jj; j < jj + jb; j += 16) {
                    __m256 sumA_1, sumB_1;
                    if (kk == 0) {
                        sumA_1 = sumB_1 = _mm256_setzero_ps();
                    } else {
                        sumA_1 = _mm256_loadu_ps((float*)&result.data[i*N+j]);
                        sumB_1 = _mm256_loadu_ps((float*)&result.data[i*N+j + 8]);
                    }

                    size_t limit = std::min((uint32_t)N, (uint32_t)(kk + kb));
                    for (size_t k = kk; k < limit; k++) {
                        auto bc_mat1_1 = _mm256_set1_ps(mat1.data[i*N+k]);
                        auto vecA_mat2 = _mm256_loadu_ps((float*)&mat2.data[k*N+j]);
                        auto vecB_mat2 = _mm256_loadu_ps((float*)&mat2.data[k*N+j + 8]);
                        sumA_1 = _mm256_add_ps(sumA_1, _mm256_mul_ps(bc_mat1_1, vecA_mat2));
                        sumB_1 = _mm256_add_ps(sumB_1, _mm256_mul_ps(bc_mat1_1, vecB_mat2));
                    }

                    _mm256_storeu_ps((float*)&result.data[i*N+j], sumA_1);
                    _mm256_storeu_ps((float*)&result.data[i*N+j + 8], sumB_1);
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

template<typename T>
inline bool operator==(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    if (lhs.size != rhs.size) return false;

    for (size_t r = 0; r < lhs.size; r++)
    {
        for (size_t c = 0; c < lhs.size; c++)
        {
            if (lhs.data[r * lhs.size + c] != rhs.data[r * rhs.size + c]) return false;
        }
    }

    return true;
}

template<typename T>
inline bool operator!=(const Matrix<T>& lhs, const Matrix<T>& rhs){return !operator==(lhs,rhs);}

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
    duration<double> elapsed_seconds = end - start;
    for (size_t iter = 0; iter < numIterations; iter++)
    {
        // Float32
        {
            Matrix<float> mat1(matrixSize);
            Matrix<float> mat2(matrixSize);
            Matrix<float> result(matrixSize);
            Matrix<float> resultSIMD(matrixSize);

            mat1.Randomize();
            mat2.Randomize();

            start = system_clock::now();
            Mult(mat1, mat2, result);
            end = system_clock::now();
            elapsed_seconds = end - start;
            std::cout << "Float32 NonSIMD: " << elapsed_seconds.count() << "s\n";

            start = system_clock::now();
            MultSIMD_Float(mat1, mat2, resultSIMD);
            end = system_clock::now();
            elapsed_seconds = end - start;
            std::cout << "Float32 SIMD: " << elapsed_seconds.count() << "s\n";
        }

        // Int16
        {
            Matrix<int16_t> mat1(matrixSize);
            Matrix<int16_t> mat2(matrixSize);
            Matrix<int16_t> result(matrixSize);
            Matrix<int16_t> resultSIMD(matrixSize);

            mat1.Randomize();
            mat2.Randomize();

            start = system_clock::now();
            Mult(mat1, mat2, result);
            end = system_clock::now();
            elapsed_seconds = end - start;
            std::cout << "Int16 NonSIMD: " << elapsed_seconds.count() << "s\n";

            start = system_clock::now();
            MultSIMD_Short(mat1, mat2, resultSIMD);
            end = system_clock::now();
            elapsed_seconds = end - start;
            std::cout << "Int16 SIMD: " << elapsed_seconds.count() << "s\n";
        }
    }

    return 0;
}