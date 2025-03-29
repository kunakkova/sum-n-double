#include "exact_sum.h"
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <cstring>
#include <cfenv>

using namespace std;

bool ValidateWithGMP(const vector<double>& numbers, double result) {
    int pos_infs = 0, neg_infs = 0;
    bool has_nan = false;
    for (double x : numbers) {
        if (isnan(x)) has_nan = true;
        else if (isinf(x)) (x > 0) ? ++pos_infs : ++neg_infs;
    }

    if (has_nan) return isnan(result);
    if (pos_infs && neg_infs) return isnan(result);
    if (pos_infs) return result == INFINITY;
    if (neg_infs) return result == -INFINITY;

    mpf_t exact_sum;
    mpf_init2(exact_sum, 2048);
    mpf_set_d(exact_sum, 0.0);

    for (double x : numbers) {
        mpf_t tmp;
        mpf_init_set_d(tmp, x);
        mpf_add(exact_sum, exact_sum, tmp);
        mpf_clear(tmp);
    }

    double exact = mpf_get_d(exact_sum);
    mpf_clear(exact_sum);

    uint64_t res_bits, exact_bits;
    memcpy(&res_bits, &result, sizeof(double));
    memcpy(&exact_bits, &exact, sizeof(double));

    // Расширенная диагностика
    cout << "  Точное значение (GMP): " << scientific << exact << endl;
    cout << "  Битовая маска результата: 0x" << hex << setw(16) << res_bits << endl;
    cout << "  Битовая маска эталона:    0x" << hex << setw(16) << exact_bits << dec << endl;
    return res_bits == exact_bits;
}

bool CheckPermutations(const vector<double>& numbers) {
    if (numbers.size() < 2) return true;
    
    const double original = ExactSum(numbers);
    if (isnan(original)) return true;
    
    vector<double> shuffled = numbers;
    shuffle(shuffled.begin(), shuffled.end(), mt19937{random_device{}()});
    const double permuted = ExactSum(shuffled);
    
    return memcmp(&original, &permuted, sizeof(double)) == 0;
}

int main() {
    struct TestCase {
        vector<double> numbers;
        string description;
        double expected;
    };

    vector<TestCase> tests = {
        {{1.0, 1e100, 1.0, -1e100}, 
         "Компенсация больших чисел", 2.0},
        
        {{0.5e308, 0.5e308, -0.5e308}, 
         "Контроль переполнения", 0.5e308},
        
        {{1.5, 2.5, 3.5, -7.55555}, 
         "Точность вычислений", -0.05555},
        
        {{DBL_MIN, DBL_MIN, -DBL_MIN}, 
         "Денормализованные числа", DBL_MIN},
        
        {{1.0, -1.0, DBL_EPSILON, -DBL_EPSILON}, 
         "Балансировка эпсилон", 0.0},
        
        {{1.0, 2.0, 3.0, 4.0, 5.0}, 
         "Простая сумма", 15.0},
        
        {{0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1}, 
         "Сумма 10×0.1", 1.0},
        
        {{NAN, 2.0, 3.0}, 
         "Распространение NaN", NAN},
        
        {{0.0, 0.0, 0.0}, 
         "Сумма нулей", 0.0},
        
        {{1e-300, 1e-300, -1e-300}, 
         "Экстремально малые числа", 1e-300},
        
        {{1e100, 1e-100, -1e100}, 
         "Разные порядки величин", 1e-100},
        
        {{}, 
         "Пустой массив", 0.0}
    };

    for (size_t i = 0; i < tests.size(); ++i) {
        const auto& test = tests[i];
        feclearexcept(FE_ALL_EXCEPT);
        
        double sum = ExactSum(test.numbers);
        bool valid = ValidateWithGMP(test.numbers, sum);
        bool perm_ok = CheckPermutations(test.numbers);

        cout << "Тест #" << i+1 << ": " << test.description << endl
             << "  Ожидаемо:        " << scientific << test.expected << endl
             << "  Полученный результат: ";

        if (isnan(sum)) cout << "NaN";
        else if (isinf(sum)) cout << ((sum > 0) ? "INF" : "-INF");
        else cout << sum;
        
        cout << endl << "  Валидация GMP:    " << (valid ? "УСПЕХ" : "ОШИБКА") 
             << endl << "  Перестановки:     " << (perm_ok ? "СОВПАДАЮТ" : "РАЗЛИЧАЮТСЯ")
             << endl << "  Исключения FPU:   ";
        
        if (fetestexcept(FE_OVERFLOW))  cout << "ПЕРЕПОЛНЕНИЕ ";
        if (fetestexcept(FE_UNDERFLOW)) cout << "АНТИПЕРЕПОЛНЕНИЕ ";
        if (fetestexcept(FE_INVALID))   cout << "НЕВЕРНАЯ ОПЕРАЦИЯ";
        cout << endl << "----------------------------------------" << endl;
    }

    return 0;
}