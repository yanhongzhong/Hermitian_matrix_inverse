#include "matrix_utils.h"
#include <fstream>
#include <iostream>
#include <random>

std::vector<std::vector<complexf>> generateHermitianMatrix(int size) {
    std::vector<std::vector<complexf>> A(size, std::vector<complexf>(size));
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j <= i; ++j) {
            complexf val(dist(gen), dist(gen));
            A[i][j] = val;
            A[j][i] = std::conj(val);
        }
    }
    return A;
}

std::vector<std::vector<complexf>> loadHermitianMatrix(const std::string& filename) {
    std::ifstream in(filename);
    int size;
    in >> size;
    std::vector<std::vector<complexf>> A(size, std::vector<complexf>(size));
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            float real, imag;
            in >> real >> imag;
            A[i][j] = complexf(real, imag);
        }
    return A;
}

void saveMatrix(const std::string& filename, const std::vector<std::vector<complexf>>& matrix) {
    std::ofstream out(filename);
    int size = matrix.size();
    out << size << std::endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            out << matrix[i][j].real() << " " << matrix[i][j].imag() << " ";
        out << std::endl;
    }
}

void printMatrix(const std::vector<std::vector<complexf>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& elem : row)
            std::cout << "(" << elem.real() << "," << elem.imag() << ") ";
        std::cout << std::endl;
    }
}