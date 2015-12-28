
#ifndef SRC_UTILS_CONSOLE_COLORS_H
#define SRC_UTILS_CONSOLE_COLORS_H

#include <stdio.h>

#define danger_text(txt) \
  printf("    \x1B[31m%s\e[0m\n", txt);

#endif
