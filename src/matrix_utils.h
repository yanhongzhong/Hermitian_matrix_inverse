#pragma once
#include <vector>
#include <complex>
#include <string>

// Generate a random Hermitian matrix of given size
typedef std::complex<float> complexf;
std::vector<std::vector<complexf>> generateHermitianMatrix(int size);
// Load Hermitian matrix from file (format: size, then real imag entries)
std::vector<std::vector<complexf>> loadHermitianMatrix(const std::string& filename);
// Save matrix to file in same format
void saveMatrix(const std::string& filename, const std::vector<std::vector<complexf>>& matrix);
// Print matrix 
void printMatrix(const std::vector<std::vector<complexf>>& matrix, std::ostream& out);
