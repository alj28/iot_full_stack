
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/shell/shell.h>

#include "system.h"
#include "system_private.h"
#include "utils.h"

LOG_MODULE_REGISTER(system_shell, LOG_LEVEL_INF);

static int send_identification_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return send_identification_msg();
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    send_subcmds,
    SHELL_CMD(identification, NULL, "Send identification message.", send_identification_cmd_handler),
    SHELL_SUBCMD_SET_END
);

int set_serial_number(uint32_t value) { return setting_set(&system_settings_handler, eSYSTEM_SETTING_ID_SERIAL_NUMBER, (setting_value_t){.uint32 = value});   }
static int set_serial_number_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return utils_single_uint32_arg_shell_cmd(sh, argc, argv, set_serial_number);
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    system_settings_set_shell_subcmds,
    SHELL_CMD(serial_number, NULL, "Serial number", set_serial_number_cmd_handler),
    SHELL_SUBCMD_SET_END
);

uint32_t get_serial_number(void) {  return setting_get_value(&system_settings_handler, eSYSTEM_SETTING_ID_SERIAL_NUMBER).uint32; }
static int get_serial_number_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return utils_single_uint32_ret_shell_cmd(sh, argc, argv, get_serial_number);
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    system_settings_get_shell_subcmds,
    SHELL_CMD(serial_number, NULL, "Serial number", get_serial_number_cmd_handler),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(
    system_shell_subcmds,
    SHELL_CMD(send, &send_subcmds, "Send message over network.", NULL),
    SHELL_CMD(set, &system_settings_set_shell_subcmds, "Set run-time variables.", NULL),
    SHELL_CMD(get, &system_settings_get_shell_subcmds, "Get run-time variables.", NULL),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(
    system,
    &system_shell_subcmds,
    "System commands.",
    NULL
);