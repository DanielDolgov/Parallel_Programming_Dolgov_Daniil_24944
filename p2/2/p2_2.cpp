#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace chrono;


double func(double x) {
    return sin(x) * cos(x); // Площадь = 0, если [0, pi]
}

// Параллельное интегрирование
double integrate_omp(double a, double b, int nsteps) {
    double h = (b - a) / nsteps;
    double sum = 0.0;
    
    #pragma omp parallel
    {
        double local_sum = 0.0;
        
        #pragma omp for
        for (int i = 0; i < nsteps; i++) {
            double x = a + (i + 0.5) * h;
            local_sum += func(x);
        }
        
        #pragma omp atomic
        sum += local_sum;
    }
    
    return sum * h;
}


int main() {
    const double a = 0.0;
    const double b = M_PI;
    const int nsteps = 40000000;
    
    vector<int> thread_counts = {1, 2, 4, 7, 8, 16, 20, 40};
    
    cout << fixed << setprecision(4);
    
    cout << "Интервал: [" << a << ", " << b << "]" << endl;
    cout << "Количество точек: " << nsteps << endl;
    cout << "Аналитически: Result = 0\n" << endl;
    
    cout << "Threads  Time(s)    Result    Speedup" << endl;
    cout << "-------------------------------------" << endl;
    
    double base_time = 0.0;
    
    for (int num_threads : thread_counts) {
        omp_set_num_threads(num_threads);
        
        auto start = steady_clock::now();
        double result = integrate_omp(a, b, nsteps);
        auto end = steady_clock::now();
        
        double time_sec = duration_cast<milliseconds>(end - start).count() / 1000.0;
        
        if (num_threads == 1) {
            base_time = time_sec;
        }
        
        double speedup = base_time / time_sec;
        cout << num_threads << "        " << time_sec << "    " << result << "    " << speedup << endl;
    }
    
    return 0;
}