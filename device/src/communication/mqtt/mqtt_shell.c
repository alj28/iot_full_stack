
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/shell/shell.h>

#include "utils_settings.h"
#include "mqtt_settings.h"
#include "utils.h"


LOG_MODULE_REGISTER(mqtt_shell, LOG_LEVEL_INF);


int set_broker_port(uint16_t value) { return setting_set(&mqtt_settings_handler, eMQTT_SETTING_ID_BROKER_PORT, (setting_value_t){.uint16 = value});   }
static int set_broker_port_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return utils_single_uint16_arg_shell_cmd(sh, argc, argv, set_broker_port);
}

static int set_broker_address_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    mqtt_settings_set_shell_subcmds,
    SHELL_CMD(broker_port, NULL, "Broker port", set_broker_port_cmd_handler),
    SHELL_SUBCMD_SET_END
);

uint16_t get_broker_port(void) {  return setting_get_value(&mqtt_settings_handler, eMQTT_SETTING_ID_BROKER_PORT).uint16; }
static int get_broker_port_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    return utils_single_uint16_ret_shell_cmd(sh, argc, argv, get_broker_port);
}

static int get_broker_address_cmd_handler(const struct shell *sh, size_t argc, char **argv) {
    uint32_t broker_address = setting_get_value(&mqtt_settings_handler, eMQTT_SETTING_ID_BROKER_ADDRESS).uint32;

    uint8_t b0, b1, b2, b3;
    b0 = (uint8_t)((broker_address >> 0) & 0x000000FFUL);
    b1 = (uint8_t)((broker_address >> 8) & 0x000000FFUL);
    b2 = (uint8_t)((broker_address >> 16) & 0x000000FFUL);
    b3 = (uint8_t)((broker_address >> 24) & 0x000000FFUL);

    shell_print(sh, "%d.%d.%d.%d", b3, b2, b1, b0);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    mqtt_settings_get_shell_subcmds,
    SHELL_CMD(broker_port, NULL, "Broker port", get_broker_port_cmd_handler),
    SHELL_CMD(broker_address, NULL, "Broker address.", get_broker_address_cmd_handler),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(
    mqtt_shell_subcmds,
    SHELL_CMD(set, &mqtt_settings_set_shell_subcmds, "Set run-time variables.", NULL),
    SHELL_CMD(get, &mqtt_settings_get_shell_subcmds, "Get run-time variables.", NULL),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(
    mqtt,
    &mqtt_shell_subcmds,
    "MQTT commands.",
    NULL
);