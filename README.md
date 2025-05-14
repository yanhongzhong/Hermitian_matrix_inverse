# Hermitian Matrix Inversion App
## Purpose
This application performs Hermitian matrix inversion using NVIDIA's cuBLAS/cuSOLVER libraries, exploiting GPU acceleration for high performance. Hermitian matrix inversion is very useful in statistical signal processing and optimization, for example in MMSE estimation/detection and in Newton's method for solving non-linear problems.
## Build Instructions
Ensure you have the NVIDIA CUDA Toolkit installed with cuBLAS and cuSOLVER. Then run:
```bash
make
```
## Usage
```bash
./bin/hermitian_inverter [-i input_file] [-o output_file] [-n iterations] [-s size] [-h]
```
## Options
* -i <file> Input Hermitian matrix file (format described below). If omitted, a random N×N Hermitian matrix is generated.

* -o <file> Output file for the inverted matrix. If omitted, prints to stdout.

* -n <iterations> Number of inversion runs for timing statistics (default: 1).

* -s <size> Dimension N of the generated N×N matrix (default: 16).

* -h Display this help message.

## Input/Output Format
Matrices are stored as:

<size>
real imag  real imag  ...  (size entries per row)
...

## Example
```bash
./bin/hermitian_inverter -s 100 -n 1000 -o inv_100_results
```
expected std output is like
```bash
# Average inversion time (ms): 4.84971
# Verification Frobenius error ||A*A_inv - I||_F: 4.75471e-05
``` 