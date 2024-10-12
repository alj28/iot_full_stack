#ifndef _BOARD_UTILS_H_
#define _BOARD_UTILS_H_

#if defined(CONFIG_ARCH_POSIX)
    #define POSIX_SPECIFIC_SECTION(code)        code
#else
    #define POSIX_SPECIFIC_SECTION(code)
#endif




#endif /* _BOARD_UTILS_H_ */