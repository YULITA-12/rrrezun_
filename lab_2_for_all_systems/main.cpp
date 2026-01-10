#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

struct ThreadData {
    int start_row, start_col, block_size, N;
    const std::vector<std::vector<int>>* A;
    const std::vector<std::vector<int>>* B;
    std::vector<std::vector<int>>* C;
};

void multiplyBlock(ThreadData data) {
    for (int i = data.start_row; i < data.start_row + data.block_size && i < data.N; ++i) {
        for (int j = data.start_col; j < data.start_col + data.block_size && j < data.N; ++j) {
            int sum = 0;
            for (int k = 0; k < data.N; ++k) {
                sum += (*data.A)[i][k] * (*data.B)[k][j];
            }
            (*data.C)[i][j] = sum;
        }
    }
}

void singleThreadMultiply(int N, const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, std::vector<std::vector<int>>& C) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


void createMatrix(int N, std::vector<std::vector<int>>& matrix) {
    for (int i = 0; i != N; ++i) {
        for (int j = 0; j != N; ++j) {
            matrix[i][j] = rand() % 101;
        }
    }
}

int main() {

    setlocale(LC_ALL, "Russian");
    int N = 10;
    std::vector<std::vector<int>> A(N, std::vector<int>(N));
    std::vector<std::vector<int>> B(N, std::vector<int>(N));

    createMatrix(N, A);
    createMatrix(N, B);

    std::vector<std::vector<int>> C_single(N, std::vector<int>(N));
    auto start_single = std::chrono::steady_clock::now();
    singleThreadMultiply(N, A, B, C_single);
    auto end_single = std::chrono::steady_clock::now();
    auto duration_single = std::chrono::duration_cast<std::chrono::microseconds>(end_single - start_single);
    std::cout << "Время однопоточного выполнения: " << duration_single.count() << " мкс" << std::endl;

    std::cout << "\n--- Многопоточное умножение (<thread>) ---" << std::endl;

    for (int k = 1; k <= N; ++k) {
        if (N % k != 0) continue;

        std::vector<std::vector<int>> C_multi(N, std::vector<int>(N));
        std::vector<std::thread> threads;
        int num_blocks = N / k;

        auto start_multi = std::chrono::steady_clock::now();

        for (int i = 0; i < num_blocks; ++i) {
            for (int j = 0; j < num_blocks; ++j) {
                ThreadData data = { i * k, j * k, k, N, &A, &B, &C_multi };
                threads.emplace_back(multiplyBlock, data);
            }
        }

        for (auto& th : threads) {
            th.join();
        }

        auto end_multi = std::chrono::steady_clock::now();
        auto duration_multi = std::chrono::duration_cast<std::chrono::microseconds>(end_multi - start_multi);

        std::cout << "Размер блока: " << k << "x" << k
            << ", Число потоков: " << threads.size()
            << ", Время: " << duration_multi.count() << " мкс" << std::endl;
    }

    return 0;
}
