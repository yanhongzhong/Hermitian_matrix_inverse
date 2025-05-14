#pragma once
#include <vector>
#include <complex>

// Inverts a Hermitian matrix using the recursive bordering algorithm
std::vector<std::vector<std::complex<float>>> invertHermitianRecursive(
    const std::vector<std::vector<std::complex<float>>>& A);