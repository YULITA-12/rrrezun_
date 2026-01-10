#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <strings.h>


bool IsProcessRunning(std::string targetName) {
    DIR* dir;
    struct dirent* entry;
    bool found = false;

    if ((dir = opendir("/proc")) == NULL) return false;

    while ((entry = readdir(dir)) != NULL) {
        if (!isdigit(*entry->d_name)) continue;

        std::string commPath = "/proc/" + std::string(entry->d_name) + "/comm";
        std::ifstream cmdFile(commPath);
        if (cmdFile.is_open()) {
            std::string procName;
            std::getline(cmdFile, procName);
            if (!procName.empty() && procName.back() == '\n') procName.pop_back();

            if (strcasecmp(procName.c_str(), targetName.c_str()) == 0) {
                found = true;
                cmdFile.close();
                break;
            }
            cmdFile.close();
        }
    }
    closedir(dir);
    return found;
}


pid_t StartVictim(const char* program, const char* arg) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return -1;
    }
    if (pid == 0) {
        execlp(program, program, arg, NULL);
        perror("exec failed");
        exit(1);
    }

    return pid; 
}

void RunKiller(std::string arg1, std::string arg2 = "") {
    std::cout << "\nЗапуск Killer с аргументами: " << arg1 << " " << arg2 << std::endl;
    
    pid_t pid = fork();

    if (pid == 0) {
        if (arg2.empty())
            execlp("./Killer", "./Killer", arg1.c_str(), NULL);
        else
            execlp("./Killer", "./Killer", arg1.c_str(), arg2.c_str(), NULL);
        
        perror("Не удалось запустить ./Killer");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        std::cout << "Killer завершил работу.\n" << std::endl;
    }
}

int main() {
    std::cout << "Linux User started" << std::endl;


    std::cout << "\n test 1: name sleep" << std::endl;
    StartVictim("sleep", "100");
    usleep(100000);

    if (IsProcessRunning("sleep")) std::cout << "Check: sleep запущен." << std::endl;

    RunKiller("--name", "sleep");

    if (!IsProcessRunning("sleep")) 
        std::cout << "SUCCESS: sleep убит." << std::endl;
    else 
        std::cout << "FAIL: sleep жив." << std::endl;


    std::cout << "\n test 2: --id" << std::endl;
    pid_t pid = StartVictim("sleep", "200");
    std::cout << "Запущен sleep с PID: " << pid << std::endl;
    usleep(100000);

    RunKiller("--id", std::to_string(pid));

    if (kill(pid, 0) == -1) 
        std::cout << "SUCCESS: sleep (ID) убит." << std::endl;
    else {
        std::cout << "FAIL: PID все еще существует." << std::endl;
        kill(pid, SIGKILL);
    }

    std::cout << "\n test 3: environment variable" << std::endl;

    setenv("PROC_TO_KILL", "sleep, cat", 1); 

    StartVictim("sleep", "300");

    StartVictim("sleep", "300");
    usleep(100000);

    if (IsProcessRunning("sleep")) std::cout << "Check: sleep запущен." << std::endl;

    std::cout << "\nЗапуск Killer" << std::endl;
    pid_t kpid = fork();
    if (kpid == 0) {
        execlp("./Killer", "./Killer", NULL);
        exit(1);
    } else {
        waitpid(kpid, NULL, 0);
    }

    if (!IsProcessRunning("sleep")) 
        std::cout << "SUCCESS: sleep убит через ENV." << std::endl;
    else {
        std::cout << "FAIL: sleep жив." << std::endl;
        system("killall sleep"); 
    }

    unsetenv("PROC_TO_KILL");

    return 0;
}
