#include "hermitian_cublas.h"
#include <vector>
#include <complex>
#include <cuda_runtime.h>
#include <cublas_v2.h>
#include <cusolverDn.h>
#include <stdexcept>

std::vector<std::vector<std::complex<float>>> invertHermitianCuBLAS(const std::vector<std::vector<std::complex<float>>>& A) {
    int n = A.size();
    int lda = n;
    std::vector<std::complex<float>> h_A(n*n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            h_A[i + j*n] = A[i][j];

    std::complex<float>* d_A;
    cudaMalloc(&d_A, sizeof(std::complex<float>)*n*n);
    cudaMemcpy(d_A, h_A.data(), sizeof(std::complex<float>)*n*n, cudaMemcpyHostToDevice);

    cusolverDnHandle_t solver;
    cusolverDnCreate(&solver);
    int workspace_size = 0;
    cusolverDnCpotrf_bufferSize(solver, CUBLAS_FILL_MODE_UPPER, n,
                                 reinterpret_cast<cuComplex*>(d_A), lda, &workspace_size);
    cuComplex* d_work;
    cudaMalloc(&d_work, sizeof(cuComplex)*workspace_size);
    int* devInfo;
    cudaMalloc(&devInfo, sizeof(int));

    // Cholesky factorization
    cusolverDnCpotrf(solver, CUBLAS_FILL_MODE_UPPER, n,
                     reinterpret_cast<cuComplex*>(d_A), lda,
                     d_work, workspace_size, devInfo);
    // Matrix inversion from factor
    cusolverDnCpotri(solver, CUBLAS_FILL_MODE_UPPER, n,
                     reinterpret_cast<cuComplex*>(d_A), lda,
                     d_work, workspace_size, devInfo);

    // Copy back
    cudaMemcpy(h_A.data(), d_A, sizeof(std::complex<float>)*n*n, cudaMemcpyDeviceToHost);

    // Cleanup
    cudaFree(d_A);
    cudaFree(d_work);
    cudaFree(devInfo);
    cusolverDnDestroy(solver);

    // Fill output
    std::vector<std::vector<std::complex<float>>> A_inv(n, std::vector<std::complex<float>>(n));
    for (int i = 0; i < n; ++i)
        for (int j = i; j < n; ++j) {
            A_inv[i][j] = h_A[i + j*n];
            A_inv[j][i] = std::conj(A_inv[i][j]);
        }
    return A_inv;
}