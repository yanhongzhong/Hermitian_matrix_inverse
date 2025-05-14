#include "hermitian_recursive.h"
#include <complex>
#include <vector>

static std::vector<std::vector<std::complex<float>>> multiply(const std::vector<std::vector<std::complex<float>>>& X,
                                                                const std::vector<std::vector<std::complex<float>>>& Y) {
    int n = X.size();
    std::vector<std::vector<std::complex<float>>> Z(n, std::vector<std::complex<float>>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int k = 0; k < n; ++k)
            for (int j = 0; j < n; ++j)
                Z[i][j] += X[i][k] * Y[k][j];
    return Z;
}

std::vector<std::vector<std::complex<float>>> invertHermitianRecursive(const std::vector<std::vector<std::complex<float>>>& A) {
    int n = A.size();
    if (n == 1) {
        return {{ std::complex<float>(1.0f) / A[0][0] }};
    }
    // Partition A = [a11, B; C, D]
    std::complex<float> a11 = A[0][0];
    std::vector<std::vector<std::complex<float>>> B(1, std::vector<std::complex<float>>(n-1));
    std::vector<std::vector<std::complex<float>>> C(n-1, std::vector<std::complex<float>>(1));
    std::vector<std::vector<std::complex<float>>> D(n-1, std::vector<std::complex<float>>(n-1));
    for (int j = 1; j < n; ++j) B[0][j-1] = A[0][j];
    for (int i = 1; i < n; ++i) C[i-1][0] = A[i][0];
    for (int i = 1; i < n; ++i)
        for (int j = 1; j < n; ++j)
            D[i-1][j-1] = A[i][j];

    auto D_inv = invertHermitianRecursive(D);
    // Schur complement: S = a11 - B * D_inv * C
    auto temp = multiply(B, multiply(D_inv, C));
    std::complex<float> S = a11 - temp[0][0];
    std::complex<float> S_inv = std::complex<float>(1.0f) / S;

    // Build inverse matrix
    std::vector<std::vector<std::complex<float>>> A_inv(n, std::vector<std::complex<float>>(n));
    // (1,1)
    A_inv[0][0] = S_inv;
    // (1,2) and (2,1)
    // inv12 = -S_inv * B * D_inv
    auto inv12 = multiply(B, D_inv);
    for (int j = 1; j < n; ++j) A_inv[0][j] = -S_inv * inv12[0][j-1];
    for (int i = 1; i < n; ++i) A_inv[i][0] = std::conj(A_inv[0][i]);
    // (2,2)
    // inv22 = D_inv + S_inv * D_inv * C * B * D_inv
    auto D_inv_C = multiply(D_inv, C);
    auto D_inv_C_B = multiply(std::vector<std::vector<std::complex<float>>>(D_inv_C.size(), std::vector<std::complex<float>>(B[0].size())), std::vector<std::vector<std::complex<float>>>());
    // Actually compute D_inv * C * B * D_inv
    auto CB = multiply(C, B);
    auto mid = multiply(D_inv, CB);
    auto term = multiply(mid, D_inv);
    for (int i = 0; i < n-1; ++i)
        for (int j = 0; j < n-1; ++j)
            A_inv[i+1][j+1] = D_inv[i][j] + S_inv * term[i][j];

    return A_inv;
}