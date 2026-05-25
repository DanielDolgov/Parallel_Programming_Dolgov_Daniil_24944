#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <iomanip>

using namespace std;
using namespace chrono;

int main() {
    vector<int> sizes = {20000, 40000};
    vector<int> thread_counts = {1, 2, 4, 7, 8, 16, 20, 40};
    
    cout << fixed << setprecision(4);
    
    for (int N : sizes) {
        cout << "Matrix size: " << N << "x" << N << endl;
        cout << "Memory required: " << (double)N * N * sizeof(double) / (1024*1024*1024) << " GB\n" << endl;
        
        vector<double> A(N * N);
        vector<double> x(N, 0.0);
        vector<double> b(N);
        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i * N + j] = (i == j) ? 2.0 : 1.0;
            }
        }
        
        cout << "Threads  Time(s)    Speedup" << endl;
        cout << "---------------------------" << endl;
        
        double base_time = 0.0;
        
        for (int num_threads : thread_counts) {
            omp_set_num_threads(num_threads);
            
            fill(b.begin(), b.end(), 0.0);
            
            auto start = steady_clock::now();
            
            #pragma omp parallel for
            for (int i = 0; i < N; i++) {
                double sum = 0.0;
                for (int j = 0; j < N; j++) {
                    sum += A[i * N + j] * x[j];
                }
                b[i] = sum;
            }
            
            auto end = steady_clock::now();
            double time_sec = duration_cast<milliseconds>(end - start).count() / 1000.0;
            
            if (num_threads == 1) {
                base_time = time_sec;
            }
            
            double speedup = base_time / time_sec;
            cout << num_threads << "    " << time_sec << "    " << speedup << endl;
        }
        cout << "\n\n";
    }
    return 0;
}