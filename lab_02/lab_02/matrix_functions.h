#pragma once
#include <vector>

std::vector<std::vector<int>> createMatrix(int size);

std::vector<std::vector<int>> simpleMultiply(std::vector<std::vector<int>> A, std::vector<std::vector<int>> B);

std::vector<std::vector<int>> multiplyWithThreads(std::vector<std::vector<int>> A, std::vector<std::vector<int>> B, int block);

bool checkEqual(std::vector<std::vector<int>> A, std::vector<std::vector<int>> B);

void printMatrix(std::vector<std::vector<int>> matrix);