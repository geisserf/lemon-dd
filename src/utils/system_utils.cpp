#include "system_utils.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/sysinfo.h"

// returns the available total virtual memory
long SystemUtils::get_total_virtual_memory() {
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long res = memInfo.totalram;

    res += memInfo.totalswap;
    res *= memInfo.mem_unit;
    return res;
}

// returns the virtual memory used (by all processes)
long SystemUtils::get_used_virtual_memory() {
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long res = memInfo.totalram - memInfo.freeram;
    res += memInfo.totalswap - memInfo.freeswap;
    res *= memInfo.mem_unit;
    return res;
}

int SystemUtils::parse_line(char* line) {
    int i = (int)strlen(line);
    while (*line < '0' || *line > '9') {
        line++;
    }
    line[i - 3] = '\0';
    i = atoi(line);
    return i;
}

// returns the virtual memory used by this process in KB
int SystemUtils::get_virtual_memory_used_by_this() {
    FILE* file = fopen("/proc/self/status", "r");
    int res = -1;
    char line[128];

    while (fgets(line, 128, file) != nullptr) {
        if (strncmp(line, "VmSize:", 7) == 0) {
            res = parse_line(line);
            break;
        }
    }
    fclose(file);
    return res;
}

// returns the available total RAM
long SystemUtils::get_total_ram() {
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long res = memInfo.totalram;
    res *= memInfo.mem_unit;
    return res;
}

// returns the RAM used (by all processes)
long SystemUtils::get_used_ram() {
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    long res = memInfo.totalram - memInfo.freeram;
    res *= memInfo.mem_unit;
    return res;
}

// returns the RAM used by this process in KB
int SystemUtils::get_ram_used_by_this() {
    FILE* file = fopen("/proc/self/status", "r");
    int res = -1;
    char line[128];

    while (fgets(line, 128, file) != nullptr) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            res = parse_line(line);
            break;
        }
    }
    fclose(file);
    return res;
}
