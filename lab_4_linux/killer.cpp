#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <strings.h>
#include <cstring>

void KillProcessByID(int pid) {
    if (kill(pid, SIGKILL) == 0) {
        std::cout << "Процесс PID " << pid << " успешно убит." << std::endl;
    } else {
        perror("Ошибка при убийстве процесса");
    }
}

void KillProcessByName(std::string targetName) {
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir("/proc")) == NULL) {
        perror("Не удалось открыть /proc");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (!isdigit(*entry->d_name)) continue;

        int pid = std::stoi(entry->d_name);

        std::string commPath = "/proc/" + std::string(entry->d_name) + "/comm";
        std::ifstream cmdFile(commPath);

        if (cmdFile.is_open()) {
            std::string procName;
            std::getline(cmdFile, procName);


            if (!procName.empty() && procName.back() == '\n')
                procName.pop_back();


            if (strcasecmp(procName.c_str(), targetName.c_str()) == 0) {
                std::cout << "Найден процесс: " << procName << " (PID: " << pid << ")" << std::endl;
                KillProcessByID(pid);
            }
            cmdFile.close();
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]) {

    std::cout << "Killer started" << std::endl;

    if (argc >= 3) {
        std::string argType = argv[1];
        std::string argValue = argv[2];

        if (argType == "--id") {
            try {
                int pid = std::stoi(argValue);
                KillProcessByID(pid);
            } catch (...) {
                std::cerr << "Некорректный ID" << std::endl;
            }
        }
        else if (argType == "--name") {
            KillProcessByName(argValue);
        }
    }


    char* envVal = getenv("PROC_TO_KILL");

    if (envVal != NULL) {
        std::string envStr = envVal;
        std::cout << "Найдена переменная PROC_TO_KILL: " << envStr << std::endl;

        std::stringstream ss(envStr);
        std::string segment;

        while (std::getline(ss, segment, ',')) {
            const auto strBegin = segment.find_first_not_of(" \t");
            if (strBegin == std::string::npos) continue;
            const auto strEnd = segment.find_last_not_of(" \t");
            const auto range = strEnd - strBegin + 1;
            std::string cleanName = segment.substr(strBegin, range);

            KillProcessByName(cleanName);
        }
    } else {
        std::cout << "Переменная PROC_TO_KILL не задана." << std::endl;
    }

    return 0;
}
