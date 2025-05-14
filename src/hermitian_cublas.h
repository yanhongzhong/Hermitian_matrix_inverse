#pragma once
#include <vector>
#include <complex>

// Inverts a Hermitian matrix using CUDA cuSOLVER (Cholesky + inversion)
std::vector<std::vector<std::complex<float>>> invertHermitianCuBLAS(
    const std::vector<std::vector<std::complex<float>>>& A);