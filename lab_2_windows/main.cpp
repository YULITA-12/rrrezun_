#include <iostream>
#include <vector>
#include <windows.h>
#include <chrono>

struct ThreadData {
    int start_row, start_col, block_size, N;
    const std::vector<std::vector<int>>* A;
    const std::vector<std::vector<int>>* B;
    std::vector<std::vector<int>>* C;
};

DWORD WINAPI multiplyBlockWin(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    for (int i = data->start_row; i < data->start_row + data->block_size && i < data->N; ++i) {
        for (int j = data->start_col; j < data->start_col + data->block_size && j < data->N; ++j) {
            int sum = 0;
            for (int k = 0; k < data->N; ++k) {
                sum += (*data->A)[i][k] * (*data->B)[k][j];
            }
            (*data->C)[i][j] = sum;
        }
    }

    delete data;
    return 0;
}

void createMatrix(int N, std::vector<std::vector<int>>& matrix) {
    for (int i = 0; i != N; ++i) {
        for (int j = 0; j != N; ++j) {
            matrix[i][j] = rand() % 101;
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
    std::cout << "Время однопоточного перемножения: " << duration_single.count() << " ìêñ" << std::endl;

    std::cout << "\n Многопоточное перемножение матриц:" << std::endl;

    for (int k = 1; k <= N; ++k) {
        if (N % k != 0) continue;

        std::vector<std::vector<int>> C_multi(N, std::vector<int>(N));
        std::vector<HANDLE> threadHandles;
        int num_blocks = N / k;

        auto start_multi = std::chrono::steady_clock::now();

        for (int i = 0; i < num_blocks; ++i) {
            for (int j = 0; j < num_blocks; ++j) {
                ThreadData* data = new ThreadData{ i * k, j * k, k, N, &A, &B, &C_multi };

                HANDLE hThread = CreateThread(NULL, 0, multiplyBlockWin, data, 0, NULL);
                if (hThread) {
                    threadHandles.push_back(hThread);
                }
            }
        }

        WaitForMultipleObjects(threadHandles.size(), threadHandles.data(), TRUE, INFINITE);

        for (HANDLE h : threadHandles) {
            CloseHandle(h);
        }

        auto end_multi = std::chrono::steady_clock::now();
        auto duration_multi = std::chrono::duration_cast<std::chrono::microseconds>(end_multi - start_multi);

        std::cout << "Размер блока: " << k << "x" << k
            << ", Число потоков: " << threadHandles.size()
            << ", Время: " << duration_multi.count() << " мкс" << std::endl;
    }

    return 0;

}
