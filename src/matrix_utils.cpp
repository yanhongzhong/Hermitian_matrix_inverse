#include "matrix_utils.h"
#include <fstream>
#include <iostream>
#include <random>

std::vector<std::vector<complexf>> generateHermitianMatrix(int size) {
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    // Step 1: Generate random complex matrix B
    std::vector<std::vector<complexf>> B(size, std::vector<complexf>(size));
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            B[i][j] = complexf(dist(gen), dist(gen));

    // Step 2: Compute A = Bᴴ * B (guaranteed Hermitian and positive semi-definite)
    std::vector<std::vector<complexf>> A(size, std::vector<complexf>(size, complexf(0.0f, 0.0f)));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j <= i; ++j) {
            complexf sum(0.0f, 0.0f);
            for (int k = 0; k < size; ++k)
                sum += std::conj(B[k][i]) * B[k][j];
            A[i][j] = sum;
            A[j][i] = std::conj(sum); // ensure Hermitian
        }
    }

    // Optional: Improve conditioning (push eigenvalues up)
    for (int i = 0; i < size; ++i) {
        A[i][i] += complexf(1.0f, 0.0f);  // shift eigenvalues
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

void printMatrix(const std::vector<std::vector<complexf>>& matrix,
                 std::ostream& out) {
    int n = matrix.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            auto& c = matrix[i][j];
            out << "(" << c.real() << "," << c.imag() << ") ";
        }
        out << "\n";
    }
}