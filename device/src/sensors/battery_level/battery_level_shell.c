
#include <zephyr/shell/shell.h>
#include <stdlib.h>

#include "battery_level_private.h"
#include "utils.h"

static int set_voltage_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return utils_single_int16_arg_shell_cmd(sh, argc, argv, set_voltage);
}

static int set_update_rate_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return utils_single_uint16_arg_shell_cmd(sh, argc, argv, set_update_rate);
}

static int set_nominal_voltage_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return utils_single_int16_arg_shell_cmd(sh, argc, argv, set_nominal_voltage);
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    battery_level_set_shell_subcmds,
    SHELL_CMD(voltage, NULL, "Set voltage [mV]", set_voltage_cmd_handler),
    SHELL_CMD(update_rate, NULL, "Set update rate [ms]", set_update_rate_cmd_handler),
    SHELL_CMD(nominal_voltage, NULL, "Set nominal voltage [mV]", set_nominal_voltage_cmd_handler),
    SHELL_SUBCMD_SET_END
);

static int get_voltage_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return utils_single_int16_ret_shell_cmd(sh, argc, argv, get_voltage);
}

static int get_update_rate_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return utils_single_uint16_ret_shell_cmd(sh, argc, argv, get_update_rate);
}

static int get_nominal_voltage_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return utils_single_int16_ret_shell_cmd(sh, argc, argv, get_nominal_voltage);
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    battery_level_get_shell_subcmds,
    SHELL_CMD(voltage, NULL, "Get voltage [mV]", get_voltage_cmd_handler),
    SHELL_CMD(update_rate, NULL, "Get update rate [ms]", get_update_rate_cmd_handler),
    SHELL_CMD(nominal_voltage, NULL, "Get nominal voltage [mV]", get_nominal_voltage_cmd_handler),
    SHELL_SUBCMD_SET_END
);

static int enable_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    shell_print(sh, "OK");
    set_enable(1);
    return 0;
}

static int disable_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    shell_print(sh, "OK");
    set_enable(0);
    return 0;
}

static int is_enabled_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    shell_print(sh, "%s", is_enabled() ? ("true") : ("false"));
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    battery_level_shell_subcmds,
    SHELL_CMD(set, &battery_level_set_shell_subcmds, "Set run-time variables.", NULL),
    SHELL_CMD(get, &battery_level_get_shell_subcmds, "Get run-time variables.", NULL),
    SHELL_CMD(enable, NULL, "Enable", enable_cmd_handler),
    SHELL_CMD(disable, NULL, "Disable", disable_cmd_handler),
    SHELL_CMD(is_enabled, NULL, "Is enabled [true|false]", is_enabled_cmd_handler),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(
    battery_level, 
    &battery_level_shell_subcmds, 
    "Battery level commands", 
    NULL
);