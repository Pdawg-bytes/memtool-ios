#include <stdio.h>
#include <stdlib.h>
#include <mach/vm_statistics.h>
#include <sys/types.h>
#include <sys/sysctl.h>

unsigned long long get_free_bytes(vm_statistics64_data_t vm_stats, vm_size_t page_size) {
    return (int64_t)vm_stats.free_count * (int64_t)page_size;
}

unsigned long long get_total_bytes() {
    int mib[2];
    size_t len;
    uint64_t total_memory_bytes;

    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    len = sizeof(total_memory_bytes);

    if (sysctl(mib, 2, &total_memory_bytes, &len, NULL, 0) == 0) {
        return total_memory_bytes;
    } else {
        perror("sysctl");
        return 1;
    }

    return 1;
}

unsigned long long get_used_bytes(unsigned long long total, unsigned long long free) {
    return total - free;
}