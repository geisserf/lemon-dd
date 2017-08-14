#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <string>

class SystemUtils {
public:
    SystemUtils() = delete;

    static long get_total_virtual_memory();
    static long get_used_virtual_memory();
    static int get_virtual_memory_used_by_this();

    static long get_total_ram();
    static long get_used_ram();
    static int get_ram_used_by_this();

protected:
    static int parse_line(char *line);
};

#endif
