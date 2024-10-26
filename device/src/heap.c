
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/sys_heap.h>

extern struct sys_heap _system_heap;

static int cmd_heap_info(const struct shell *shell, size_t argc, char **argv)
{
    struct sys_memory_stats info;
    sys_heap_runtime_stats_get(&_system_heap, &info);

    shell_print(shell, "Free: %u bytes", info.free_bytes);
    shell_print(shell, "Allocated: %u bytes", info.allocated_bytes);
    shell_print(shell, "Max allocated: %u bytes", info.max_allocated_bytes);

    return 0;
}

SHELL_CMD_REGISTER(heap_info, NULL, "Prints the free heap size", cmd_heap_info);
