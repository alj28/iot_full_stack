
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/shell/shell.h>

#include "system.h"
#include "system_private.h"

LOG_MODULE_REGISTER(system_shell, LOG_LEVEL_INF);

static int send_identification_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return send_identification_msg();
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    send_subcmds,
    SHELL_CMD(identification, NULL, "Send identification message.", send_identification_cmd_handler),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(
    system_shell_subcmds,
    SHELL_CMD(send, &send_subcmds, "Send message over network.", NULL),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(
    system,
    &system_shell_subcmds,
    "System commands.",
    NULL
);