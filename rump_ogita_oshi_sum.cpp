// Реализация алгоритма Румпа-Огита-Оши для точного вычисления суммы чисел с плавающей запятой.

#include "rump_ogita_oshi_sum.h"
#include <vector>
#include <cmath>

// Функция TwoSum вычисляет сумму двух чисел a и b, а также погрешность t,
// возникающую из-за округления при вычислении суммы.
void TwoSum(double a, double b, double& s, double& t) {
    s = a + b;          // Основная сумма
    double a1 = s - b;  // Восстановление первого слагаемого
    double b1 = s - a1; // Восстановление второго слагаемого
    double da = a - a1; // Погрешность первого слагаемого
    double db = b - b1; // Погрешность второго слагаемого
    t = da + db;        // Итоговая погрешность
}

// Функция ExactSum вычисляет точную сумму вектора чисел по алгоритму Румпа-Огита-Оши.
// Обрабатывает специальные значения (NaN, бесконечности), затем аккумулирует части суммы с учетом погрешностей.
double ExactSum(const std::vector<double>& numbers) {
    bool has_nan = false;
    int pos_infs = 0, neg_infs = 0;

    // Проверка на специальные значения
    for (double x : numbers) {
        if (std::isnan(x)) {
            has_nan = true; // Обнаружен NaN
        } else if (std::isinf(x)) {
            (x > 0) ? ++pos_infs : ++neg_infs; // Счетчик положительных и отрицательных бесконечностей
        }
    }

    // Обработка специальных случаев
    if (has_nan) return NAN;              // Если есть NaN, результат — NaN
    if (pos_infs && neg_infs) return NAN; // Если есть обе бесконечности, результат — NaN
    if (pos_infs) return INFINITY;        // Только положительные бесконечности — +∞
    if (neg_infs) return -INFINITY;       // Только отрицательные бесконечности — -∞

    std::vector<double> parts; // Вектор для хранения частей суммы

    // Основной цикл: добавление чисел с учетом погрешностей
    for (double x : numbers) {
        double residual = x; // Остаток для обработки
        for (size_t i = 0; i < parts.size(); ++i) {
            if (residual == 0.0) break; // Если остаток обнулился, выход
            TwoSum(parts[i], residual, parts[i], residual); // Обновление части и остатка
        }
        if (residual != 0.0) parts.push_back(residual); // Добавление ненулевого остатка
    }

    // Финальное суммирование частей с компенсацией погрешностей
    double sum = 0.0;  // Основная сумма
    double comp = 0.0; // Компенсация погрешности
    for (auto it = parts.rbegin(); it != parts.rend(); ++it) {
        TwoSum(sum, *it, sum, comp); // Постепенное добавление частей
    }
    return sum + comp; // Итоговая точная сумма
}