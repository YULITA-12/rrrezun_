#include "Number.h"

// Конструктор
Number::Number(double val) {
    value = val;//присваивает переданное значение полю value
}

// Сложение
Number Number::plus(Number other) {
    double result = value + other.value;// сложение значения текущего и переданного объектов
    return Number(result);//ВРЕМЕННЫЙ обьект
}

// Вычитание
Number Number::minus(Number other) {
    double result = value - other.value;
    return Number(result);
}

// Умножение
Number Number::multiply(Number other) {
    double result = value * other.value;
    return Number(result);
}

// Деление
Number Number::divide(Number other) {
    double result = value / other.value;
    return Number(result);
}