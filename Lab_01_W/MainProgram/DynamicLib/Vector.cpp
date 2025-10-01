#include "Vector.h"
#include <cmath>

// Конструктор с параметрами
Vector::Vector(Number x_val, Number y_val) {
    x = x_val;
    y = y_val;
}

// Конструктор по умолчанию
Vector::Vector() {
    x = Number(0.0);
    y = Number(0.0);
}

// Длина вектора
Number Vector::getLength() {
    double x_val = x.value;
    double y_val = y.value;
    double result = sqrt(x_val * x_val + y_val * y_val);
    return Number(result);
}

// Угол вектора
Number Vector::getAngle() {
    double x_val = x.value;
    double y_val = y.value;
    double result = atan2(y_val, x_val);
    return Number(result);
}

// Сложение векторов
Vector Vector::add(Vector other) {
    Number new_x = x.plus(other.x);
    Number new_y = y.plus(other.y);
    return Vector(new_x, new_y);
}