#include <stdlib.h>

#include <zephyr/shell/shell.h>

#include "utils.h"

int utils_single_int16_arg_shell_cmd(const struct shell *sh, size_t argc, char **argv, void *setter) {
    int err_count = 0;

    if ((1+1) != argc) {
        shell_error(sh, "Invalid number of arguments.");
        return -EINVAL;
    }

    int value_int = atoi(argv[1]);
    if ((value_int < INT16_MIN) || (value_int > INT16_MAX)) {
        shell_error(sh, "Argument value %d out of int16_t range.", value_int);
        err_count++;
    }
    
    if (NULL == setter) {
        shell_error(sh, "Run-time error. No setter function.");
        err_count++;
    }

    if (0 == err_count) {
        int16_t value_int16 = (int16_t)value_int;
        utils_single_int16_t_setter fp_setter = (utils_single_int16_t_setter)setter;
        err_count = fp_setter(value_int16);
    }

    if (0 == err_count) {
        shell_print(sh, "OK");
    } else {
        shell_error(sh, "NOK");
    }

    return (0 != err_count) ? (-EINVAL) : (0);
}

int utils_single_uint16_arg_shell_cmd(const struct shell *sh, size_t argc, char **argv, void *setter) {
    int err_count = 0;

    if ((1+1) != argc) {
        shell_error(sh, "Invalid number of arguments.");
        return -EINVAL;
    }

    int value_int = atoi(argv[1]);
    if ((value_int < 0) || (value_int > UINT16_MAX)) {
        shell_error(sh, "Argument value %d out of uint16_t range.", value_int);
        err_count++;
    }
    
    if (NULL == setter) {
        shell_error(sh, "Run-time error. No setter function.");
        err_count++;
    }

    if (0 == err_count) {
        uint16_t value_uint16 = (int16_t)value_int;
        utils_single_uint16_t_setter fp_setter = (utils_single_uint16_t_setter)setter;
        err_count = fp_setter(value_uint16);
    }

    if (0 == err_count) {
        shell_print(sh, "OK");
    } else {
        shell_error(sh, "NOK");
    }

    return (0 != err_count) ? (-EINVAL) : (0);
}

int utils_single_int16_ret_shell_cmd(const struct shell *sh, size_t argc, char **argv, void *getter) {
    int err_count = 0;
    
    if (NULL == getter) {
        shell_error(sh, "Run-time error. No setter function.");
        err_count++;
    }

    if (0 == err_count) {
        utils_single_int16_t_getter fp_getter = (utils_single_int16_t_getter)getter;
        int16_t value = fp_getter();
        shell_print(sh, "%d", value);
    }

    if (0 == err_count) {
        shell_print(sh, "OK");
    } else {
        shell_error(sh, "NOK");
    }

    return (0 != err_count) ? (-EINVAL) : (0);
}

int utils_single_uint16_ret_shell_cmd(const struct shell *sh, size_t argc, char **argv, void *getter) {
    int err_count = 0;
    
    if (NULL == getter) {
        shell_error(sh, "Run-time error. No setter function.");
        err_count++;
    }

    if (0 == err_count) {
        utils_single_uint16_t_getter fp_getter = (utils_single_uint16_t_getter)getter;
        uint16_t value = fp_getter();
        shell_print(sh, "%d", value);
    }

    if (0 == err_count) {
        shell_print(sh, "OK");
    } else {
        shell_error(sh, "NOK");
    }

    return (0 != err_count) ? (-EINVAL) : (0);
}

