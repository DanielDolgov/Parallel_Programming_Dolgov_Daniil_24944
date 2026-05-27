#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <iomanip>

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
    
    double norm_b = euclidean_norm(b);
    double norm = 1.0;
    int iter = 0;
    
    auto start = steady_clock::now();
    
    while (norm > eps) {

        // Умножение матрицы на вектор
        for (int i = 0; i < N; i++) {
            double sum = 0.0;
            for (int j = 0; j < N; j++) {
                double A_elem = (i == j) ? 2.0 : 1.0;
                sum += A_elem * x[j];
            }
            r[i] = sum - b[i];
        }

        norm = euclidean_norm(r) / norm_b;
        
        for (int i = 0; i < N; i++) {
            x[i] = x[i] - tau * r[i];
        }
        
        iter++;
        
        if (iter % 1 == 0) {
            cout << iter << ": norm = " << norm << endl;
        }
    }
    
    auto end = steady_clock::now();
    double time_sec = duration_cast<milliseconds>(end - start).count() / 1000.0;
    
    cout << "\nIterations: " << iter << endl;
    cout << "Time(s): " << time_sec << endl;
    cout << "Final norm: " << norm << endl;
    cout << "x[0] = " << x[0] << endl;
    
    return 0;
}