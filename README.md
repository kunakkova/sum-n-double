# Сумма N чисел по алгоритму Румпа-Огита-Оши

Репозиторий содержит реализацию алгоритма Румпа-Огита-Оши для точного вычисления суммы чисел с плавающей запятой. Алгоритм обеспечивает повышенную точность по сравнению с обычным суммированием, особенно в случаях, когда числа имеют разные порядки величины или требуется компенсировать ошибки округления.

## Подробное описание алгоритма Румпа-Огита-Оши

Алгоритм Румпа-Огита-Оши (Rump-Ogita-Oishi) — это метод точного вычисления суммы последовательности чисел с плавающей запятой. Он основан на компенсации ошибок округления, которые возникают при сложении чисел в арифметике с плавающей запятой стандарта IEEE 754. Основная цель алгоритма — сохранить точность результата даже при сложении чисел с существенно различающимися масштабами или при накоплении малых погрешностей.

**Идея алгоритма**:  
1. **Декомпозиция ошибок**: Каждая операция сложения разбивается на основную сумму и погрешность с использованием метода `TwoSum`.
2. **Аккумуляция частей**: Все промежуточные суммы и погрешности сохраняются в векторе `parts`, что позволяет отслеживать и корректировать ошибки на каждом шаге.
3. **Иерархическое суммирование**: Финалная сумма вычисляется в обратном порядке с компенсацией остаточных погрешностей, что минимизирует накопление ошибок.

### Принцип работы

- **Компенсация ошибок**: Функция `TwoSum` позволяет разделить операцию сложения на основную сумму и погрешность, сохраняя информацию, которая обычно теряется из-за округления.  
- **Накопление частей**: Вектор `parts` собирает как промежуточные суммы, так и остатки (погрешности), которые корректируются при добавлении каждого нового числа. Это минимизирует накопление ошибок.  
- **Обратное суммирование**: Финальное сложение в обратном порядке с учетом компенсации (`comp`) обеспечивает точный результат, максимально приближенный к математически правильной сумме.  

Алгоритм гарантирует, что результат будет настолько точным, насколько это возможно в рамках представления чисел с плавающей запятой.

## Почему используется алгоритм Румпа-Огита-Оши, а не алгоритм Кэхена?

Алгоритм Кэхена (Kahan Summation Algorithm) — это известный метод компенсации ошибок округления при суммировании чисел с плавающей запятой. Однако алгоритм Румпа-Огита-Оши имеет ряд преимуществ:  

- **Более высокая точность**:  
  - Алгоритм Кэхена компенсирует погрешность только на каждом шаге и сохраняет одну переменную компенсации, что может быть недостаточно для больших наборов данных или чисел с сильно различающимися порядками.  
  - Румпа-Огита-Оши накапливает все промежуточные погрешности в векторе `parts` и учитывает их в финальном суммировании, что обеспечивает более точный результат.  

- **Независимость от порядка**:  
  - Результат алгоритма Кэхена может зависеть от порядка слагаемых, что нежелательно для задач, требующих воспроизводимости.  
  - Румпа-Огита-Оши стремится к инвариантности результата относительно порядка чисел благодаря накоплению и обработке всех частей суммы.  

Таким образом, алгоритм Румпа-Огита-Оши предпочтительнее в задачах, где требуется максимальная точность и надежность при перестановках.

## Описание реализации

Реализация алгоритма состоит из следующих этапов:  
1. **Проверка специальных значений**: Обработка `NaN`, бесконечностей и пустого массива на раннем этапе.  
2. **Основной цикл суммирования**:  
   - Для каждого числа в векторе выполняется итеративная коррекция элементов вектора `parts` с помощью `TwoSum`, чтобы сохранить остатки погрешностей.  
3. **Финальное суммирование**:  
   - Элементы `parts` суммируются в обратном порядке с компенсацией (`comp`), что обеспечивает точный результат.  

Алгоритм избегает использования дополнительной памяти сверх вектора `parts`, что делает его эффективным для больших наборов данных.

## Обработка специальных случаев

Алгоритм корректно обрабатывает особые значения чисел с плавающей запятой:  

- **`NaN`**: Если в векторе есть хотя бы одно `NaN`, результат — `NaN`.  
- **Бесконечности**:  
  - Если есть как `+∞`, так и `-∞`, результат — `NaN`.  
  - Если есть только `+∞`, результат — `+∞`.  
  - Если есть только `-∞`, результат — `-∞`.  
- **Денормализованные числа**: Обрабатываются как обычные числа, сохраняя точность в пределах возможностей арифметики с плавающей запятой.  
- **Пустой массив**: Возвращается `0.0`.  

Эти правила обеспечивают предсказуемое поведение алгоритма в любых условиях.

## Тесты

Файл `tests.cpp` содержит обширный набор тестов для проверки реализации:  

### Описание тестов

Тесты охватывают следующие сценарии:  
- **Базовые случаи**: Простые суммы, нули, повторяющиеся элементы.  
- **Экстремальные значения**: Очень большие (близкие к `DBL_MAX`), очень малые (денормализованные) числа.  
- **Компенсация ошибок**: Суммы с взаимно уничтожающимися слагаемыми (например, `1e100 + 1 - 1e100`).  
- **Специальные значения**: Проверка корректной обработки `NaN`, бесконечностей, комбинаций спецзначений.  

### Валидация

- **Сравнение с GMP**: Используется библиотека GMP (GNU Multiple Precision Arithmetic Library) для вычисления эталонной суммы с высокой точностью. Результаты сравниваются побитово.  
- **Проверка перестановок**: Тесты проверяют, что результат не зависит от порядка чисел в векторе, путем случайного перемешивания входных данных.  

## Компиляция и запуск

### Требования

- Компилятор C++ с поддержкой стандарта C++11 или выше (например, `g++`).  
- Установленная библиотека GMP (`libgmp` и `libgmpxx`).  

### Компиляция

Для компиляции тестов выполните следующую команду:  
```sh
g++ -std=c++11 -o tests tests.cpp rump_ogita_oshi_sum.cpp -lgmpxx -lgmp
```
## Источники

- **Оригинальная работа**:  
  Rump, S.M., Ogita, T., Oishi, S. (2008). *Accurate Floating-Point Summation*. SIAM Journal on Scientific Computing, 30(4), 2008.  

- **Документация GMP**:  
  GNU Multiple Precision Arithmetic Library.  

- **Теоретические основы**:  
  Higham, N.J. (2002). *Accuracy and Stability of Numerical Algorithms*. SIAM.  
  Глава 4: «Summation».

- **Алгоритм Кэхена**:  
  Kahan, W. (1965). *Further remarks on reducing truncation errors*. Communications of the ACM.
