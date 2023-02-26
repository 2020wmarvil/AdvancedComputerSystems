#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime> 
#include <immintrin.h>
#include <iostream>
#include <limits>
#include <stddef.h>
#include <string>

int main(int argc, const char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Wrong arguments. Usage:\n";
        std::cerr << argv[0] << " [FILENAME]\n";
        return EXIT_FAILURE;
    }

    std::string file = argv[1];

    using namespace std::chrono;

    time_point<system_clock> start, end;
    duration<double> elapsed_seconds = end - start;

    std::cout << "hello world\n";

    return EXIT_SUCCESS;
}