
#include <zephyr/shell/shell.h>
#include <stdlib.h>

#include "battery_level_private.h"

static int post_temperature_to_bus(const struct shell *sh, size_t argc, char **argv) {
    int err_count = 0;

    if ((2 + 1) != argc) {
        shell_error(sh, "Invalid number of arguments");
        return -EINVAL;
    }

    int voltage_mv_int = atoi(argv[1]);
    if ((voltage_mv_int < 0) || (voltage_mv_int > INT16_MAX))
    {
        shell_error(sh, "Voltage %d out of range.", voltage_mv_int);
        err_count++;
    }

    int percent_int = atoi(argv[2]);
    if ((percent_int < 0) || (percent_int > 100)) {
        shell_error(sh, "Percent %d out of range.", percent_int);
        err_count++;
    }

    int status = 0;
    if (0 == err_count) {
        int16_t voltage_mv = (int16_t)voltage_mv_int;
        uint8_t percent = (uint8_t)percent_int;
        int status = publish_data_to_channel(voltage_mv, percent);

        shell_print(sh, "\n\
Bus updated:\n\
    voltage_mv: %d\n\
    percent:    %d\n\
Status: %s\n\
        \n", voltage_mv, percent, (0 == status) ? "OK" : "NOK");
    }

    return ((0 != err_count) || (0 != status)) ? (-EINVAL) : (0);
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    battery_level_shell_subcmds,
    SHELL_CMD(update_bus, NULL, "Update temperature to bus.", post_temperature_to_bus),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(
    battery_level, 
    &battery_level_shell_subcmds, 
    "Battery level commands", 
    NULL
);