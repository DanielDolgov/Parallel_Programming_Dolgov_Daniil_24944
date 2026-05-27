#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;


// Параллельная инициализация
void parallel_init(vector<double>& A, vector<double>& x, int N, int num_threads) {
    vector<thread> init_threads;
    int rows_per_thread = N / num_threads;
    
    for (int t = 0; t < num_threads; t++) {
        int start_row = t * rows_per_thread;
        int end_row = (t == num_threads - 1) ? N : start_row + rows_per_thread;
        
        init_threads.emplace_back([&, start_row, end_row]() {
            for (int i = start_row; i < end_row; i++) {
                for (int j = 0; j < N; j++) {
                    A[i * N + j] = (i == j) ? 2.0 : 1.0;
                }
                x[i] = 0.0;
            }
        });
    }
    
    for (auto& th : init_threads) {
        th.join();
    }
}


int main() {
    vector<int> sizes = {20000, 40000};
    vector<int> thread_counts = {1, 2, 4, 7, 8, 16, 20, 40};
    
    cout << fixed << setprecision(4);
    
    for (int N : sizes) {
        cout << "Matrix size: " << N << "x" << N << endl;
        cout << "Memory required: " << (double)N * N * sizeof(double) / (1024*1024*1024) << " GB\n" << endl;
        
        vector<double> A(N * N);
        vector<double> x(N);
        vector<double> b(N);
        
        parallel_init(A, x, N, thread::hardware_concurrency());
        
        cout << "Threads  Time(s)    Speedup" << endl;
        cout << "---------------------------" << endl;
        
        double base_time = 0.0;
        
        for (int num_threads : thread_counts) {
            fill(b.begin(), b.end(), 0.0);
            
            // Распределяем строки между потоками
            int rows_per_thread = N / num_threads;
            
            auto start = steady_clock::now();
            
            vector<thread> threads_vec;
            
            // Создаём потоки
            for (int t = 0; t < num_threads; t++) {
                int start_row = t * rows_per_thread;
                int end_row = (t == num_threads - 1) ? N : start_row + rows_per_thread;
                
                threads_vec.emplace_back([&, start_row, end_row]() {
                    for (int i = start_row; i < end_row; i++) {
                        double sum = 0.0;
                        for (int j = 0; j < N; j++) {
                            double A_elem = (i == j) ? 2.0 : 1.0;
                            sum += A_elem * x[j];
                        }
                        b[i] = sum;
                    }
                });
            }
            
            // Ждём завершения всех потоков
            for (auto& th : threads_vec) {
                th.join();
            }
            
            auto end = steady_clock::now();
            double time_sec = duration_cast<milliseconds>(end - start).count() / 1000.0;
            
            if (num_threads == 1) {
                base_time = time_sec;
            }
            
            double speedup = base_time / time_sec;
            cout << num_threads << "    " << time_sec << "    " << speedup << endl;
        }
    }
    
    return 0;
}