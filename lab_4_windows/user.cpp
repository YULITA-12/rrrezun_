#include <windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>

bool IsProcessRunning(std::string name) {
    bool found = false;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            std::string currentName = pe.szExeFile;
            if (_stricmp(currentName.c_str(), name.c_str()) == 0) {
                found = true;
                break;
            }
        } while (Process32Next(hSnapshot, &pe));
    }
    CloseHandle(hSnapshot);
    return found;
}

DWORD StartProcess(std::string path) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, const_cast<char*>(path.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "Не удалось запустить " << path << std::endl;
        return 0;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    return pi.dwProcessId;
}

void RunKiller(std::string args) {
    std::string cmd = "Killer.exe " + args;
    std::cout << "\nЗапуск Killer с аргументами: " << args << std::endl;
    
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    if (CreateProcess(NULL, const_cast<char*>(cmd.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        std::cerr << "!!! Ошибка запуска Killer.exe. Убедитесь, что он в той же папке." << std::endl;
    }
    std::cout << "Killer завершил работу.\n" << std::endl;
}

int main() {

    std::cout << "\ntest 1: --name notepad.exe ---\n" << std::endl;
    StartProcess("notepad.exe");
    Sleep(1000);

    if (IsProcessRunning("notepad.exe")) 
        std::cout << "Check: Notepad запущен." << std::endl;
    
    RunKiller("--name notepad.exe");

    if (!IsProcessRunning("notepad.exe")) 
        std::cout << "SUCCESS: Notepad больше нет." << std::endl;
    else 
        std::cout << "FAIL: Notepad все еще жив." << std::endl;


    std::cout << "\nID test\n" << std::endl;
    DWORD pid = StartProcess("notepad.exe");
    std::cout << "Запущен Notepad с PID: " << pid << std::endl;
    Sleep(1000);

    if (IsProcessRunning("notepad.exe")) 
        std::cout << "Check: Notepad запущен." << std::endl;

    RunKiller("--id " + std::to_string(pid));

    if (!IsProcessRunning("notepad.exe")) 
        std::cout << "SUCCESS: Notepad больше нет." << std::endl;
    else 
        std::cout << "FAIL: Notepad все еще жив." << std::endl;


    std::cout << "\nEnvironment variable test\n" << std::endl;

    SetEnvironmentVariable("PROC_TO_KILL", "notepad.exe,calc.exe");
    
    StartProcess("notepad.exe");
    StartProcess("calc.exe");
    Sleep(1000);

    if (IsProcessRunning("notepad.exe") && IsProcessRunning("calc.exe"))
        std::cout << "Check: Notepad и Calc запущены." << std::endl;

    RunKiller(""); 

    if (!IsProcessRunning("notepad.exe") && !IsProcessRunning("calc.exe")) 
        std::cout << "SUCCESS: Оба процесса убиты через ENV." << std::endl;
    else 
        std::cout << "FAIL: Кто-то выжил." << std::endl;

    SetEnvironmentVariable("PROC_TO_KILL", NULL);
    std::cout << "Переменная окружения удалена." << std::endl;

    system("pause");
    return 0;
}
