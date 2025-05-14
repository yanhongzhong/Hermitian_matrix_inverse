#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <getopt.h>
#include "matrix_utils.h"
#include "hermitian_cublas.h"
#include "timer.h"

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " [options]\n"
              << "  -i <file>       Input Hermitian matrix file (default: generate random)\n"
              << "  -o <file>       Output file for inverted matrix (default: stdout)\n"
              << "  -n <iterations> Number of inversion runs (default: 1)\n"
              << "  -s <size>       Size N of generated N x N matrix (default: 16)\n"
              << "  -h              Display this help message\n";
}

float verify_inverse(const std::vector<std::vector<std::complex<float>>>& A,
                     const std::vector<std::vector<std::complex<float>>>& A_inv) {
    int n = A.size();
    // Compute P = A * A_inv
    std::vector<std::vector<std::complex<float>>> P(n, std::vector<std::complex<float>>(n, {0,0}));
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            for (int j = 0; j < n; ++j) {
                P[i][j] += A[i][k] * A_inv[k][j];
            }
        }
    }
    // Compute Frobenius norm of (P - I)
    float sum_sq = 0.0f;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::complex<float> diff = P[i][j] - (i==j ? std::complex<float>(1,0) : std::complex<float>(0,0));
            sum_sq += std::norm(diff);  // norm = |diff|^2
        }
    }
    return std::sqrt(sum_sq);
}

int main(int argc, char* argv[]) {
    bool use_generated = true;
    std::string input_file = "";
    std::string output_file = "";
    int iterations = 1;
    int mat_size = 16; 

    int opt;
    while ((opt = getopt(argc, argv, "i:o:n:s:h")) != -1) {
        switch (opt) {
            case 'i': use_generated = false; input_file = optarg; break;
            case 'o': output_file = optarg; break;
            case 'n': iterations = std::stoi(optarg); break;
            case 's': mat_size = std::stoi(optarg); break;
            case 'h': print_usage(argv[0]); return 0;
            default:
                std::cerr << "Unknown option '" << char(opt) << "'\n";
                print_usage(argv[0]);
                return 1;
        }
    }

    std::vector<std::vector<std::complex<float>>> A;
    if (use_generated) A = generateHermitianMatrix(mat_size);
    else A = loadHermitianMatrix(input_file);

    std::vector<std::vector<std::complex<float>>> A_inv;
    Timer timer;
    float total_time = 0.0f;

    timer.start();
    for (int i = 0; i < iterations; ++i) {
        A_inv = invertHermitianCuBLAS(A);
    }
    total_time += timer.stop();
    float avg_time = total_time / iterations;

    float frob_err = verify_inverse(A, A_inv);

    if (!output_file.empty()) {
        std::ofstream out(output_file);
        out << "# Input matrix A (" << mat_size << "x" << mat_size << ")\n";
        printMatrix(A, out);
        out << "\n# Inverted matrix A_inv\n";
        printMatrix(A_inv, out);
        out << "\n# Average inversion time (ms): " << avg_time << "\n";
        out << "# Verification Frobenius error ||A*A_inv - I||_F: " << frob_err << "\n";
    } else {
        std::cout << "# Input matrix A (" << mat_size << "x" << mat_size << ")\n";
        printMatrix(A, std::cout);
        std::cout << "\n# Inverted matrix A_inv\n";
        printMatrix(A_inv, std::cout);
        std::cout << "\n# Average inversion time (ms): " << avg_time << "\n";
        std::cout << "# Verification Frobenius error ||A*A_inv - I||_F: " << frob_err << "\n";
    }

    return 0;
}