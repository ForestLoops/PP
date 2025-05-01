#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;
const int N = 1000;

int main() {
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C_serial(N, vector<int>(N, 0));
    vector<vector<int>> C_parallel(N, vector<int>(N, 0));

    // Initialize matrices A and B with random values
    srand(time(0));  // Add seed for randomness
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }

    // --- Serial Matrix Multiplication ---
    auto start_serial = chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++)
                sum += A[i][k] * B[k][j];
            C_serial[i][j] = sum;
        }
    auto end_serial = chrono::high_resolution_clock::now();
    auto duration_serial = chrono::duration_cast<chrono::milliseconds>(end_serial - start_serial);

    // --- Parallel Matrix Multiplication ---
    auto start_parallel = chrono::high_resolution_clock::now();
    #pragma omp parallel for
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++)
                sum += A[i][k] * B[k][j];
            C_parallel[i][j] = sum;
        }
    auto end_parallel = chrono::high_resolution_clock::now();
    auto duration_parallel = chrono::duration_cast<chrono::milliseconds>(end_parallel - start_parallel);

    // --- Output Times ---
    cout << "Time taken for SERIAL matrix multiplication   : " << duration_serial.count() << " ms" << endl;
    cout << "Time taken for PARALLEL matrix multiplication : " << duration_parallel.count() << " ms" << endl;

    return 0;
}
