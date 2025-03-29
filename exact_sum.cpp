#include "exact_sum.h"
#include <vector>
#include <cmath>

void TwoSum(double a, double b, double& s, double& t) {
    s = a + b;
    double a1 = s - b;
    double b1 = s - a1;
    double da = a - a1;
    double db = b - b1;
    t = da + db;
}

double ExactSum(const std::vector<double>& numbers) {
    std::vector<double> parts;

    for (double x : numbers) {
        if (std::isnan(x)) return x;
        if (std::isinf(x)) return x;
        
        double residual = x;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (residual == 0.0) break;
            TwoSum(parts[i], residual, parts[i], residual);
        }
        if (residual != 0.0) parts.push_back(residual);
    }

    double sum = 0.0;
    double comp = 0.0;
    for (auto it = parts.rbegin(); it != parts.rend(); ++it) {
        TwoSum(sum, *it, sum, comp);
    }
    return sum + comp;
}