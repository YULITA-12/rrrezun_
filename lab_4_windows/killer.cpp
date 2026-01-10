#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

void KillProcessByID(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    
    if (hProcess == NULL) {
        std::cerr << "Не удалось открыть процесс PID: " << pid << std::endl;
        return;
    }

    if (TerminateProcess(hProcess, 1)) {
        std::cout << "Процесс PID " << pid << " успешно завершен." << std::endl;
    } else {
        std::cerr << "Не удалось завершить PID: " << pid << std::endl;
    }

    CloseHandle(hProcess);
}


void KillProcessByName(std::string name) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe)) {
        do {
            std::string currentName = pe.szExeFile;

            std::string nameLower = name;
            std::string currentLower = currentName;
            std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            std::transform(currentLower.begin(), currentLower.end(), currentLower.begin(), ::tolower);

            if (currentLower == nameLower) {
                std::cout << "Найден процесс: " << currentName << " (PID: " << pe.th32ProcessID << ")" << std::endl;
                KillProcessByID(pe.th32ProcessID);
            }
        } while (Process32Next(hSnapshot, &pe));
    }

    CloseHandle(hSnapshot);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    std::cout << "Killer Started" << std::endl;

    if (argc >= 3) {
        std::string argType = argv[1];
        std::string argValue = argv[2];

        if (argType == "--id") {
            DWORD pid = std::stoul(argValue);
            KillProcessByID(pid);
        } 
        else if (argType == "--name") {
            KillProcessByName(argValue);
        }
    }

    const int ENV_BUF_SIZE = 32767;
    char envBuffer[ENV_BUF_SIZE];
    
    if (GetEnvironmentVariable("PROC_TO_KILL", envBuffer, ENV_BUF_SIZE) > 0) {
        std::string envStr = envBuffer;
        std::cout << "Найдена переменная PROC_TO_KILL: " << envStr << std::endl;

        std::stringstream ss(envStr);
        std::string segment;

        while (std::getline(ss, segment, ',')) {
            segment.erase(0, segment.find_first_not_of(' '));
            segment.erase(segment.find_last_not_of(' ') + 1);
            
            if (!segment.empty()) {
                KillProcessByName(segment);
            }
        }
    } else {
        std::cout << "Переменная PROC_TO_KILL не задана или пуста" << std::endl;
    }

    return 0;
}
