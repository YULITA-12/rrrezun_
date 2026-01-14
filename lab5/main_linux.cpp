#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sstream>
#include <cmath>

const int N = 10;

using namespace std;

void runM() {
    int x;
    while (cin >> x) {
        cout << (x * 7) << " ";
    }
    cout << endl; // Сброс буфера
}

void runA() {
    int x;
    while (cin >> x) {
        cout << (x + N) << " ";
    }
    cout << endl;
}

void runP() {
    long long x;
    while (cin >> x) {
        cout << (x * x * x) << " ";
    }
    cout << endl;
}

void runS() {
    long long x;
    long long sum = 0;
    while (cin >> x) {
        sum += x;
    }
    cerr << "Результат работы цепочки (S): " << sum << endl;
}

void createProcess(void (*func)(), int readPipe[], int writePipe[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        if (readPipe != nullptr) {
            dup2(readPipe[0], STDIN_FILENO);
            close(readPipe[0]);
            close(readPipe[1]);
        }
        if (writePipe != nullptr) {
            dup2(writePipe[1], STDOUT_FILENO);
            close(writePipe[0]);
            close(writePipe[1]);
        }

        func();
        exit(0);
    }
}

int main() {
    int p1[2], p2[2], p3[2], p4[2];

    if (pipe(p1) == -1  pipe(p2) == -1  pipe(p3) == -1 || pipe(p4) == -1) {
        perror("pipe");
        return 1;
    }

    cout << "Запуск процессов..." << endl;

    createProcess(runM, p1, p2);
    createProcess(runA, p2, p3);
    createProcess(runP, p3, p4);
    createProcess(runS, p4, nullptr);
  
    close(p1[0]);
    close(p2[0]); close(p2[1]);
    close(p3[0]); close(p3[1]);
    close(p4[0]); close(p4[1]);

    string inputData = "1 2 3";
    cout << "Main отправляет данные: " << inputData << endl;
    cout << "Ожидаемый расчет: " << endl;
    cout << "1 -> 7*1 -> 7+" << N << "=" << 17 << " -> 17^3=" << 17*17*17 << endl;

    write(p1[1], inputData.c_str(), inputData.length());

    close(p1[1]);

    for (int i = 0; i < 4; i++) {
        wait(NULL);
    }

    cout << "Все процессы завершены." << endl;

    return 0;
}
