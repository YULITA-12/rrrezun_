#include <iostream>
#include <windows.h>
#include "Number.h"
#include "Vector.h"

using namespace std;

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    cout << "Тестируем библиотеки!" << endl;
    cout << "=====================" << endl;

    cout << endl << "1. ТЕСТ ЧИСЕЛ:" << endl;

    Number a(5.0);
    Number b(3.0);

    cout << "Число a = " << a.value << endl;
    cout << "Число b = " << b.value << endl;

    Number c = a.plus(b);
    cout << "a + b = " << c.value << endl;

    c = a.minus(b);
    cout << "a - b = " << c.value << endl;

    c = a.multiply(b);
    cout << "a * b = " << c.value << endl;

    c = a.divide(b);
    cout << "a / b = " << c.value << endl;

    cout << endl << "2. ТЕСТ ВЕКТОРОВ:" << endl;

    Vector v1(Number(3.0), Number(4.0));  
    Vector v2(Number(1.0), Number(2.0)); 

    cout << "Вектор 1: (" << v1.x.value << ", " << v1.y.value << ")" << endl;
    cout << "Вектор 2: (" << v2.x.value << ", " << v2.y.value << ")" << endl;

    Number length = v1.getLength();
    cout << "Длина вектора 1: " << length.value << endl;

    Number angle = v1.getAngle();
    cout << "Угол вектора 1: " << angle.value << endl;

    Vector v3 = v1.add(v2);
    cout << "Сумма векторов: (" << v3.x.value << ", " << v3.y.value << ")" << endl;

    cout << endl << "Все тесты завершены!" << endl;

    system("pause");  // Ожидает нажатия любой клавиши
    return 0;
}