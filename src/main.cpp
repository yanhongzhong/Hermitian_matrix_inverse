#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <getopt.h>
#include "matrix_utils.h"
#include "hermitian_recursive.h"
#include "hermitian_cublas.h"
#include "timer.h"

int main(int argc, char* argv[]) {
    bool use_recursive = false;
    bool use_generated = true;
    std::string input_file = "";
    std::string output_file = "";
    int iterations = 1;

    int opt;
    while ((opt = getopt(argc, argv, "rm:i:o:n:")) != -1) {
        switch (opt) {
            case 'r': use_recursive = true; break;
            case 'm': use_recursive = std::string(optarg) == "recursive"; break;
            case 'i': use_generated = false; input_file = optarg; break;
            case 'o': output_file = optarg; break;
            case 'n': iterations = std::stoi(optarg); break;
            default: std::cerr << "Invalid option." << std::endl; return 1;
        }
    }

    std::vector<std::vector<std::complex<float>>> A;
    if (use_generated) A = generateHermitianMatrix(1024);
    else A = loadHermitianMatrix(input_file);

    std::vector<std::vector<std::complex<float>>> A_inv;
    Timer timer;
    float total_time = 0.0f;

    for (int i = 0; i < iterations; ++i) {
        timer.start();
        if (use_recursive) {
            A_inv = invertHermitianRecursive(A);
        } else {
            A_inv = invertHermitianCuBLAS(A);
        }
        total_time += timer.stop();
    }

    if (!output_file.empty()) saveMatrix(output_file, A_inv);
    else printMatrix(A_inv);

    std::cout << "Average time: " << (total_time / iterations) << " ms" << std::endl;
    return 0;
}