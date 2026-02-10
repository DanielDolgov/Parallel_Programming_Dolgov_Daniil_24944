#include <iostream>
#include <vector>
#include <cmath>

int main() {
    #ifdef DOUBLE
        double sum = 0;
        std::vector<double> sins;
        std::string type = "Сумма double: ";
    #else
        float sum = 0;
        std::vector<float> sins;
        std::string type = "Сумма float: ";
    #endif

    int cnt = pow(10, 7);
    for (int i = 0; i < cnt; i++) {
        sins.push_back(sin(2 * M_PI * i / cnt));
        sum += sins.back();
    }
    std::cout << type << sum << std::endl; 
    return 0;
}