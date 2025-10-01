#pragma once

class Number {
public:
    double value;//Каждый объект типа Number будет иметь свою собственную копию этой переменной, где хранится фактическое числовое значение.

    // Конструктор
    Number(double val = 0.0);

    // Простые функции
    Number plus(Number other);
    Number minus(Number other);
    Number multiply(Number other);
    Number divide(Number other);
};