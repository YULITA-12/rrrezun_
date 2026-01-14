#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

const int N = 10;

using namespace std;


void runM() {
    int x;
    while (cin >> x) {
        cout << (x * 7) << " ";
    }
    cout << endl;
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
    cerr << "Результат (S): " << sum << endl;
}

HANDLE startProcess(string exePath, string mode, HANDLE hIn, HANDLE hOut) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hIn;
    si.hStdOutput = hOut;
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    ZeroMemory(&pi, sizeof(pi));

    string cmdLine = "\"" + exePath + "\" " + mode;

    vector<char> cmdBuf(cmdLine.begin(), cmdLine.end());
    cmdBuf.push_back(0);


    if (!CreateProcess(NULL, cmdBuf.data(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        cerr << "Ошибка запуска процесса " << mode << ". Error: " << GetLastError() << endl;
        return NULL;
    }

    CloseHandle(pi.hThread);

    return pi.hProcess;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    if (argc > 1) {
        string mode = argv[1];
        if (mode == "M") runM();
        else if (mode == "A") runA();
        else if (mode == "P") runP();
        else if (mode == "S") runS();
        return 0;
    }

    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    string exePath = string(buffer);

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE; // Важно!
    sa.lpSecurityDescriptor = NULL;

    HANDLE hPipe1_Read, hPipe1_Write;
    HANDLE hPipe2_Read, hPipe2_Write;
    HANDLE hPipe3_Read, hPipe3_Write;
    HANDLE hPipe4_Read, hPipe4_Write;

    if (!CreatePipe(&hPipe1_Read, &hPipe1_Write, &sa, 0) 
        !CreatePipe(&hPipe2_Read, &hPipe2_Write, &sa, 0) 
        !CreatePipe(&hPipe3_Read, &hPipe3_Write, &sa, 0) ||
        !CreatePipe(&hPipe4_Read, &hPipe4_Write, &sa, 0)) {
        cerr << "Ошибка создания пайпов" << endl;
        return 1;
    }

    cout << "Запуск цепочки процессов (Windows)..." << endl;

    vector<HANDLE> processes;


    processes.push_back(startProcess(exePath, "M", hPipe1_Read, hPipe2_Write));

    processes.push_back(startProcess(exePath, "A", hPipe2_Read, hPipe3_Write));

    processes.push_back(startProcess(exePath, "P", hPipe3_Read, hPipe4_Write));
