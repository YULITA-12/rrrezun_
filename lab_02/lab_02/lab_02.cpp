#include "matrix_functions.h"
#include <iostream>
#include <chrono>

int main() {
    int matrix_size = 6;

    std::cout << "=== MATRIX MULTIPLICATION TEST ===" << std::endl;
    std::cout << "Matrix size: " << matrix_size << "x" << matrix_size << std::endl << std::endl;

    auto A = createMatrix(matrix_size);
    auto B = createMatrix(matrix_size);

    std::cout << "1. SIMPLE MULTIPLICATION" << std::endl;
    auto start1 = std::chrono::high_resolution_clock::now();
    auto result_normal = simpleMultiply(A, B);
    auto end1 = std::chrono::high_resolution_clock::now();
    long time1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();
    std::cout << "Time: " << time1 << " microseconds" << std::endl << std::endl;

  
    std::cout << "2. MULTITHREADED MULTIPLICATION (std::thread)" << std::endl;

    for (int block_size = 1; block_size <= matrix_size; block_size++) {
        std::cout << "Block " << block_size << "x" << block_size << ":" << std::endl;

        int blocks_count = ((matrix_size + block_size - 1) / block_size);
        int total_threads = blocks_count * blocks_count;

        auto start2 = std::chrono::high_resolution_clock::now();
        auto result2 = multiplyWithThreads(A, B, block_size);
        auto end2 = std::chrono::high_resolution_clock::now();
        long time2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();

        std::cout << "   Time: " << time2 << " mcs, ";
        std::cout << "Threads: " << total_threads << ", ";
        std::cout << (checkEqual(result2, result_normal) ? "OK" : "ERROR") << std::endl << std::endl;
    }

    std::cout << "=== TEST COMPLETED ===" << std::endl;
    return 0;
}