#include <mach/vm_statistics.h>

#ifndef MEMORY_INFO_H
#define MEMORY_INFO_H

unsigned long long get_free_bytes(vm_statistics64_data_t vm_stats, vm_size_t page_size);
unsigned long long get_total_bytes();
unsigned long long get_used_bytes(unsigned long long total, unsigned long long free);

#endif /*MEMORY_INFO_H*/