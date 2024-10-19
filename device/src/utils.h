#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

typedef int(*utils_single_int16_t_setter)(int16_t value);
int utils_single_int16_arg_shell_cmd(const struct shell *sh, size_t argc, char **argv, void *setter);

typedef int(*utils_single_uint16_t_setter)(uint16_t value);
int utils_single_uint16_arg_shell_cmd(const struct shell *sh, size_t argc, char **argv, void *setter);

typedef int(*utils_single_int16_t_getter)(void);
int utils_single_int16_ret_shell_cmd(const struct shell *sh, size_t argc, char **argv, void *getter);

typedef int(*utils_single_uint16_t_getter)(void);
int utils_single_uint16_ret_shell_cmd(const struct shell *sh, size_t argc, char **argv, void *getter);


#endif /* _UTILS_H_ */