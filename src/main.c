#include "main.h"
#include "memory_info.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>

action_type get_action_type(const char* action) {
    if (strcmp(action, "--free") == 0) {
        return ACTION_FREE;
    } else if (strcmp(action, "--used") == 0) {
        return ACTION_USED;
    } else if (strcmp(action, "--total") == 0) {
        return ACTION_TOTAL;
    } else if (strcmp(action, "--pagesize") == 0) {
        return ACTION_PAGESIZE;
    } else if (strcmp(action, "--help") == 0) {
        return ACTION_HELP;
    } else if (strcmp(action, "--version") == 0) {
        return ACTION_VERSION;
    } else {
        return ACTION_INVALID;
    }
}

memory_unit get_memory_unit(const char* unit_option) {
    if (strcmp(unit_option, "--mb") == 0) {
        return UNIT_MB;
    } else if (strcmp(unit_option, "--mib") == 0) {
        return UNIT_MIB;
    } else if (strcmp(unit_option, "--kb") == 0) {
        return UNIT_KB;
    } else if (strcmp(unit_option, "--kib") == 0) {
        return UNIT_KIB;
    } else if (strcmp(unit_option, "--bytes") == 0) {
        return UNIT_BYTES;
    } else {
        return UNIT_BYTES;
    }
}

void print_memory(unsigned long long size_in_bytes, memory_unit unit, bool show_unit) {
    switch (unit) {
        case UNIT_BYTES:
            printf("%llu", size_in_bytes);
            break;
        case UNIT_MB:
            printf("%.2f", (float)size_in_bytes / (1000 * 1000));
            break;
        case UNIT_MIB:
            printf("%.2f", (float)size_in_bytes / (1024 * 1024));
            break;
        case UNIT_KB:
            printf("%.2f", (float)size_in_bytes / 1000);
            break;
        case UNIT_KIB:
            printf("%.2f", (float)size_in_bytes / 1024);
            break;
    }

    if (show_unit) {
        switch (unit) {
            case UNIT_BYTES:
                printf(" bytes\n");
                break;
            case UNIT_MB:
                printf(" MB\n");
                break;
            case UNIT_MIB:
                printf(" MiB\n");
                break;
            case UNIT_KB:
                printf(" KB\n");
                break;
            case UNIT_KIB:
                printf(" KiB\n");
                break;
        }
    } else {
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("Classic iOS Memtool - Pdawg, 2023\nType '--help' for more.\n");
        return 0;
    }

    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;

    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);

    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) && KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO, (host_info64_t)&vm_stats, &count)) {
        bool show_unit = true;
        memory_unit unit = UNIT_BYTES;
        action_type action = ACTION_INVALID;

        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-') {
                if (strcmp(argv[i], "--nounit") == 0) {
                    show_unit = false;
                } else {
                    memory_unit next_unit = get_memory_unit(argv[i]);
                    if (next_unit != UNIT_BYTES) {
                        unit = next_unit;
                    } else {
                        action_type next_action = get_action_type(argv[i]);
                        if (next_action != ACTION_INVALID) {
                            action = next_action;
                        } else {
                            printf("Invalid option: %s\n", argv[i]);
                            return 1;
                        }
                    }
                }
            } else {
                printf("Invalid option: %s\n", argv[i]);
                return 1;
            }
        }

        if (action != ACTION_INVALID) {
            unsigned long long total_memory_bytes = get_total_bytes();
            unsigned long long free_memory_bytes = get_free_bytes(vm_stats, page_size);
            unsigned long long used_memory_bytes = get_used_bytes(total_memory_bytes, free_memory_bytes);
            unsigned long long pagesize_bytes = page_size;

            bool useMemoryPrint = true;

            unsigned long long size_in_bytes = 0;
            switch (action) {
                case ACTION_FREE:
                    useMemoryPrint = true;
                    size_in_bytes = free_memory_bytes;
                    break;
                case ACTION_USED:
                    useMemoryPrint = true;
                    size_in_bytes = used_memory_bytes;
                    break;
                case ACTION_TOTAL:
                    useMemoryPrint = true;
                    size_in_bytes = total_memory_bytes;
                    break;
                case ACTION_PAGESIZE:
                    useMemoryPrint = true;
                    size_in_bytes = pagesize_bytes;
                    break;
                case ACTION_HELP:
                    useMemoryPrint = false;
                    printf("\033[1mMemtool help\033[0m\n");
                    printf("\033[95mActions:\n\033[0m\033[96m--total: \033[0mPrints your devices total memory size.\n\033[96m--free: \033[0mPrints the amount of free memory.\n\033[96m--used: \033[0mPrints the amount of used RAM by the system.\n\033[96m--pagesize: \033[0mPrints the memory page size.\n\033[96m--version: \033[0mPrints the version of memtool that you are currently using.\n\033[96m--help: \033[0mDisplays flags and commands for memtool.\n");
                    printf("\n\033[95mFlags:\n\033[0m\033[96m--nounit: \033[0mHides any unit identifier in the output.\n\033[96m--mb: \033[0mPrints the count in MB.\n\033[96m--mib: \033[0mPrints the count in MiB.\n\033[96m--kb: \033[0mPrints the count in KB.\n\033[96m--kib: \033[0mPrints the count in KiB.\nBytes are the default flag. Do not use --bytes, it will not work.\n");
                    break;
                case ACTION_VERSION:
                    useMemoryPrint = false;
                    printf("\033[35m                          _              _ \n");
                    printf(" _ __ ___   ___ _ __ ___ | |_ ___   ___ | |\n");
                    printf("| '_ ` _ \\ / _ \\ '_ ` _ \\| __/ _ \\ / _ \\| |\n");
                    printf("\033[95m| | | | | |  __/ | | | | | || (_) | (_) | |\n");
                    printf("\033[97m|_| |_| |_|\\___|_| |_| |_|\\__\\___/ \\___/|_|\n\n");
                    printf("\033[0m");
                    printf("Memtool 1.0.0, Pdawg 2023\n");
                    break;
                case ACTION_INVALID:
                    useMemoryPrint = false;
                    printf("[-] Undefinied error in action parser.");
                    break;
            }
            if (useMemoryPrint) { print_memory(size_in_bytes, unit, show_unit); }

            return 0;
        }
    }

    return 1;
}