#pragma once
#include "Number.h"

class Vector {
public:
    Number x;
    Number y;

    // Конструкторы
    Vector(Number x_val, Number y_val);
    Vector();

    // Функции для работы с векторами
    Number getLength();
    Number getAngle();
    Vector add(Vector other);
};