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

int main(int argc, char* argv[]) {
    bool use_recursive = false;
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

    if (!output_file.empty()) saveMatrix(output_file, A_inv);
    else printMatrix(A_inv);

    std::cout << "Average time: " << (total_time / iterations) << " ms" << std::endl;
    return 0;
}