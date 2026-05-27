#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <omp.h>

using namespace std;
using namespace chrono;


// Евклидовая норма вектора
double euclidean_norm(const vector<double>& v) {
    double sum = 0.0;
    for (int i = 0; i < v.size(); i++) {
        sum += v[i] * v[i];
    }
    return sqrt(sum);
}


int main() {
    const int N = 30000;
    const double tau = 1.5e-5;   // Параметр
    const double eps = 1e-5;   // Точность
    
    vector<double> x(N, 0.0);
    vector<double> b(N, N+1);
    vector<double> r(N);   // r = Ax - b
    vector<int> thread_counts = {1, 2, 4, 7, 8, 16, 20, 40};

    cout << "Threads  Time(s)    Speedup" << endl;
    cout << "---------------------------" << endl;

    double base_time = 0.0;
    
    double norm_b = euclidean_norm(b);
    double norm = 1.0;
    int iter = 0;

    for (int num_threads : thread_counts) {
        omp_set_num_threads(num_threads);
        fill(x.begin(), x.end(), 0.0);
        norm = 1.0;
        iter = 0;
    
        auto start = steady_clock::now();
        
        while (norm > eps) {

            #pragma omp parallel for
            for (int i = 0; i < N; i++) {
                double sum = 0.0;
                for (int j = 0; j < N; j++) {
                    double A_elem = (i == j) ? 2.0 : 1.0;
                    sum += A_elem * x[j];
                }
                r[i] = sum - b[i];
            }

            norm = euclidean_norm(r) / norm_b;
            
            #pragma omp parallel for
            for (int i = 0; i < N; i++) {
                x[i] = x[i] - tau * r[i];
            }
            
            iter++;
        }
        
        auto end = steady_clock::now();
        double time_sec = duration_cast<milliseconds>(end - start).count() / 1000.0;

        if (num_threads == 1) {
            base_time = time_sec;
        }

        double speedup = base_time / time_sec;

        cout << num_threads << "    " << time_sec << "    " << speedup << endl;
    }
    return 0;
}